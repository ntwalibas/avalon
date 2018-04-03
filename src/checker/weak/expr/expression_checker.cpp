#include <stdexcept>
#include <utility>
#include <memory>
#include <vector>
#include <string>

/* Expressions */
#include "program/ast/expr/tuple_expression.hpp"
#include "program/ast/expr/list_expression.hpp"
#include "program/ast/expr/map_expression.hpp"
#include "program/ast/expr/expr.hpp"

/* Statement */
#include "program/ast/stmt/expression_stmt.hpp"

/* Declarations */
#include "program/ast/decl/function.hpp"
#include "program/ast/decl/variable.hpp"
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
        else if(an_expression -> is_map_expression()) {
            return check_map(an_expression, l_scope, ns_name);
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

    /**
     * check_map
     * we validate all the expressions that occur within the map
     * in addition, it makes sure that those expressions are all of the same type instance
     */
    type_instance expression_checker::check_map(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<map_expression> const & map_expr = std::static_pointer_cast<map_expression>(an_expression);
        std::vector<std::pair<std::shared_ptr<expr>, std::shared_ptr<expr> > >& elements = map_expr -> get_elements();
        type_instance map_type_instance = inferer::infer(an_expression, l_scope, ns_name);

        // if the inference engine was able to deduce the type instance of the expression
        // we make sure that key values are hashable but only if the type instance is not abstract
        if(map_expr -> has_type_instance() == true && map_type_instance.is_abstract() == false) {
            // get all the functions named has in this scope and find one that hashes values from the key type instance
            std::vector<std::shared_ptr<function> > hash_functions = l_scope -> get_functions(map_type_instance.get_namespace(), "__hash__", 1);

            // get the key type instance
            std::vector<type_instance>& map_instance_params = map_type_instance.get_params();
            type_instance key_type_instance = map_instance_params[0];

            // if no such function was found, we have a problem
            if(hash_functions.size() == 0) {
                throw invalid_type(map_expr -> get_token(), "The map type generated by this expression has a key that's not hashable.");
            }
            // if at least one function was found, we try to find one that accepts our type instance as hash value
            else {
                bool function_found = false;
                for(auto& hash_function : hash_functions) {
                    std::vector<std::pair<std::string, variable> >& function_params = hash_function -> get_params();
                    std::pair<std::string, variable>& function_param = function_params[0];
                    type_instance param_type_instance = function_param.second.get_type_instance();
                    if(type_instance_weak_compare(param_type_instance, key_type_instance) == true) {
                        function_found = true;
                        break;
                    }
                }

                // if the hash function we seek was not found, we raise an exception
                if(function_found == false) {
                    throw invalid_type(map_expr -> get_token(), "The map type generated by this expression has a key that's not hashable.");
                }
            }
        }

        // if the map is not empty, type check all its elements
        if(elements.size() > 0) {
            // we make sure each element in the map has the type instance that map up the map
            std::vector<type_instance>& map_instance_params = map_type_instance.get_params();
            type_instance map_key_instance = map_instance_params[0];
            type_instance map_value_instance = map_instance_params[1];

            // we make sure each element in the map has the same type instance as the first element
            type_instance element_key_instance;
            type_instance element_value_instance;
            int index = 0;
            for(auto& element : elements) {
                element_key_instance = check(element.first, l_scope, ns_name);
                element_value_instance = check(element.second, l_scope, ns_name);
                if(
                   type_instance_weak_compare(map_key_instance, element_key_instance) == false ||
                   type_instance_weak_compare(map_value_instance, element_value_instance) == false
                ) {
                    throw invalid_type(map_expr -> get_token(), "Element number <" + std::to_string(index) + "> in the list has type instance <" + mangle_type_instance(element_key_instance) + ":" + mangle_type_instance(element_value_instance) + "> while the map has type instance <" + mangle_type_instance(map_type_instance) + ">. Please ensure each element in the map is of the proper type instance.");
                }
                index = index + 1;
            }
        }

        return map_type_instance;
    }
}
