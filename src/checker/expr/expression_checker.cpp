#include <stdexcept>
#include <utility>
#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include <cstdio>
#include <map>

/* Expressions */
#include "representer/hir/ast/expr/assignment_expression.hpp"
#include "representer/hir/ast/expr/identifier_expression.hpp"
#include "representer/hir/ast/expr/grouped_expression.hpp"
#include "representer/hir/ast/expr/literal_expression.hpp"
#include "representer/hir/ast/expr/binary_expression.hpp"
#include "representer/hir/ast/expr/unary_expression.hpp"
#include "representer/hir/ast/expr/match_expression.hpp"
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

/* Builtins */
#include "representer/hir/builtins/avalon_string.hpp"

/* Scope */
#include "representer/hir/symtable/scope.hpp"

/* Checker */
#include "checker/decl/variable/variable_checker.hpp"
#include "checker/expr/expression_checker.hpp"
#include "checker/decl/type/type_checker.hpp"

/* Inferer */
#include "inferer/inferer.hpp"

/* Exceptions */
#include "representer/exceptions/symbol_can_collide.hpp"
#include "representer/exceptions/symbol_not_found.hpp"
#include "checker/exceptions/invalid_expression.hpp"
#include "checker/exceptions/invalid_function.hpp"
#include "checker/exceptions/invalid_variable.hpp"
#include "checker/exceptions/invalid_type.hpp"


namespace avalon {
    /**
     * the default constructor expects nothing.
     */
    expression_checker::expression_checker() : m_inside_match(true) {        
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
        return check(an_expression, l_scope, ns_name, ns_name);
    }

