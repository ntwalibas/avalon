#include <stdexcept>
#include <memory>
#include <vector>
#include <string>

/* Expressions */
#include "program/ast/expr/tuple_expression.hpp"
#include "program/ast/expr/list_expression.hpp"
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

/* Exceptions */
#include "checker/exceptions/invalid_type.hpp"


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
    type_instance expression_checker::check(std::shared_ptr<expression_stmt> const & l_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
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
        else if(an_expression -> is_tuple_expression()) {
            return check_tuple(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_list_expression()) {
            return check_list(an_expression, l_scope, ns_name);
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
        return inferer::infer(an_expression, l_scope, ns_name);
    }

    /**
     * check_literal
     * literals are built-in data (constructors)
     * this function simply returns the type instance for each type of literal
     */
    type_instance expression_checker::check_literal(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        return inferer::infer(an_expression, l_scope, ns_name);
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

    /**
     * check_list
     * we validate all the expressions that occur within the list
     * in addition, it makes sure that those expressions are all of the same type instance
     */
    type_instance expression_checker::check_list(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<list_expression> const & list_expr = std::static_pointer_cast<list_expression>(an_expression);
        std::vector<std::shared_ptr<expr> >& elements = list_expr -> get_elements();
        type_instance list_type_instance = inferer::infer(an_expression, l_scope, ns_name);

        // if the list is not empty, type check all its elements
        if(elements.size() > 0) {
            // we get the type instance of the first element as it determines the type instance of the whole list
            std::shared_ptr<expr>& first_element = elements[0];
            type_instance first_element_instance = inferer::infer(first_element, l_scope, ns_name);

            // we make sure each element in the list has the same type instance as the first element
            type_instance element_instance;
            int index = 0;
            for(auto& element : elements) {
                element_instance = check(element, l_scope, ns_name);
                if(type_instance_weak_compare(first_element_instance, element_instance) == false) {
                    throw invalid_type(list_expr -> get_token(), "Element number <" + std::to_string(index) + "> in the list has type instance <" + mangle_type_instance(element_instance) + "> while the list has type instance <" + mangle_type_instance(list_type_instance) + ">. Please ensure each element in the list is of the proper type instance.");
                }
                index = index + 1;
            }
        }

        return list_type_instance;
    }
}
