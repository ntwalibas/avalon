#include <stdexcept>
#include <memory>
#include <vector>
#include <string>

/* Expressions */
#include "program/ast/expr/tuple_expression.hpp"
#include "program/ast/expr/expr.hpp"

/* Statement */
#include "program/ast/stmt/expression_stmt.hpp"

/* Declarations */
#include "program/ast/decl/type.hpp"

/* Scope */
#include "program/symtable/scope.hpp"

/* Checker */
#include "checker/weak/expr/expression_checker.hpp"

/* Inferer */
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
        if(an_expression -> is_underscore_expression()) {
            return check_underscore(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_literal_expression()) {
            return check_literal(an_expression, l_scope, ns_name);
        }        
        else {
            throw std::runtime_error("[compiler error] unexpected expression type in expression checker.");
        }
    }


    /**
     * check_underscore
     * returns an abstract type instance since underscore expression can never have a concrete type
     */
    type_instance expression_checker::check_underscore(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        type_instance underscore_type_instance = inferer::infer(an_expression, l_scope, ns_name);
        return underscore_type_instance;
    }

    /**
     * check_literal
     * literals are built-in data (constructors)
     * this function simply returns the type instance for each type of literal
     */
    type_instance expression_checker::check_literal(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        type_instance literal_type_instance = inferer::infer(an_expression, l_scope, ns_name);
        return literal_type_instance;
    }

    /**
     * check_tuple
     * we validate all the expressions that occur within the tuple
     */
    type_instance expression_checker::check_tuple(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<tuple_expression> const & tup_expr = std::static_pointer_cast<tuple_expression>(an_expression);
        std::vector<std::shared_ptr<expr> >& elements = tup_expr -> get_elements();

        // we validate all the expressions that are in the tuple
        for(auto& element : elements)
            check(element, l_scope, ns_name);

        // we derive the tuple type instance
        return inferer::infer(an_expression, l_scope, ns_name);
    }
}
