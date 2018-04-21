#ifndef AVALON_CHECKER_DECL_FUNCTION_FUNCTION_CHECKER_HPP_
#define AVALON_CHECKER_DECL_FUNCTION_FUNCTION_CHECKER_HPP_

#include <utility>
#include <memory>
#include <string>

#include "representer/hir/ast/decl/function.hpp"
#include "representer/hir/symtable/scope.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * find_function
     * given a function name, parameters' type instances and possibly a return type, get the function that matches all
     */
    std::shared_ptr<function> find_function(const std::string& name, std::vector<type_instance>& param_instances, type_instance& ret_instance, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::vector<token>& standins);
    std::shared_ptr<function> find_function(const std::string& name, std::vector<type_instance>& param_instances, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::vector<token>& standins);


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
        void check(function& function_decl, const std::string& ns_name);

        /**
         * check_header
         * this function ensures that the function's header (parameters and return type) is of the correct form
         */
        static void check_header(function& function_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_body
         * this function ensures that the body of the function is valid:
         * - makes sure that declarations that make up the function's body are either variable declarations or statement declarations
         * - make sure that all return statements return objects of the correct type
         * - make sure that all declarations that make up the function body are valid
         */
        void check_body(function& function_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_termination
         * this function makes sure that all declarations in the function's body are reachable and that the function terminates
         */
        void check_termination(function& function_decl, std::shared_ptr<scope>& l_scope);        
    };
}

#endif
