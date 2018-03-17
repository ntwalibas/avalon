#include <stdexcept>
#include <iostream>
#include <memory>
#include <vector>

#include <boost/filesystem.hpp>

/* Error */
#include "error/error.hpp"

/* Program */
#include "program/symtable/gtable.hpp"
#include "program/ast/program.hpp"

/* Checker */
#include "checker/checker.hpp"

/* Importer */
#include "importer/importer.hpp"

/* Parser */
#include "parser/parser.hpp"

/* Lexer */
#include "lexer/token.hpp"
#include "lexer/lexer.hpp"

/* Scanner */
#include "scanner/scanner.hpp"

/* Compiler */
#include "compiler/exceptions/invalid_directory.hpp"
#include "compiler/exceptions/file_not_found.hpp"
#include "compiler/compiler.hpp"


namespace avalon {
    namespace fs = boost::filesystem;

    /**
     * the constructor expects nothing
     */
    compiler::compiler() {        
    }

    /**
     * add_m_search_paths
     * when compiling, we need to find the location specified by fully qualified names in
     * import declarations.
     * this function allows users to add search paths to search paths global.
     */
    void compiler::add_search_path(const std::string& path) {
        // make sure the given path exists and is a directory
        fs::path p(path);
        if(fs::exists(p) == false || (fs::exists(p) == true && fs::is_directory(p) == false))
            throw invalid_directory("Search path <" + path + "> either doesn't exists or is not a directory or the compiler doesn't have permission to open it.");

        m_search_paths.push_back(path);        
    }

    void compiler::add_search_path(const fs::path& p) {
        if(fs::exists(p) == false || (fs::exists(p) == true && fs::is_directory(p) == false))
            throw invalid_directory("Search path <" + p.string() + "> either doesn't exists or is not a directory or the compiler doesn't have permission to open it.");

        m_search_paths.push_back(p.string());
    }

    /**
     * source_exists
     * given a source path, this function returns true if the path exists in
     * one of the search paths.
     */
    bool compiler::source_exists(const std::string& source_path) {
        for(auto it = m_search_paths.begin(); it != m_search_paths.end(); ++it) {
            fs::path p((*it) + "/" + source_path);
            if(fs::exists(p) && fs::is_regular_file(p))
                return true;
        }

        return false;
    }

    /**
     * get_source_path
     * given a source file, get the absolute path to it.
     * returns a string containing said absolute path.
     * in case the file was not found, throw "file_not_found" exception.
     */
    std::string compiler::get_source_path(const std::string& source_path) {
        for(auto it = m_search_paths.begin(); it != m_search_paths.end(); ++it) {
            fs::path p((*it) + "/" + source_path);
            if(fs::exists(p) && fs::is_regular_file(p)) {
                return p.string();
            }
        }

        throw file_not_found("Failed to open file: <" + source_path + ">. Possible reasons: file does not exist or file is not available for reading.");
    }

    /**
     * scan
     * calls the scanner to get the text to scan
     */
    std::string compiler::scan(const std::string& source_path) {
        error error_handler(source_path);

        // make sure the file given exists and the absolute path to it
        std::string source_abs_path;
        try {
            source_abs_path = get_source_path(source_path);
        } catch(file_not_found err) {
            throw err;
        }

        // scan the file for content
        scanner scr(source_abs_path);
        std::string source;
        try {
            source = scr.scan();
            return source;
        } catch(file_not_found err) {
            throw err;
        }
    }

    /**
     * lex
     * calls the lexer to tokenize the source code
     */
    std::vector<std::shared_ptr<token> > compiler::lex(const std::string& source_path) {
        error error_handler(source_path);

        // make sure the file given exists and the absolute path to it
        std::string source_abs_path;
        try {
            source_abs_path = get_source_path(source_path);
        } catch(file_not_found err) {
            throw err;
        }

        // scan the file for content
        scanner scr(source_abs_path);
        std::string source;
        try {
            source = scr.scan();
        } catch(file_not_found err) {
            throw err;
        }

        // tokenize the file
        lexer lxr(source_abs_path, source, error_handler);
        std::vector<std::shared_ptr<token> > tokens;
        try {
            tokens = lxr.lex();
            return tokens;
        } catch(lex_error err) {
            throw err;
        }
    }

