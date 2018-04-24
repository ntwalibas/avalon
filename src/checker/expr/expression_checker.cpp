#include <stdexcept>
#include <utility>
#include <memory>
#include <vector>
#include <string>
#include <map>

/* Expressions */
#include "representer/hir/ast/expr/identifier_expression.hpp"
#include "representer/hir/ast/expr/grouped_expression.hpp"
#include "representer/hir/ast/expr/tuple_expression.hpp"
#include "representer/hir/ast/expr/list_expression.hpp"
#include "representer/hir/ast/expr/call_expression.hpp"
#include "representer/hir/ast/expr/cast_expression.hpp"
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
#include "checker/decl/variable/variable_checker.hpp"
#include "checker/expr/expression_checker.hpp"
#include "checker/decl/type/type_checker.hpp"

/* Inferer */
#include "inferer/inferer.hpp"

/* Exceptions */
#include "checker/exceptions/invalid_expression.hpp"
#include "checker/exceptions/invalid_variable.hpp"
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
        else if(an_expression -> is_identifier_expression()) {
            return check_identifier(an_expression, l_scope, ns_name, ns_name);
        }
        else if(an_expression -> is_grouped_expression()) {
            return check_grouping(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_cast_expression()) {
            return check_cast(an_expression, l_scope, ns_name);
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
        if(l_scope -> function_exists(sub_ns_name, call_expr -> get_name())) {
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

        // get the constructor
        record_constructor& cons = l_scope -> get_record_constructor(sub_ns_name, call_name, args.size());
        std::map<token,type_instance>& cons_params = cons.get_params();

        // we check the constructor's parameters
        for(auto& arg : args) {
            token& arg_tok = arg.first;
            std::shared_ptr<expr>& arg_val = arg.second;            

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
        // first, we make sure that no parser type instance was attached
        if(call_expr -> type_instance_from_parser()) {
            throw invalid_expression(call_expr -> get_token(), "Function calls cannot have type instances specified. Maybe you wish to specify the return type.");
        }

        std::vector<std::pair<token, std::shared_ptr<expr> > >& args = call_expr -> get_arguments();

        // 1. we start by check the function call arguments
        for(auto& arg : args) {
            type_instance arg_instance = check(arg.second, l_scope, ns_name);            
            if(arg_instance.is_complete() == false)
                throw invalid_expression(arg.second -> expr_token(), "All expressions passed as arguments to a function call must have complete type instances.");
        }

        // 2. we get the function from the inferer as it carries the expression type as well
        function new_fun(star_tok);
        type_instance instance = inferer::infer_function_call(new_fun, call_expr, l_scope, ns_name, sub_ns_name);

        // 3. we make sure arguments name and parameters names coincide if the first argument has non-star token as argument name
        if(args.size() > 0) {
            std::pair<token, std::shared_ptr<expr> > first_arg = args[0];
            bool check_names = first_arg.first != star_tok ? true : false;
            std::vector<std::pair<std::string, variable> >& params = new_fun.get_params();
            auto arg_it = args.begin(), arg_end = args.end();
            auto param_it = params.begin(), param_end = params.end();
            for(; arg_it != arg_end && param_it != param_end; ++arg_it, ++param_it) {
                // we check argument names but only if the first name of all arguments is provided
                if(check_names) {
                    if(arg_it -> first.get_lexeme() != param_it -> first)
                        throw invalid_expression(arg_it -> first, "Expected argument to have name <" + param_it -> first + "> as that is the name of the parameter.");
                }
                // if the first argument name was not provided, we also forbid any other name from being provided
                else {
                    if(arg_it -> first != star_tok)
                        throw invalid_expression(arg_it -> first, "Unexpected argument name. Either provide names for all arguments or none at all.");
                }
            }
        }

        return instance;
    }

    /**
     * check_identifier
     * this function determines the kind of identifier expression this is (default constructor or variable)
     * then dispatches the checking to the appropriate checker.
     */
    type_instance expression_checker::check_identifier(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
        std::shared_ptr<identifier_expression> const & id_expr = std::static_pointer_cast<identifier_expression>(an_expression);

        // we determine whether we have a variable expression or a default constructor expression
        if(l_scope -> variable_exists(sub_ns_name, id_expr -> get_name())) {
            id_expr -> set_expression_type(VAR_EXPR);
            return check_variable(id_expr, l_scope, ns_name, sub_ns_name);
        }
        else if(l_scope -> default_constructor_exists(sub_ns_name, id_expr -> get_name(), 0)) {
            id_expr -> set_expression_type(CONSTRUCTOR_EXPR);
            return check_constructor(id_expr, l_scope, ns_name, sub_ns_name);
        }
        else {
            throw invalid_expression(id_expr -> get_token(), "This identifier isn't bound to any declaration. It is neither a variable or a constructor.");
        }
    }

    /**
     * check_variable
     * we make sure that the variable declaration associated with this variable expression is valid
     */
    type_instance expression_checker::check_variable(std::shared_ptr<identifier_expression> const & id_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
        // get the variable declaration this expression is bound to and check it
        std::shared_ptr<variable>& var_decl = l_scope -> get_variable(sub_ns_name, id_expr -> get_name());

        // check the variable declaration
        variable_checker v_checker;
        try {
            v_checker.check(var_decl, l_scope, sub_ns_name);
        } catch(invalid_variable err) {
            throw invalid_expression(err.get_token(), err.what());
        }

        // we infer the type instance of the variable expression
        type_instance var_instance = inferer::infer_variable(id_expr, l_scope, ns_name, sub_ns_name);

        // set the variable as used
        var_decl -> is_used(true);

        return var_instance;
    }

    /**
     * check_constructor
     * we make sure that the default constructor associated with this identifier expression is valid
     */
    type_instance expression_checker::check_constructor(std::shared_ptr<identifier_expression> const & id_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
        return inferer::infer_constructor(id_expr, l_scope, ns_name, sub_ns_name);
    }

    /**
     * check_grouping
     * we make sure that the expression in the grouping is valid
     */
    type_instance expression_checker::check_grouping(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        // we start by checking the subexpression
        std::shared_ptr<grouped_expression> const & group_expr = std::static_pointer_cast<grouped_expression>(an_expression);
        std::shared_ptr<expr>& value = group_expr -> get_value();
        check(value, l_scope, ns_name);

        // we return the infered type instance
        return inferer::infer(an_expression, l_scope, ns_name, ns_name);
    }

    /**
     * check_cast
     * we make sure there is a function in the will allow the cast given the type instance to cast to and the type instance of the expression
     * it also makes sure the destination type instance has a type builder and the origin expression is valid
     */
    type_instance expression_checker::check_cast(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<cast_expression> const & cast_expr = std::static_pointer_cast<cast_expression>(an_expression);
        type_instance& cast_instance = cast_expr -> get_cast_type_instance();
        std::shared_ptr<expr>& value = cast_expr -> get_val();

        // check the cast type instance
        try {
            type_instance_checker::complex_check(cast_instance, l_scope, ns_name);
            if(cast_instance.is_complete() == false)
                throw invalid_expression(cast_instance.get_token(), "The cast to type instance must be complete.");
        } catch(invalid_type err) {
            throw invalid_expression(err.get_token(), err.what());
        }

        // check the expression to cast
        check(value, l_scope, ns_name);

        // infer the type instance
        function cast_fun(star_tok);
        type_instance instance = inferer::infer_cast(cast_fun, cast_expr, l_scope, ns_name);

        // return the infered type instance
        return instance;
    }
}
