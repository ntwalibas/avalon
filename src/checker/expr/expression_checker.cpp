#include <stdexcept>
#include <utility>
#include <memory>
#include <vector>
#include <string>
#include <map>

/* Expressions */
#include "representer/hir/ast/expr/tuple_expression.hpp"
#include "representer/hir/ast/expr/list_expression.hpp"
#include "representer/hir/ast/expr/call_expression.hpp"
#include "representer/hir/ast/expr/map_expression.hpp"
#include "representer/hir/ast/expr/expr.hpp"

/* Statement */
#include "representer/hir/ast/stmt/expression_stmt.hpp"

/* Declarations */
#include "representer/hir/ast/decl/function.hpp"
#include "representer/hir/ast/decl/variable.hpp"
#include "representer/hir/ast/decl/type.hpp"

/* Scope */
#include "representer/hir/symtable/scope.hpp"

/* Checker */
#include "checker/expr/expression_checker.hpp"

/* Inferer */
#include "inferer/inferer.hpp"

/* Exceptions */
#include "checker/exceptions/invalid_expression.hpp"
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
        else if(an_expression -> is_call_expression()) {
            return check_call(an_expression, l_scope, ns_name, ns_name);
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
        return inferer::infer(an_expression, l_scope, ns_name, ns_name);
    }

    /**
     * check_literal
     * literals are built-in data (constructors)
     * this function simply returns the type instance for each type of literal
     */
    type_instance expression_checker::check_literal(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        return inferer::infer(an_expression, l_scope, ns_name, ns_name);
    }

    /**
     * check_tuple
     * we validate all the expressions that occur within the tuple
     */
    type_instance expression_checker::check_tuple(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<tuple_expression> const & tup_expr = std::static_pointer_cast<tuple_expression>(an_expression);
        std::vector<std::pair<std::string, std::shared_ptr<expr> > >& elements = tup_expr -> get_elements();

        // we validate all the expressions that are in the tuple
        for(auto& element : elements)
            check(element.second, l_scope, ns_name);

        // we derive the tuple type instance
        return inferer::infer(an_expression, l_scope, ns_name, ns_name);
    }

    /**
     * check_list
     * we validate all the expressions that occur within the list
     * in addition, it makes sure that those expressions are all of the same type instance
     */
    type_instance expression_checker::check_list(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<list_expression> const & list_expr = std::static_pointer_cast<list_expression>(an_expression);
        std::vector<std::shared_ptr<expr> >& elements = list_expr -> get_elements();

        // if the list is not empty, type check all its elements
        if(elements.size() > 0) {
            // we get the type instance of the first element as it determines the type instance of the whole list
            std::shared_ptr<expr>& first_element = elements[0];
            type_instance first_element_instance = check(first_element, l_scope, ns_name);

            // we make sure each element in the list has the same type instance as the first element
            type_instance element_instance;
            int index = 0;
            for(auto& element : elements) {
                element_instance = check(element, l_scope, ns_name);
                if(type_instance_weak_compare(first_element_instance, element_instance) == false) {
                    throw invalid_type(list_expr -> get_token(), "Element number <" + std::to_string(index) + "> in the list has type instance <" + mangle_type_instance(element_instance) + "> while the list has type instance <[" + mangle_type_instance(first_element_instance) + "]>. Please ensure each element in the list is of the proper type instance.");
                }
                index = index + 1;
            }
        }

        return inferer::infer(an_expression, l_scope, ns_name, ns_name);
    }

    /**
     * check_map
     * we validate all the expressions that occur within the map
     * in addition, it makes sure that those expressions are all of the same type instance
     */
    type_instance expression_checker::check_map(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<map_expression> const & map_expr = std::static_pointer_cast<map_expression>(an_expression);
        std::vector<std::pair<std::shared_ptr<expr>, std::shared_ptr<expr> > >& elements = map_expr -> get_elements();
        
        // if the map is not empty, type check all its elements
        if(elements.size() > 0) {
            // we make sure each element in the map has the type instance that map up the map
            std::pair<std::shared_ptr<expr>, std::shared_ptr<expr> >& first_element = elements[0];
            type_instance map_key_instance = check(first_element.first, l_scope, ns_name);
            type_instance map_value_instance = check(first_element.second, l_scope, ns_name);

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
                    throw invalid_type(map_expr -> get_token(), "Element number <" + std::to_string(index) + "> in the list has type instance <" + mangle_type_instance(element_key_instance) + ":" + mangle_type_instance(element_value_instance) + "> while the map has type instance <{" + mangle_type_instance(map_key_instance) + ":" + mangle_type_instance(map_value_instance) + "}>. Please ensure each element in the map is of the proper type instance.");
                }
                index = index + 1;
            }
        }

        // if we are here, then the map is valid as its subexpressions are valid
        // we infer the type instance of the map and perform a last check
        type_instance map_type_instance = inferer::infer(an_expression, l_scope, ns_name, ns_name);

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

        return map_type_instance;
    }

    /**
     * check_call
     * this function determines the kind of expression the call expression is (default constructor, record constructor or function call)
     * then dispatches the checking to the actual checker.
     */
    type_instance expression_checker::check_call(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
        std::shared_ptr<call_expression> const & call_expr = std::static_pointer_cast<call_expression>(an_expression);        

        // we decide if we have a function call or a constructor call
        if(l_scope -> function_exists(ns_name, call_expr -> get_name())) {
            call_expr -> set_expression_type(FUNCTION_CALL_EXPR);
            return check_function_call(call_expr, l_scope, ns_name, sub_ns_name);
        }
        // we check if it is a default constructor or a record constructor
        else {
            if(call_expr -> has_record_syntax()) {
                call_expr -> set_expression_type(DEFAULT_CONSTRUCTOR_EXPR);
                return check_record_constructor(call_expr, l_scope, ns_name, sub_ns_name);
            }
            else {
                call_expr -> set_expression_type(RECORD_CONSTRUCTOR_EXPR);
                return check_default_constructor(call_expr, l_scope, ns_name, sub_ns_name);
            }
        }
    }

    /**
     * check_default_constructor
     * we validate the expressions that occur within the constructor.
     */
    type_instance expression_checker::check_default_constructor(std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::string& sub_ns_name) {        
        std::vector<std::pair<token, std::shared_ptr<expr> > >& args = call_expr -> get_arguments();
        const std::string& call_name = call_expr -> get_name();
        
        // we try to find if we have a default constructor corresponding to the call expression
        if(l_scope -> default_constructor_exists(sub_ns_name, call_name, args.size()) == false) {
            throw invalid_expression(call_expr -> get_token(), "Failed to find a default constructor with the given name and arity in the given namespace.");
        }

        // we check the constructor's parameters
        for(auto& arg : args) {
            token& arg_tok = arg.first;
            std::shared_ptr<expr>& arg_val = arg.second;

            // we make sure that the argument name is "star" since in a default constructors, only anonymous arguments are allowed
            if(arg_tok.get_lexeme() != "*") {
                throw invalid_expression(arg_tok, "Unexpected named argument in default constructor call.");
            }

            // we check the argument value
            check(arg_val, l_scope, ns_name);
        }

        // so we have a default constructor with the given name and arity; we deduce its type instance
        return inferer::infer_default_constructor(call_expr, l_scope, ns_name, sub_ns_name);
    }

    /**
     * check_record_constructor
     * we validate the expressions that occur within the constructor.
     */
    type_instance expression_checker::check_record_constructor(std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
        std::vector<std::pair<token, std::shared_ptr<expr> > >& args = call_expr -> get_arguments();
        const std::string& call_name = call_expr -> get_name();
        
        // we try to find if we have a default constructor corresponding to the call expression
        if(l_scope -> record_constructor_exists(sub_ns_name, call_name, args.size()) == false) {
            throw invalid_expression(call_expr -> get_token(), "Failed to find a record constructor with the given name and arity in the given namespace.");
        }

        // we check the constructor's parameters
        for(auto& arg : args) {
            token& arg_tok = arg.first;
            std::shared_ptr<expr>& arg_val = arg.second;
            record_constructor& cons = l_scope -> get_record_constructor(sub_ns_name, call_name, args.size());
            std::map<token,type_instance>& cons_params = cons.get_params();

            // we make sure that the argument name is not "star" since in a record constructors, all arguments must have names
            if(arg_tok.get_lexeme() == "*") {
                throw invalid_expression(arg_tok, "Excepted the argument's name in record constructor call.");
            }
            // if we have a name, we make sure it exists on the constructor
            else {
                if(cons_params.count(arg_tok) == 0) {
                    throw invalid_expression(arg_tok, "The record constructor that builds this expression does not accept an argument by the name <" + arg_tok.get_lexeme() + ">.");
                }
            }

            // we check the argument value
            check(arg_val, l_scope, ns_name);
        }

        // return the infered type of the record constructor
        return inferer::infer_record_constructor(call_expr, l_scope, ns_name, sub_ns_name);
    }

    /**
     * check_function_call
     * we validate the expressions that were passed as argument to the function
     * we also make sure that all arguments were named or none was
     */
    type_instance expression_checker::check_function_call(std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
        type_instance instance;
        return instance;
    }
}