    type_instance expression_checker::check(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
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
            return check_call(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_identifier_expression()) {
            return check_identifier(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_grouped_expression()) {
            return check_grouping(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_cast_expression()) {
            return check_cast(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_unary_expression()) {
            return check_unary(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_binary_expression()) {
            return check_binary(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_match_expression()) {
            return check_match(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_assignment_expression()) {
            return check_assignment(an_expression, l_scope, ns_name);
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
        std::vector<std::pair<std::string, std::shared_ptr<expr> > >& elements = tup_expr -> get_elements();

        // we make sure the expression is not dependent on match expressions
        if(tup_expr -> has_match_expression())
            throw invalid_expression(tup_expr -> get_token(), "A tuple expression cannot depend on a match expression.");

        // we validate all the expressions that are in the tuple
        for(auto& element : elements)
            check(element.second, l_scope, ns_name);

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

        // we make sure the expression is not dependent on match expressions
        if(list_expr -> has_match_expression())
            throw invalid_expression(list_expr -> get_token(), "A list expression cannot depend on a match expression.");

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

        return inferer::infer(an_expression, l_scope, ns_name);
    }

    /**
     * check_map
     * we validate all the expressions that occur within the map
     * in addition, it makes sure that those expressions are all of the same type instance
     */
    type_instance expression_checker::check_map(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<map_expression> const & map_expr = std::static_pointer_cast<map_expression>(an_expression);
        std::vector<std::pair<std::shared_ptr<expr>, std::shared_ptr<expr> > >& elements = map_expr -> get_elements();

        // we make sure the expression is not dependent on match expressions
        if(map_expr -> has_match_expression())
            throw invalid_expression(map_expr -> get_token(), "A map expression cannot depend on a match expression.");
        
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
                    std::vector<std::pair<std::string, std::shared_ptr<variable> > >& function_params = hash_function -> get_params();
                    std::pair<std::string, std::shared_ptr<variable> >& function_param = function_params[0];
                    type_instance param_type_instance = function_param.second -> get_type_instance();
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
    type_instance expression_checker::check_call(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<call_expression> const & call_expr = std::static_pointer_cast<call_expression>(an_expression);
        const std::string& sub_ns_name = call_expr -> get_namespace();

        // we decide if we have a function call or a constructor call
        if(l_scope -> function_exists(sub_ns_name, call_expr -> get_name())) {
            call_expr -> set_expression_type(FUNCTION_CALL_EXPR);
            return check_function_call(call_expr, l_scope, ns_name);
        }
        // we check if it is a default constructor or a record constructor
        else {
            if(call_expr -> has_record_syntax()) {
                call_expr -> set_expression_type(RECORD_CONSTRUCTOR_EXPR);
                return check_record_constructor(call_expr, l_scope, ns_name);
            }
            else {
                call_expr -> set_expression_type(DEFAULT_CONSTRUCTOR_EXPR);
                return check_default_constructor(call_expr, l_scope, ns_name);
            }
        }
    }

    /**
     * check_default_constructor
     * we validate the expressions that occur within the constructor.
     */
    type_instance expression_checker::check_default_constructor(std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::vector<std::pair<token, std::shared_ptr<expr> > >& args = call_expr -> get_arguments();
        const std::string& sub_ns_name = call_expr -> get_namespace();
        const std::string& call_name = call_expr -> get_name();

        // we make sure the expression is not dependent on match expressions
        if(call_expr -> has_match_expression())
            throw invalid_expression(call_expr -> get_token(), "A default constructor expression cannot depend on a match expression.");
        
        // we try to find if we have a default constructor corresponding to the call expression
        if(l_scope -> default_constructor_exists(sub_ns_name, call_name, args.size()) == false) {
            throw invalid_expression(call_expr -> get_token(), "Failed to find a default constructor with the given name and arity in the given namespace.");
        }

        // set up a vector of type instances that we would have if the constructor was complete
        std::vector<type_instance> params;

        // we check the constructor's parameters
        for(auto& arg : args) {
            token& arg_tok = arg.first;
            std::shared_ptr<expr>& arg_val = arg.second;

            // we make sure that the argument name is "star" since in a default constructors, only anonymous arguments are allowed
            if(arg_tok.get_lexeme() != "*") {
                throw invalid_expression(arg_tok, "Unexpected named argument in default constructor call.");
            }

            // we check the argument value
            params.push_back(check(arg_val, l_scope, ns_name));
        }

        // infer the type instance of the constructor
        type_instance instance = inferer::infer_default_constructor(call_expr, l_scope, ns_name);

        // set the constructor name on the call expression
        std::string mangled_name = mangle_constructor(call_name, params, instance);
        call_expr -> set_callee(mangled_name);

        // return the infered type of the record constructor
        return instance;
    }

    /**
     * check_record_constructor
     * we validate the expressions that occur within the constructor.
     */
    type_instance expression_checker::check_record_constructor(std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::vector<std::pair<token, std::shared_ptr<expr> > >& args = call_expr -> get_arguments();
        const std::string& sub_ns_name = call_expr -> get_namespace();
        const std::string& call_name = call_expr -> get_name();

        // we make sure the expression is not dependent on match expressions
        if(call_expr -> has_match_expression())
            throw invalid_expression(call_expr -> get_token(), "A record constructor expression cannot depend on a match expression.");
        
        // we try to find if we have a default constructor corresponding to the call expression
        if(l_scope -> record_constructor_exists(sub_ns_name, call_name, args.size()) == false) {
            throw invalid_expression(call_expr -> get_token(), "Failed to find a record constructor with the given name and arity in the given namespace.");
        }

        // get the constructor
        record_constructor& cons = l_scope -> get_record_constructor(sub_ns_name, call_name, args.size());
        std::map<token,type_instance>& cons_params = cons.get_params();
        std::vector<type_instance> params;

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
            params.push_back(check(arg_val, l_scope, ns_name));
        }

        // infer the type instance of the constructor
        type_instance instance = inferer::infer_record_constructor(call_expr, l_scope, ns_name);

        // set the constructor name on the call expression
        std::string mangled_name = mangle_constructor(call_name, params, instance);
        call_expr -> set_callee(mangled_name);

        // return the infered type of the record constructor
        return instance;
    }

    /**
     * check_function_call
     * we validate the expressions that were passed as argument to the function
     * we also make sure that all arguments were named or none was
     */
    type_instance expression_checker::check_function_call(std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        // first, we make sure that this function call doesn't depend on underscore expressions
        if(call_expr -> has_underscore()) {
            throw invalid_expression(call_expr -> get_token(), "Function calls cannot depend on underscore expressions.");
        }

        // we make sure the expression is not dependent on match expressions
        if(call_expr -> has_match_expression())
            throw invalid_expression(call_expr -> get_token(), "A function call expression cannot depend on a match expression.");

        // we make sure that no parser type instance was attached
        if(call_expr -> type_instance_from_parser()) {
            throw invalid_expression(call_expr -> get_token(), "Function calls cannot have type instances specified. Maybe you wish to specify the return type.");
        }

        std::vector<std::pair<token, std::shared_ptr<expr> > >& args = call_expr -> get_arguments();

        // 1. we start by check the function call arguments
        for(auto& arg : args) {
            type_instance arg_instance = check(arg.second, l_scope, ns_name);
            // we make sire that the argument type instance is complete
            if(arg_instance.is_complete() == false)
                throw invalid_expression(arg.second -> expr_token(), "All expressions passed as arguments to a function call must have complete type instances.");
        }

        // 2. we get the function from the inferer as it carries the expression type as well
        function new_fun(star_tok);
        type_instance instance = inferer::infer_function_call(new_fun, call_expr, l_scope, ns_name);

        // 3. we make sure arguments name and parameters names coincide if the first argument has non-star token as argument name
        if(args.size() > 0) {
            std::pair<token, std::shared_ptr<expr> > first_arg = args[0];
            bool check_names = first_arg.first != star_tok ? true : false;
            std::vector<std::pair<std::string, std::shared_ptr<variable> > >& params = new_fun.get_params();
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

        // 4. set the callee name on the call expression
        call_expr -> set_callee(new_fun.get_name());

        return instance;
    }

    /**
     * check_identifier
     * this function determines the kind of identifier expression this is (default constructor or variable)
     * then dispatches the checking to the appropriate checker.
     */
    type_instance expression_checker::check_identifier(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<identifier_expression> const & id_expr = std::static_pointer_cast<identifier_expression>(an_expression);
        const std::string& sub_ns_name = id_expr -> get_namespace();

        // we determine whether we have a variable expression or a default constructor expression
        if(l_scope -> variable_exists(sub_ns_name, id_expr -> get_name()) || (m_inside_match && id_expr -> type_instance_from_parser())) {
            id_expr -> set_expression_type(VAR_EXPR);
            return check_variable(id_expr, l_scope, ns_name);
        }
        else if(l_scope -> default_constructor_exists(sub_ns_name, id_expr -> get_name(), 0)) {
            id_expr -> set_expression_type(CONSTRUCTOR_EXPR);
            return check_constructor(id_expr, l_scope, ns_name);
        }
        else {
            throw invalid_expression(id_expr -> get_token(), "This identifier isn't bound to any declaration. It is neither a variable or a constructor.");
        }
    }

    /**
     * check_variable
     * we make sure that the variable declaration associated with this variable expression is valid
     */
    type_instance expression_checker::check_variable(std::shared_ptr<identifier_expression> const & id_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        // if we are in the middle of checking a match expression and a type instance was provided then we need to create a variable declaration, not simply check one
        if(m_inside_match && id_expr -> type_instance_from_parser() == true) {
            token var_tok = id_expr -> get_token();
            type_instance& parser_type_instance = id_expr -> get_type_instance();
            std::shared_ptr<variable> var_decl = std::make_shared<variable>(var_tok, false);
            var_decl -> is_public(false);
            var_decl -> is_global(false);
            var_decl -> is_used(true);
            var_decl -> check_initializer(false);
            var_decl -> set_type_instance(parser_type_instance);
            l_scope -> add_variable(ns_name, var_decl);            

            // check the variable declaration
            variable_checker v_checker;
            try {
                v_checker.check(var_decl, l_scope, ns_name);
            } catch(invalid_variable err) {
                throw invalid_expression(err.get_token(), err.what());
            }

            // we infer the type instance of the variable expression
            type_instance var_instance = inferer::infer_variable(id_expr, l_scope, ns_name);
            return var_instance;
        }
        else {
            const std::string& sub_ns_name = id_expr -> get_namespace();
            std::shared_ptr<variable>& var_decl = l_scope -> get_variable(sub_ns_name, id_expr -> get_name());
            std::shared_ptr<expr>& var_val = var_decl -> get_value();

            // we make sure the variable is not initialized with a match match expressions
            if(var_val != nullptr && var_val -> has_match_expression())
                throw invalid_expression(var_decl -> get_token(), "A variable cannot contain a match expression.");

            // check the variable declaration
            variable_checker v_checker;
            try {
                v_checker.check(var_decl, l_scope, sub_ns_name);
            } catch(invalid_variable err) {
                throw invalid_expression(err.get_token(), err.what());
            }

            // we infer the type instance of the variable expression
            type_instance var_instance = inferer::infer_variable(id_expr, l_scope, ns_name);

            // set the variable as used
            var_decl -> is_used(true);

            return var_instance;
        }
    }

    /**
     * check_constructor
     * we make sure that the default constructor associated with this identifier expression is valid
     */
    type_instance expression_checker::check_constructor(std::shared_ptr<identifier_expression> const & id_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        type_instance instance = inferer::infer_constructor(id_expr, l_scope, ns_name);
        std::vector<type_instance> params;
        std::string mangled_name = mangle_constructor(id_expr -> get_name(), params, instance);
        id_expr -> set_callee(mangled_name);
        return instance;
    }

    /**
     * check_grouping
     * we make sure that the expression in the grouping is valid
     */
    type_instance expression_checker::check_grouping(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<grouped_expression> const & group_expr = std::static_pointer_cast<grouped_expression>(an_expression);
        std::shared_ptr<expr>& value = group_expr -> get_value();

        // we make sure the expression is not dependent on match expressions
        if(group_expr -> has_match_expression())
            throw invalid_expression(group_expr -> get_token(), "A grouped expression cannot depend on a match expression.");

        // check the grouped expressoin
        check(value, l_scope, ns_name);

        // we return the infered type instance
        return inferer::infer(an_expression, l_scope, ns_name);
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

        // we make sure the expression is not dependent on match expressions
        if(cast_expr -> has_match_expression())
            throw invalid_expression(cast_expr -> get_token(), "A cast expression cannot depend on a match expression.");

        // make sure the value to cast doesn't depend on the underscore expression
        if(value -> is_underscore_expression()) {
            throw invalid_expression(value -> expr_token(), "The underscore expression cannot be an argument to the cast operator.");
        }
        if(value -> is_call_expression()) {
            std::shared_ptr<call_expression> const & call_expr = std::static_pointer_cast<call_expression>(value);
            if(call_expr -> has_underscore()) {
                throw invalid_expression(call_expr -> get_token(), "The cast operator cannot depend on a call expression that in turn depends on the underscore expression.");
            }
        }

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
        return inferer::infer_cast(cast_fun, cast_expr, l_scope, ns_name);
    }

    /**
     * check_unary
     * we make sure that there is a function that corresponds to the unary expression
     */
    type_instance expression_checker::check_unary(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<unary_expression> const & unary_expr = std::static_pointer_cast<unary_expression>(an_expression);
        std::shared_ptr<expr>& value = unary_expr -> get_val();

        // we make sure the expression is not dependent on match expressions
        if(unary_expr -> has_match_expression())
            throw invalid_expression(unary_expr -> get_token(), "A unary expression cannot depend on a match expression.");

        // make sure the value to unary expression doesn't depend on the underscore expression
        if(value -> is_underscore_expression()) {
            throw invalid_expression(value -> expr_token(), "Unary operators cannot depend on the underscore expression.");
        }
        if(value -> is_call_expression()) {
            std::shared_ptr<call_expression> const & call_expr = std::static_pointer_cast<call_expression>(value);
            if(call_expr -> has_underscore()) {
                throw invalid_expression(call_expr -> get_token(), "Unary operators cannot depend on a call expression that in turn depends on the underscore expression.");
            }
        }

        // check the value
        check(value, l_scope, ns_name);

        // infer the type instance
        function unary_fun(star_tok);
        return inferer::infer_unary(unary_fun, unary_expr, l_scope, ns_name);
    }

    /**
     * check_binary
     * we make sure that there is a function that corresponds to the binary expression
     */
    type_instance expression_checker::check_binary(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<binary_expression> const & binary_expr = std::static_pointer_cast<binary_expression>(an_expression);
        binary_expression_type expr_type = binary_expr -> get_expression_type();

        // we make sure the expression is not dependent on match expressions
        if(binary_expr -> has_match_expression())
            throw invalid_expression(binary_expr -> get_token(), "A binary expression cannot depend on a match expression.");

        // work on operations that decay into functions
        if(
           expr_type == PLUS_EXPR           ||
           expr_type == MINUS_EXPR          ||
           expr_type == MUL_EXPR            ||
           expr_type == DIV_EXPR            ||
           expr_type == MOD_EXPR            ||
           //expr_type == POW_EXPR           ||
           expr_type == LEFT_SHIFT_EXPR     ||
           expr_type == RIGHT_SHIFT_EXPR    ||
           expr_type == LOGICAL_AND_EXPR    ||
           expr_type == LOGICAL_OR_EXPR     ||
           expr_type == BITWISE_AND_EXPR    ||
           expr_type == BITWISE_OR_EXPR     ||
           expr_type == BITWISE_XOR_EXPR    ||
           expr_type == EQUAL_EQUAL_EXPR    ||
           expr_type == NOT_EQUAL_EXPR      ||
           expr_type == LESS_EXPR           ||
           expr_type == LESS_EQUAL_EXPR     ||
           expr_type == GREATER_EXPR        ||
           expr_type == GREATER_EQUAL_EXPR

        ) {
            return check_functional_binary(expr_type, binary_expr, l_scope, ns_name);
        }
        else if(expr_type == DOT_EXPR) {
            return check_dot_binary(expr_type, binary_expr, l_scope, ns_name);
        }
        else if(expr_type == SUBSCRIPT_EXPR) {
            return check_subscript_binary(expr_type, binary_expr, l_scope, ns_name);
        }
        else {
            throw std::runtime_error("[compiler error] unexpected binary operator.");
        }
    }

    /**
     * check_functional_binary
     * we make sure that the binary expression has a function that corresponds to it
     */
    type_instance expression_checker::check_functional_binary(binary_expression_type expr_type, std::shared_ptr<binary_expression> const & binary_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<expr>& lval = binary_expr -> get_lval();
        std::shared_ptr<expr>& rval = binary_expr -> get_rval();

        // make sure the lval doesn't depend on the underscore expression
        if(lval -> is_underscore_expression()) {
            throw invalid_expression(lval -> expr_token(), "Binary operators cannot depend on the underscore expression.");
        }
        if(lval -> is_call_expression()) {
            std::shared_ptr<call_expression> const & call_expr = std::static_pointer_cast<call_expression>(lval);
            if(call_expr -> has_underscore()) {
                throw invalid_expression(call_expr -> get_token(), "Binary operators cannot depend on a call expression that in turn depends on the underscore expression.");
            }
        }

        // make sure the rval doesn't depend on the underscore expression
        if(rval -> is_underscore_expression()) {
            throw invalid_expression(rval -> expr_token(), "Binary operators cannot depend on the underscore expression.");
        }
        if(rval -> is_call_expression()) {
            std::shared_ptr<call_expression> const & call_expr = std::static_pointer_cast<call_expression>(rval);
            if(call_expr -> has_underscore()) {
                throw invalid_expression(call_expr -> get_token(), "Binary operators cannot depend on a call expression that in turn depends on the underscore expression.");
            }
        }

        // check the values
        check(lval, l_scope, ns_name);
        check(rval, l_scope, ns_name);

        // infer the type instance
        function binary_fun(star_tok);
        return inferer::infer_functional_binary(expr_type, binary_fun, binary_expr, l_scope, ns_name);
    }

    /**
     * check_dot_binary
     * we make sure that the dot expression lval refers to either a namespace or a variable expression
     */
    type_instance expression_checker::check_dot_binary(binary_expression_type expr_type, std::shared_ptr<binary_expression> const & binary_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<expr>& lval = binary_expr -> get_lval();
        std::shared_ptr<expr>& rval = binary_expr -> get_rval();
        const std::string& name = lval -> expr_token().get_lexeme();


        // make sure the lval doesn't depend on the underscore expression
        if(lval -> is_underscore_expression()) {
            throw invalid_expression(lval -> expr_token(), "Binary operators cannot depend on the underscore expression.");
        }
        if(lval -> is_call_expression()) {
            std::shared_ptr<call_expression> const & call_expr = std::static_pointer_cast<call_expression>(lval);
            if(call_expr -> has_underscore()) {
                throw invalid_expression(call_expr -> get_token(), "Binary operators cannot depend on a call expression that in turn depends on the underscore expression.");
            }
        }

        // make sure the rval doesn't depend on the underscore expression
        if(rval -> is_underscore_expression()) {
            throw invalid_expression(rval -> expr_token(), "Binary operators cannot depend on the underscore expression.");
        }
        if(rval -> is_call_expression()) {
            std::shared_ptr<call_expression> const & call_expr = std::static_pointer_cast<call_expression>(rval);
            if(call_expr -> has_underscore()) {
                throw invalid_expression(call_expr -> get_token(), "Binary operators cannot depend on a call expression that in turn depends on the underscore expression.");
            }
        }

        // if the lval is a namespace name
        if(l_scope -> has_namespace(name)) {
            return check_namespace_binary(name, rval, l_scope, ns_name);
        }
        // if the lval is a variable
        else if(l_scope -> variable_exists(ns_name, name)) {
            return check_variable_binary(lval, rval, l_scope, ns_name);
        }
        // anything else if an error
        else {
            throw invalid_expression(lval -> expr_token(), "Expected a namespace name or a variable name as lval to a dot expression.");
        }
    }

    /**
     * check_subscript_binary
     * we make sure that the lval is a variable declaration and that its content is accessible for reading
     */
    type_instance expression_checker::check_subscript_binary(binary_expression_type expr_type, std::shared_ptr<binary_expression> const & binary_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<expr>& lval = binary_expr -> get_lval();
        std::shared_ptr<expr>& rval = binary_expr -> get_rval();
        const std::string& name = lval -> expr_token().get_lexeme();

        // make sure the lval doesn't depend on the underscore expression
        if(lval -> is_underscore_expression()) {
            throw invalid_expression(lval -> expr_token(), "Binary operators cannot depend on the underscore expression.");
        }
        if(lval -> is_call_expression()) {
            std::shared_ptr<call_expression> const & call_expr = std::static_pointer_cast<call_expression>(lval);
            if(call_expr -> has_underscore()) {
                throw invalid_expression(call_expr -> get_token(), "Binary operators cannot depend on a call expression that in turn depends on the underscore expression.");
            }
        }

        // make sure the rval doesn't depend on the underscore expression
        if(rval -> is_underscore_expression()) {
            throw invalid_expression(rval -> expr_token(), "Binary operators cannot depend on the underscore expression.");
        }
        if(rval -> is_call_expression()) {
            std::shared_ptr<call_expression> const & call_expr = std::static_pointer_cast<call_expression>(rval);
            if(call_expr -> has_underscore()) {
                throw invalid_expression(call_expr -> get_token(), "Binary operators cannot depend on a call expression that in turn depends on the underscore expression.");
            }
        }

        // if the lval is an identifier expression, then it is a variable expression
        if(l_scope -> variable_exists(ns_name, name)) {
            return check_variable_subscript(lval, rval, l_scope, ns_name);
        }
        // anything else if an error
        else {
            throw invalid_expression(lval -> expr_token(), "Expected a variable name as lval to a subscript expression.");
        }
    }

    type_instance expression_checker::check_namespace_binary(const std::string& sub_ns_name, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        // if the rval is an identifier expression, then it must be either a variable expression or a default constructor expression
        if(rval -> is_identifier_expression()) {
            std::shared_ptr<identifier_expression> const & id_expr = std::static_pointer_cast<identifier_expression>(rval);
            if(l_scope -> variable_exists(sub_ns_name, id_expr -> get_name())) {
               id_expr -> set_namespace(sub_ns_name);
               return check_variable(id_expr, l_scope, ns_name);
            }
            else if(l_scope -> default_constructor_exists(sub_ns_name, id_expr -> get_name(), 0)) {
                id_expr -> set_namespace(sub_ns_name);
                return check_constructor(id_expr, l_scope, ns_name);
            }
            else {
                throw invalid_expression(id_expr -> get_token(), "Expected a variable expression or a default constructor expression after the namespace name.");
            }        
        }
        // if it is a call expression, we expect it to be either a function call or a default constructor or a record constructor
        else if(rval -> is_call_expression()) {
            std::shared_ptr<call_expression> const & call_expr = std::static_pointer_cast<call_expression>(rval);
            if(l_scope -> function_exists(sub_ns_name, call_expr -> get_name())) {
                call_expr -> set_namespace(sub_ns_name);
               return check_function_call(call_expr, l_scope, ns_name);
            }
            else if(l_scope -> default_constructor_exists(sub_ns_name, call_expr -> get_name(), call_expr -> get_arguments().size())) {
                call_expr -> set_namespace(sub_ns_name);
                return check_default_constructor(call_expr, l_scope, ns_name);
            }
            else if(l_scope -> record_constructor_exists(sub_ns_name, call_expr -> get_name(), call_expr -> get_arguments().size())) {
                call_expr -> set_namespace(sub_ns_name);
                return check_record_constructor(call_expr, l_scope, ns_name);
            }
            else {
                throw invalid_expression(call_expr -> get_token(), "Expected a function call or a record constructor.");
            }
        }
        // if the rval is a binary expression then it is either a dot expression or a subscript expression and the lval of that binary expression is a variable expression
        else if(rval -> is_binary_expression()) {
            std::shared_ptr<binary_expression> const & binary_expr = std::static_pointer_cast<binary_expression>(rval);
            binary_expression_type expr_type = binary_expr -> get_expression_type();
            std::shared_ptr<expr>& sub_lval = binary_expr -> get_lval();
            std::shared_ptr<expr>& sub_rval = binary_expr -> get_rval();

            if(sub_lval -> is_identifier_expression()) {
                std::shared_ptr<identifier_expression> const & id_expr = std::static_pointer_cast<identifier_expression>(sub_lval);
                if(l_scope -> variable_exists(sub_ns_name, id_expr -> get_name())) {
                    id_expr -> set_namespace(sub_ns_name);
                    if(expr_type == DOT_EXPR) {
                        return check_variable_attribute(sub_lval, sub_rval, l_scope, ns_name);
                    }
                    else if(expr_type == SUBSCRIPT_EXPR) {
                        return check_variable_subscript(sub_lval, sub_rval, l_scope, ns_name);
                    }
                    else {
                        throw invalid_expression(binary_expr -> get_token(), "Excepted either a dot expression or a subscript expression.");
                    }
                }
                else {
                    throw invalid_expression(id_expr -> get_token(), "Excepted the name of a variable.");
                }
            }
            else {
                throw invalid_expression(binary_expr -> get_token(), "The lval of this binary expression must be a variable name.");
            }
        }
        else {
            throw invalid_expression(rval -> expr_token(), "The rval of a dot expression after an lval namespace must be either a variable expression, a function call or a default constructor.");
        }
    }

    type_instance expression_checker::check_variable_binary(std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        // if the rval is an identifier expression, then it must be an attribute from either a record constructor expression or a named tuple expression
        if(rval -> is_identifier_expression()) {
            return check_variable_attribute(lval, rval, l_scope, ns_name);
        }
        else {
            throw invalid_expression(rval -> expr_token(), "Expected a record constructor attribute or a named tuple attribute as rval to a dot expression.");
        }
    }

    type_instance expression_checker::check_variable_attribute(std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<identifier_expression> const & id_expr = std::static_pointer_cast<identifier_expression>(lval);
        std::shared_ptr<variable>& var_decl = l_scope -> get_variable(id_expr -> get_namespace(), id_expr -> get_name());        

        // we only check to see if we have a tuple expression if we have a global variable
        if(var_decl -> is_global()) {
            std::shared_ptr<expr>& var_value = var_decl -> get_value();
            if(var_value -> is_tuple_expression()) {
                return check_tuple_attribute(var_value, rval, l_scope, ns_name);
            }
            else {
                return check_custom_attribute(lval, rval, l_scope, ns_name);
            }
        }
        // for local variables, we assume no tuples will ever be in them
        else {
            std::shared_ptr<expr>& var_value = var_decl -> get_value();
            if(var_value != nullptr && var_value -> is_tuple_expression()) {
                throw invalid_expression(var_decl -> get_token(), "Named tuples are not allowed (yet) as local variables and hence their data members cannot be accessed.");
            }
            else {
                return check_custom_attribute(lval, rval, l_scope, ns_name);
            }            
        }
    }

    type_instance expression_checker::check_tuple_attribute(std::shared_ptr<expr>& lval_val, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<tuple_expression> const & tuple_expr = std::static_pointer_cast<tuple_expression>(lval_val);
        std::vector<std::pair<std::string, std::shared_ptr<expr> > >& elements = tuple_expr -> get_elements();
        const token& rval_tok = rval -> expr_token();

        for(auto& element : elements) {
            if(element.first == rval_tok.get_lexeme())
                return check(element.second, l_scope, ns_name);
        }

        throw invalid_expression(rval_tok, "This attribute does not exist in the named tuple contained in the variable being accessed.");
    }

    type_instance expression_checker::check_custom_attribute(std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        // we check the lval
        check(lval, l_scope, ns_name);

        // we build a function call expression for __getattr__ that will be validated
        token rval_tok = rval -> expr_token();
        std::string call_name = "__getattr_" + rval_tok.get_lexeme() + "__";
        token call_tok(rval_tok.get_type(), call_name, rval_tok.get_line(), rval_tok.get_column(), rval_tok.get_source_path());
        std::shared_ptr<call_expression> getattr_expr = std::make_shared<call_expression>(call_tok);
        // argument is a variable expression
        getattr_expr -> add_argument(star_tok, lval);

        return check_function_call(getattr_expr, l_scope, ns_name);
    }

    type_instance expression_checker::check_variable_subscript(std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<identifier_expression> const & id_expr = std::static_pointer_cast<identifier_expression>(lval);
        std::shared_ptr<variable>& var_decl = l_scope -> get_variable(id_expr -> get_namespace(), id_expr -> get_name());
        type_instance& var_instance = var_decl -> get_type_instance();

        if(var_instance.get_category() == TUPLE) {
            return check_tuple_subscript(var_instance, rval, l_scope, ns_name);
        }
        else if(var_instance.get_category() == LIST) {
            return check_list_subscript(var_instance, rval, l_scope, ns_name);
        }
        else if(var_instance.get_category() == MAP) {
            return check_map_subscript(var_instance, rval, l_scope, ns_name);
        }
        else {
            return check_custom_subscript(lval, rval, l_scope, ns_name);
        }
    }

    type_instance expression_checker::check_tuple_subscript(type_instance& var_instance, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::vector<type_instance>& params = var_instance.get_params();
        const token& rval_tok = rval -> expr_token();
        std::size_t key = 0;

        // make sure the rval is a literal integer expression
        if(rval -> is_literal_expression() == false) {
            throw invalid_expression(rval_tok, "Expected an integer as key to access the tuple.");
        }
        else {
            std::shared_ptr<literal_expression> const & key_expr = std::static_pointer_cast<literal_expression>(rval);
            if(key_expr -> get_expression_type() == INTEGER_EXPR) {
                std::sscanf(rval_tok.get_lexeme().c_str(), "%zu", &key);
            }
            else {
                throw invalid_expression(rval_tok, "Expected an integer as key to access the tuple.");
            }
        }

        // make sure the key exists
        if(key >= params.size()) {
            throw invalid_expression(rval_tok, "This key is out of range within the tuple contained in the variable.");
        }

        // return the type instance
        return inferer::infer_tuple_subscript(var_instance, rval, l_scope, ns_name);
    }

    type_instance expression_checker::check_list_subscript(type_instance& var_instance, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        const token& rval_tok = rval -> expr_token();

        // make sure the rval is a literal integer expression
        if(rval -> is_literal_expression() == false) {
            throw invalid_expression(rval_tok, "Expected an integer as key to access the list.");
        }
        else {
            std::shared_ptr<literal_expression> const & key_expr = std::static_pointer_cast<literal_expression>(rval);
            if(key_expr -> get_expression_type() != INTEGER_EXPR) {
                throw invalid_expression(rval_tok, "Expected an integer as key to access the list.");
            }
        }

        // return the type instance
        return inferer::infer_list_subscript(var_instance, rval, l_scope, ns_name);
    }

    type_instance expression_checker::check_map_subscript(type_instance& var_instance, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::vector<type_instance>& instance_params = var_instance.get_params();
        type_instance& key_instance = instance_params[0];
        type_instance& value_instance = instance_params[1];
        const token& rval_tok = rval -> expr_token();

        // make sure the rval has the expected type of the key
        type_instance rval_instance = check(rval, l_scope, ns_name);
        if(type_instance_strong_compare(rval_instance, key_instance) == false) {
            throw invalid_expression(rval_tok, "Expected the index to be of type instance <" + mangle_type_instance(key_instance) + "> but it is instead of type instance <" + mangle_type_instance(rval_instance) + ">.");
        }
        
        // return the type instance
        return value_instance;
    }

    type_instance expression_checker::check_custom_subscript(std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        // we check the lval and rval
        check(lval, l_scope, ns_name);
        check(rval, l_scope, ns_name);

        // we build a function call expression for __getitem__ that will be validated
        const token& rval_tok = rval -> expr_token();
        std::string call_name = "__getitem_" + rval_tok.get_lexeme() + "__";
        token call_tok(rval_tok.get_type(), call_name, rval_tok.get_line(), rval_tok.get_column(), rval_tok.get_source_path());
        std::shared_ptr<call_expression> getitem_expr = std::make_shared<call_expression>(call_tok);
        // argument is a variable expression
        getitem_expr -> add_argument(star_tok, lval);
        
        return check_function_call(getitem_expr, l_scope, ns_name);
    }

    /**
     * check_match
     * we make sure that the rval of the match expression is a literal or a constructor expression
     */
    type_instance expression_checker::check_match(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        m_inside_match = true;

        std::shared_ptr<match_expression> const & match_expr = std::static_pointer_cast<match_expression>(an_expression);
        std::shared_ptr<expr>& lval = match_expr -> get_lval();
        std::shared_ptr<expr>& rval = match_expr -> get_rval();

        // we make sure that the rval is an underscore expression, a literal expression expression or a constructor expression
        if(rval -> is_underscore_expression() || rval -> is_literal_expression() || rval -> is_identifier_expression() || rval -> is_call_expression() || rval -> is_binary_expression()) {
            if(rval -> is_identifier_expression()) {
                if(l_scope -> default_constructor_exists(ns_name, rval -> expr_token().get_lexeme(), 0) == false) {
                    throw invalid_expression(rval -> expr_token(), "Expected a default constructor.");
                }
            }
            else if(rval -> is_call_expression()) {
                if(l_scope -> function_exists(ns_name, rval -> expr_token().get_lexeme()) == true) {
                    throw invalid_expression(rval -> expr_token(), "A function call cannot be used as rval to a match expression");
                }
            }
            else if(rval -> is_binary_expression()) {
                std::shared_ptr<binary_expression> const & bin_expr = std::static_pointer_cast<binary_expression>(rval);
                std::shared_ptr<expr>& bin_lval = bin_expr -> get_lval();
                std::shared_ptr<expr>& bin_rval = bin_expr -> get_rval();
                // validate the lval - we expect a namespace name
                if(bin_lval -> is_identifier_expression()) {
                    // we have a namespace, we make sure it is followed by a constructor
                    const std::string& sub_ns_name = bin_lval -> expr_token().get_lexeme();
                    if(l_scope -> has_namespace(sub_ns_name)) {
                        if(bin_rval -> is_identifier_expression()) {
                            if(l_scope -> default_constructor_exists(ns_name, rval -> expr_token().get_lexeme(), 0) == false) {
                                throw invalid_expression(rval -> expr_token(), "Expected a default constructor.");
                            }
                        }
                        else if(bin_rval -> is_call_expression()) {
                            if(l_scope -> function_exists(sub_ns_name, bin_rval -> expr_token().get_lexeme()) == true) {
                                throw invalid_expression(bin_rval -> expr_token(), "A function call cannot be used as rval to a match expression");
                            }
                        }
                        else {
                            throw invalid_expression(bin_rval -> expr_token(), "Expected a constructor expression after namespace name in binary expression used in a match expression.");
                        }
                    }
                    else {
                        throw invalid_expression(bin_lval -> expr_token(), "Expected a namespace name before constructor in match expression.");
                    }
                }
                else {
                    throw invalid_expression(bin_lval -> expr_token(), "Expected a namespace name before constructor in match expression.");
                }
            }
        }
        // anything else that is none of the above is an error
        else {
            throw invalid_expression(rval -> expr_token(), "Expected the underscore expression or a literal expression or constructor expression as rval to a match expression.");
        }

        // we check both the lval and the rval
        type_instance lval_instance = check(lval, l_scope, ns_name);
        type_instance rval_instance = check(rval, l_scope, ns_name);

        // we make sure both the rval and lval type instance are the same
        if(type_instance_weak_compare(lval_instance, rval_instance) == false) {
            throw invalid_expression(lval -> expr_token(), "This lval expression has type instance <" + mangle_type_instance(lval_instance) + "> while the rval expression expression has type instance <" + mangle_type_instance(rval_instance) + ">. Both type instances but be the same - though not necessarily equal.");
        }

        m_inside_match = false;

        return inferer::infer(an_expression, l_scope, ns_name);
    }

    /**
     * check_assignment
     * we make sure that the expression on the right side is valid
     * and make sure that it's type is compatible with that of the variable on the left side
     */
    type_instance expression_checker::check_assignment(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<assignment_expression> const & assign_expr = std::static_pointer_cast<assignment_expression>(an_expression);
        std::shared_ptr<expr>& lval = assign_expr -> get_lval();
        std::shared_ptr<expr>& rval = assign_expr -> get_rval();
        const std::string& lval_name = lval -> expr_token().get_lexeme();

        // we make sure the expression is not dependent on match expressions
        if(assign_expr -> has_match_expression())
            throw invalid_expression(assign_expr -> get_token(), "An assignment expression cannot depend on a match expression.");

        // we make sure the lval is a variable expression
        if(lval -> is_identifier_expression()) {
            if(l_scope -> variable_exists(ns_name, lval_name) == false) {
                throw invalid_expression(lval -> expr_token(), "The lval of an assignment expression must be a variable.");
            }
            else {
                std::shared_ptr<variable>& var_decl = l_scope -> get_variable(ns_name, lval_name);
                if(var_decl -> is_mutable() == false)
                    throw invalid_expression(lval -> expr_token(), "The variable <" + mangle_variable(* var_decl) + "> is immutable hence is not a valid lval to an assignment.");
            }
        }
        else if(lval -> is_binary_expression()) {
            std::shared_ptr<binary_expression> const & bin_expr = std::static_pointer_cast<binary_expression>(lval);
            std::shared_ptr<expr>& bin_lval = bin_expr -> get_lval();
            std::shared_ptr<expr>& bin_rval = bin_expr -> get_rval();
            // validate the lval - we expect a namespace name
            if(bin_lval -> is_identifier_expression()) {
                // we have a namespace, we make sure it is followed by a constructor
                const std::string& sub_ns_name = bin_lval -> expr_token().get_lexeme();
                if(l_scope -> has_namespace(sub_ns_name)) {
                    if(bin_rval -> is_identifier_expression()) {
                        if(l_scope -> variable_exists(sub_ns_name, bin_rval -> expr_token().get_lexeme()) == false) {
                            throw invalid_expression(bin_rval -> expr_token(), "The lval of an assignment expression must be a variable.");
                        }
                        else {
                            std::shared_ptr<variable>& var_decl = l_scope -> get_variable(sub_ns_name, bin_rval -> expr_token().get_lexeme());
                            if(var_decl -> is_mutable() == false)
                                throw invalid_expression(bin_rval -> expr_token(), "The variable <" + mangle_variable(* var_decl) + "> is immutable hence is not a valid lval to an assignment.");
                        }
                    }
                    else {
                        throw invalid_expression(bin_rval -> expr_token(), "The lval of an assignment expression must be a variable.");
                    }
                }
                else {
                    throw invalid_expression(bin_lval -> expr_token(), "Expected a namespace name before variable in assignment lval.");
                }
            }
            else {
                throw invalid_expression(bin_lval -> expr_token(), "Expected a namespace name before variable in assignment lval.");
            }
        }

        // we check both the lval and the rval
        type_instance lval_instance = check(lval, l_scope, ns_name);
        type_instance rval_instance = check(rval, l_scope, ns_name);

        // we make sure both the rval and lval type instance are equal
        if(type_instance_strong_compare(lval_instance, rval_instance) == false) {
            throw invalid_expression(lval -> expr_token(), "This lval expression has type instance <" + mangle_type_instance(lval_instance) + "> while the rval expression expression has type instance <" + mangle_type_instance(rval_instance) + ">. Both type instances but be equal.");
        }

        return lval_instance;
    }
}
