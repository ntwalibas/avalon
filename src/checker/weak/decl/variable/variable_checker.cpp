#include <memory>
#include <string>

#include "checker/weak/decl/variable/variable_checker.hpp"
#include "checker/exceptions/invalid_expression.hpp"
#include "checker/weak/expr/expression_checker.hpp"
#include "checker/exceptions/invalid_variable.hpp"
#include "checker/weak/decl/type/type_checker.hpp"
#include "checker/exceptions/invalid_type.hpp"
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
            expression_checker checker;
            try {
                type_instance expr_instance = checker.check(variable_val, l_scope, ns_name);
                // if the expression has a type instance after checking is done, we work with the variable type instance if any
                if(expr_instance.get_name() != "*" && expr_instance.get_namespace() != "*") {                    
                    // if the variable has a type instance set, we make sure it is the same as the one on the expression
                    if(variable_decl -> has_type_instance()) {
                        type_instance variable_type_instance = variable_decl -> get_type_instance();
                        if(type_instance_weak_compare(variable_type_instance, expr_instance) == false) {
                            throw invalid_variable(variable_decl -> get_token(), "The variable has a different type instance than the expression it is initialized with.");
                        }
                    }
                    else {
                        variable_decl -> set_type_instance(expr_instance);
                    }
                }
                // if the expression has no type instance and the variable has none as well, this is an error
                // this prevents code like "var x = []" because we can't deduce the type of "x"
                else if(expr_instance.get_name() == "*" && expr_instance.get_namespace() == "*" && variable_decl -> has_type_instance() ==  false) {
                    throw invalid_variable(variable_decl -> get_token(), "Variable declaration lacks type instance and none could be derived from the initializer expression. Please provide one.");
                }
            } catch(invalid_expression err) {
                throw err;
            }
        }
    }
}
