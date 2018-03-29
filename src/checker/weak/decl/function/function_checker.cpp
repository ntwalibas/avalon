#include <memory>
#include <string>

#include "checker/weak/decl/function/termination_checker.hpp"
#include "checker/weak/decl/function/function_checker.hpp"
#include "checker/weak/decl/function/header_checker.hpp"
#include "checker/exceptions/invalid_function.hpp"
#include "program/ast/decl/function.hpp"
#include "program/ast/program.hpp"


namespace avalon {
    /**
     * a default constructor expects nothing
     */
    function_checker::function_checker() {        
    }

    /**
     * check
     * this function makes sure:
     * - all parameters' types and return type are valid
     * - the body of the function is valid
     * - all declarations that make up a function are reachable (conservatively)
     * - all branches do terminate normally (conservatively)
     */
    void function_checker::check(std::shared_ptr<function>& function_decl, program& prog, const std::string& ns_name) {
        try {
            // we check the header aka function signature
            check_header(function_decl, prog, ns_name);

            // we check for termination
            check_termination(function_decl, prog);
        } catch(invalid_function err) {
            throw err;
        }
    }

    /**
     * check_header
     * this function ensures that the function's header (parameters and return type) is of the correct form
     */
    void function_checker::check_header(std::shared_ptr<function>& function_decl, program& prog, const std::string& ns_name) {        
        try {
            header_checker::check_header(function_decl, prog, ns_name);
        } catch(invalid_function err) {
            throw err;
        }
    }

    /**
     * check_body
     * this function ensures that the body of the function is valid:
     * - makes sure that declarations that make up the function's body are either variable declarations or statement declarations
     * - make sure that all return statements return objects of the correct type
     * - make sure that all declarations that make up the function body are valid
     */
    void function_checker::check_body(std::shared_ptr<function>& function_decl) {
        
    }

    /**
     * check_termination
     * this function makes sure that all declarations in the function's body are reachable and that the function terminates
     */
    void function_checker::check_termination(std::shared_ptr<function>& function_decl, program& prog) {
        std::shared_ptr<scope>& l_scope = prog.get_scope();
        termination_checker term_checker;
        try {
            term_checker.check_termination(function_decl, l_scope);
        } catch(invalid_function err) {
            throw err;
        }
    }
}
