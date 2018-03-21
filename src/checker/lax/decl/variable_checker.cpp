#include <memory>
#include <string>

#include "checker/exceptions/invalid_variable.hpp"
#include "checker/lax/decl/variable_checker.hpp"
#include "checker/exceptions/invalid_type.hpp"
#include "checker/lax/decl/type_checker.hpp"
#include "program/ast/decl/variable.hpp"
#include "program/symtable/scope.hpp"
#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"


namespace avalon {
    /**
     * the constructor expects nothing
     */
    variable_checker::variable_checker() {        
    }

    /**
     * check
     * validate the variable declaration
     */
    void variable_checker::check(const std::shared_ptr<variable>& variable_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<expr>& variable_val = variable_decl -> get_value();

        // if the variable is immutable, we make sure it is initalized
        if(variable_decl -> is_mutable() && variable_val == nullptr) {
            throw invalid_variable(variable_decl -> get_token(), "A immutable variable must be initialized on declaration.");
        }

        // if the variable has a type instance, we make sure it has a type
        if(variable_decl -> has_type_instance()) {
            type_instance variable_type_instance = variable_decl -> get_type_instance();
            try {
                type_instance_checker::complex_check(variable_type_instance, l_scope, ns_name);
                // if the type instance checked out, we replace it with the new one that has the type builder attached
                variable_decl -> set_type_instance(variable_type_instance);
            } catch(invalid_type err) {
                throw err;
            }
        }

        // if the variable is initialized, we check the initializer expression
        if(variable_val != nullptr) {
            
        }
    }
}
