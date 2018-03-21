#include <memory>
#include <vector>
#include <string>

#include "checker/stage_one/decl/function/header_checker.hpp"
#include "checker/stage_one/decl/type_checker.hpp"
#include "checker/exceptions/invalid_type.hpp"
#include "program/ast/decl/function.hpp"
#include "program/symtable/scope.hpp"
#include "program/ast/decl/type.hpp"
#include "program/ast/program.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * check_header
     * this function ensures that the function's header (parameters and return type) is of the correct form
     */
    void header_checker::check_header(std::shared_ptr<function>& function_decl, program& prog, const std::string& ns_name) {
        std::vector<std::pair<std::string, variable> >& params = function_decl -> get_params();
        std::vector<token>& constraints = function_decl -> get_constraints();
        std::shared_ptr<scope>& l_scope = prog.get_scope();

        // we make sure all the function's parameters type instances are valid
        for(auto& param : params) {
            type_instance param_type_instance = param.second.get_type_instance();
            try {
                type_instance_checker::check(param_type_instance, l_scope, ns_name, constraints);
                // if the type instance checked out, we replace the one on the parameter with the updated on
                param.second.set_type_instance(param_type_instance);
            } catch(invalid_type err) {
                throw err;
            }
        }

        // we make sure the function's return type instance is valid
        type_instance return_type_instance = function_decl -> get_return_type_instance();
        try {
            type_instance_checker::check(return_type_instance, l_scope, ns_name, constraints);
            // if the type instance checked out, we replace the previous return type instance
            function_decl -> set_return_type_instance(return_type_instance);
        } catch(invalid_type err) {
            throw err;
        }
    }
}
