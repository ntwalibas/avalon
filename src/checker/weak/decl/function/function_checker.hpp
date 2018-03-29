#ifndef AVALON_CHECKER_LAX_DECL_FUNCTION_FUNCTION_CHECKER_HPP_
#define AVALON_CHECKER_LAX_DECL_FUNCTION_FUNCTION_CHECKER_HPP_

#include <utility>
#include <memory>
#include <string>

#include "program/ast/decl/function.hpp"
#include "program/ast/program.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class function_checker {
    public:
        /**
         * a default constructor expects nothing
         */
        function_checker();

        /**
         * check
         * this function makes sure:
         * - all parameters' types and return type are valid
         * - the body of the function is valid
         * - all declarations that make up a function are reachable (conservatively)
         * - all branches do terminate normally (conservatively)
         */
        void check(std::shared_ptr<function>& function_decl, program& prog, const std::string& ns_name);

        /**
         * check_header
         * this function ensures that the function's header (parameters and return type) is of the correct form
         */
        static void check_header(std::shared_ptr<function>& function_decl, program& prog, const std::string& ns_name);

        /**
         * check_body
         * this function ensures that the body of the function is valid:
         * - makes sure that declarations that make up the function's body are either variable declarations or statement declarations
         * - make sure that all return statements return objects of the correct type
         * - make sure that all declarations that make up the function body are valid
         */
        void check_body(std::shared_ptr<function>& function_decl);

        /**
         * check_termination
         * this function makes sure that all declarations in the function's body are reachable and that the function terminates
         */
        void check_termination(std::shared_ptr<function>& function_decl, program& prog);        
    };
}

#endif
