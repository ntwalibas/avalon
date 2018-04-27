#include <memory>
#include <string>

#include "checker/decl/variable/variable_checker.hpp"
#include "checker/exceptions/invalid_expression.hpp"
#include "checker/expr/expression_checker.hpp"
#include "checker/exceptions/invalid_variable.hpp"
#include "checker/decl/type/type_checker.hpp"
#include "checker/exceptions/invalid_type.hpp"
#include "representer/hir/ast/decl/variable.hpp"
#include "representer/hir/symtable/scope.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"


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

        // we only proceed to check the variable if we don't know that it is valid or invalid
        if(variable_decl -> is_valid(UNKNOWN) == false)
            return;

        // we automatically mark the variable as invalid and let it be valid only after the checks below are finished running
        variable_decl -> set_is_valid(INVALID);

        // we make sure it is initalized
        if(variable_val == nullptr) {
            throw invalid_variable(variable_decl -> get_token(), "Variable declarations must be initialized.");
        }

        // if the variable has a type instance, we check it
        if(variable_decl -> has_type_instance()) {
            type_instance& var_instance = variable_decl -> get_type_instance();
            try {
                type_instance_checker::complex_check(var_instance, l_scope, ns_name);
            } catch(invalid_type err) {
                throw invalid_variable(err.get_token(), err.what());
            }
        }

        // we check the initializer expression
        expression_checker checker;
        try {
            type_instance expr_instance = checker.check(variable_val, l_scope, ns_name);
            
            // if the expression type instance is not complete after checking, we raise an error
            if(expr_instance.is_complete() == false) {
                throw invalid_variable(variable_decl -> get_token(), "The initializer expression to a variable expression must be complete.");
            }
            
            // if the variable has a type instance set, we make sure it is the same as the one on the expression
            if(variable_decl -> has_type_instance()) {
                type_instance& var_instance = variable_decl -> get_type_instance();
                if(type_instance_strong_compare(var_instance, expr_instance) == false) {
                    throw invalid_variable(variable_decl -> get_token(), "The variable has a different type instance <" + mangle_type_instance(var_instance) + "> than the expression it is initialized with <" + mangle_type_instance(expr_instance) + ">.");
                }
            }
            else {
                variable_decl -> set_type_instance(expr_instance);
            }            
        } catch(invalid_expression err) {
            throw invalid_variable(err.get_token(), err.what());
        }

        // if we are here, then the variable is valid
        variable_decl -> set_is_valid(VALID);
    }
}
