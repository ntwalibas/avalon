#include <stdexcept>
#include <memory>
#include <string>

#include "checker/weak/expr/expression_checker.hpp"
#include "program/ast/stmt/expression_stmt.hpp"
#include "program/symtable/scope.hpp"
#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "inferer/inferer.hpp"


namespace avalon {
    /**
     * the default constructor expects nothing.
     */
    expression_checker::expression_checker() {        
    }

    /**
     * check
     * given an expression, this functions checks that it is a valid expression
     * and as a side effect it deduces the expression type instance and returns it.
     */
    type_instance expression_checker::check(std::shared_ptr<expression_stmt>& l_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<expr>& an_expression = l_expression -> get_expression();
        return check(an_expression, l_scope, ns_name);
    }

    /**
     * check
     * determines the kind of expression we have then calls the appropriate checker
     */
    type_instance expression_checker::check(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        if(an_expression -> is_underscore_expression())
            return check_underscore(an_expression, l_scope, ns_name);
        
        else
            throw std::runtime_error("[compiler error] unexpected expression type in expression checker.");
    }


    /**
     * check_underscore
     * returns an abstract type instance since underscore expression can never have a concrete type
     */
    type_instance expression_checker::check_underscore(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        type_instance underscore_type_instance = inferer::infer(an_expression, l_scope, ns_name);
        return underscore_type_instance;
    }
}
