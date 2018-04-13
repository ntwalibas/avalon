#include <memory>
#include <vector>

/* Error */
#include "error/error.hpp"

/* Program */
#include "representer/hir/symtable/gtable.hpp"
#include "representer/hir/ast/program.hpp"

/* Scanner */
#include "scanner/scanner.hpp"

/* Lexer */
#include "lexer/token.hpp"
#include "lexer/lexer.hpp"

/* Parser */
#include "parser/parser.hpp"

/* Utils */
#include "utils/exceptions/file_not_found.hpp"
#include "utils/parse_util.hpp"
#include "utils/file_util.hpp"

namespace avalon {
    program parse_util::parse(const std::string& source_path, std::vector<std::string>& search_paths) {
        error error_handler(source_path);
        file_util futil(search_paths);

        // make sure the file given exists and the absolute path to it
        std::string source_abs_path;
        try {
            source_abs_path = futil.get_source_path(source_path);
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
        try {
            std::vector<std::shared_ptr<token> >& tokens = lxr.lex();
            
            // parse the file
            parser psr(tokens, source_path, error_handler);
            program prog;
            try {
                prog = psr.parse();
                return prog;
            } catch(parse_error err) {
                throw err;
            }
        } catch(lex_error err) {
            throw err;
        }
    }
}
