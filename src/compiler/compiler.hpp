#ifndef AVALON_COMPILER_HPP_
#define AVALON_COMPILER_HPP_

#include <memory>
#include <vector>

#include <boost/filesystem.hpp>

/* Error */
#include "error/error.hpp"

/* Program */
#include "hir/symtable/gtable.hpp"
#include "hir/ast/program.hpp"

/* Lexer */
#include "lexer/token.hpp"


namespace avalon {
    namespace fs = boost::filesystem;

    class compiler {
    public:
        /**
         * the constructor expects nothing
         */
        compiler();

        /**
         * add_search_paths
         * when compiling, we need to find the location specified by fully qualified names in
         * import declarations.
         * this function allows users to add search paths to search paths global.
         */
        void add_search_path(const std::string& path);
        void add_search_path(const fs::path& p);

        /**
         * source_exists
         * given a source path, this function returns true if the path exists in
         * one of the search paths.
         */
        bool source_exists(const std::string& source_path);

        /**
         * get_source_path
         * given a source file, get the absolute path to it.
         * returns a string containing said absolute path.
         * in case the file was not found, throw "file_not_found" exception.
         */
        std::string get_source_path(const std::string& source_path);

        /**
         * scan
         * calls the scanner to get the text to scan
         */
        std::string scan(const std::string& source_path);

        /**
         * lex
         * calls the lexer to tokenize the source code
         */
        std::vector<std::shared_ptr<token> > lex(const std::string& source_path);

        /**
         * parse
         * calls the parser to generate an AST
         */
        program parse(const std::string& source_path);

        /**
         * import
         * calls the importer to parse all dependencies and make sure there are cyclic dependencies
         */
        gtable import(const std::string& source_path);

        /**
         * check
         * calls the checker to validate the entire program
         */
        //void check(const std::string& source_path);

    private:

        /**
         * a vector of possible search paths
         */
        std::vector<std::string> m_search_paths;
    };
}

#endif