    /**
     * parse
     * calls the parser to generate an AST
     */
    program compiler::parse(const std::string& source_path) {
        error error_handler(source_path);

        // make sure the file given exists and the absolute path to it
        std::string source_abs_path;
        try {
            source_abs_path = get_source_path(source_path);
        } catch(file_not_found err) {
            throw err;
        }

        // scan the file for content
        scanner scr(source_abs_path);
        std::string source;
        try {
            source = scr.scan();
        } catch(file_not_found err) {
            throw err;
        }

        // tokenize the file
        lexer lxr(source_abs_path, source, error_handler);
        std::vector<std::shared_ptr<token> > tokens;
        try {
            tokens = lxr.lex();
        } catch(lex_error err) {
            throw err;
        }

        // parse the file
        parser psr(tokens, source_path, error_handler);
        program prog;
        try {            
            prog = psr.parse();
            return prog;
        } catch(parse_error err) {
            throw err;
        }
    }

    /**
     * import
     * calls the importer to parse all dependencies and make sure there are cyclic dependencies
     */
    gtable compiler::import(const std::string& source_path) {
        error error_handler(source_path);

        // make sure the file given exists and the absolute path to it
        std::string source_abs_path;
        try {
            source_abs_path = get_source_path(source_path);
        } catch(file_not_found err) {
            throw err;
        }

        // scan the file for content
        scanner scr(source_abs_path);
        std::string source;
        try {
            source = scr.scan();
        } catch(file_not_found err) {
            throw err;
        }

        // tokenize the file
        lexer lxr(source_abs_path, source, error_handler);
        std::vector<std::shared_ptr<token> > tokens;
        try {
            tokens = lxr.lex();
        } catch(lex_error err) {
            throw err;
        }

        // parse the file
        parser psr(tokens, source_path, error_handler);
        program prog;
        try {            
            prog = psr.parse();
        } catch(parse_error err) {
            throw err;
        }

        // import all other files
        importer ipr(prog, *this, error_handler);
        gtable gtab;
        try {
            gtab = ipr.import_all();
            return gtab;
        } catch(import_error err) {
            throw err;
        } catch(parse_error err) {
            throw err;
        } catch(lex_error err) {
            throw err;
        } catch(std::runtime_error err) {
            throw err;
        }
    }

    /**
     * check
     * calls the checker to validate the entire program
     */
    void compiler::check(const std::string& source_path) {
        error error_handler(source_path);

        // make sure the file given exists and the absolute path to it
        std::string source_abs_path;
        try {
            source_abs_path = get_source_path(source_path);
        } catch(file_not_found err) {
            error_handler.log(err.what());
            return;
        }

        // scan the file for content
        scanner scr(source_abs_path);
        std::string source;
        try {
            source = scr.scan();
        } catch(file_not_found err) {
            error_handler.log(err.what());
            return;
        }

        // tokenize the file
        lexer lxr(source_abs_path, source, error_handler);
        std::vector<std::shared_ptr<token> > tokens;
        try {
            tokens = lxr.lex();
        } catch(lex_error err) {
            err.show();
            return;
        }

        // parse the file
        parser psr(tokens, source_path, error_handler);
        program prog;
        try {            
            prog = psr.parse();
        } catch(parse_error err) {
            err.show();
            return;
        }

        // import all dependencies
        importer ipr(prog, *this, error_handler);
        gtable gtab;
        try {
            gtab = ipr.import_all();
        } catch(import_error err) {
            err.show();
        } catch(parse_error err) {
            err.show();
        } catch(lex_error err) {
            err.show();
        } catch(std::runtime_error err) {
            error_handler.log(err.what());
        }

        // check the program
        checker ckr(gtab, source_path, error_handler);
        try {
            ckr.check();
        } catch(check_error err) {
            err.show();
        }

        return;
    }
}
