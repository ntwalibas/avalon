#include <stdexcept>
#include <utility>
#include <memory>
#include <string>
#include <map>

/* Expressions */
#include "representer/hir/ast/expr/underscore_expression.hpp"
#include "representer/hir/ast/expr/identifier_expression.hpp"
#include "representer/hir/ast/expr/literal_expression.hpp"
#include "representer/hir/ast/expr/tuple_expression.hpp"
#include "representer/hir/ast/expr/list_expression.hpp"
#include "representer/hir/ast/expr/call_expression.hpp"
#include "representer/hir/ast/expr/map_expression.hpp"
#include "representer/hir/ast/expr/expr.hpp"

/* Checker */
#include "checker/decl/function/function_checker.hpp"
#include "checker/decl/type/type_checker.hpp"

/* Symbol table */
#include "representer/hir/symtable/scope.hpp"

/* AST */
#include "representer/hir/ast/decl/function.hpp"
#include "representer/hir/ast/decl/type.hpp"

/* Generator */
#include "checker/decl/function/function_generator.hpp"

/* Inferer */
#include "inferer/inferer.hpp"

/* Lexer */
#include "lexer/token.hpp"

/* Exceptions */
#include "representer/exceptions/symbol_can_collide.hpp"
#include "representer/exceptions/symbol_not_found.hpp"
#include "checker/exceptions/invalid_expression.hpp"
#include "checker/exceptions/invalid_function.hpp"
#include "checker/exceptions/invalid_type.hpp"


namespace avalon {
    static void build_type_instance(type_instance& dest_instance, type_instance cons_instance, type_instance expr_instance, token err_tok) {
        // if the type instance from the constructor is abstract,
        // then it is a type instance built from a type parameter (done in the inferer below)
        // we search for it on the destination type instance to perform a replacement with the type instance from the expression
        if(cons_instance.is_abstract() == true) {
            std::vector<type_instance>& dest_params = dest_instance.get_params();
            // we try to find the constructor instance amond the type parameters given to us on the destination instance
            for(auto& dest_param : dest_params) {
                // if we find the constructor type instance, we perform the replacement
                if(dest_param.get_token() == cons_instance.get_token()) {
                    // we replace the token
                    const token& expr_tok = expr_instance.get_token();
                    dest_param.set_token(const_cast<token&>(expr_tok));

                    // we replace the tyoe
                    std::shared_ptr<type>& expr_type = expr_instance.get_type();                    
                    dest_param.set_type(expr_type);

                    // we replace the namespace
                    const std::string& ns_name = expr_instance.get_namespace();
                    dest_param.set_namespace(ns_name);

                    // if the expression type instance is parametrized, so will this one
                    if(expr_instance.is_complete() == false)
                        dest_param.is_parametrized(true);

                    // we set parameters
                    std::vector<type_instance>& expr_params = expr_instance.get_params();
                    for(auto& expr_param : expr_params)
                        dest_param.add_param(expr_param);
                }
                // if we cannot find the constructor type instance, we try to see if the replacement was not done already                
                else {
                    // if it was already done, we make sure the new type instance (it exists on the expression) is equal to the existing one
                    if(cons_instance.get_token() == dest_param.get_old_token()) {
                        if(type_instance_weak_compare(dest_param, expr_instance) == false)
                            throw invalid_expression(err_tok, "This expression is not of the expected type instance");
                    }
                }
            }
        }
        // if it has a type, then we compare it with the type instance from the expression
        else {
            // if the type instance of the expression and that of the contructor parameter are different, no point continuing
            if(type_instance_weak_compare(cons_instance, expr_instance) == false) {
                throw invalid_expression(err_tok, "This expression has type instance <" + mangle_type_instance(expr_instance) + "> while the expected type instance is <" + mangle_type_instance(cons_instance) + ">.");
            }
            // if the type instance from the constructor compares weakly from the type instance from the expression,
            // we check to see if the type instance from the expression is a concrete type before proceeding
            else {
                // if the type instance from the constructor is concrete but the one from the expression is not, we have an error
                // WHY? I wrote this code in an earlier version but can't seem to recall why I required this
                if(cons_instance.is_abstract() == false && expr_instance.is_abstract() == true) {
                    throw invalid_expression(err_tok, "This expression has type instance <" + mangle_type_instance(expr_instance) + "> while the expected type instance is <" + mangle_type_instance(cons_instance) + ">.");
                }
                // if on the other hand both the type instance from the constructor and the one from the expression are concrete,
                // then we look into their arguments for potential type parameters
                else {
                    std::vector<type_instance>& cons_params = cons_instance.get_params();
                    std::vector<type_instance>& expr_params = expr_instance.get_params();

                    auto cons_it = cons_params.begin(), cons_end = cons_params.end();
                    auto expr_it = expr_params.begin(), expr_end = expr_params.end();
                    for(; cons_it != cons_end && expr_it != expr_end; ++cons_it, ++expr_it)
                        build_type_instance(dest_instance, * cons_it, * expr_it, err_tok);
                }
            }
        }
    }

    /**
     * infer
     * given an expression, this function infers the type instance of said expression and returns it
     */
    type_instance inferer::infer(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
        if(an_expression -> is_underscore_expression()) {
            return inferer::infer_underscore(an_expression);
        }
        else if(an_expression -> is_literal_expression()) {
            return inferer::infer_literal(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_tuple_expression()) {
            return inferer::infer_tuple(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_list_expression()) {
            return inferer::infer_list(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_map_expression()) {
            return inferer::infer_map(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_call_expression()) {
            return inferer::infer_call(an_expression, l_scope, ns_name, sub_ns_name);
        }
        else if(an_expression -> is_identifier_expression()) {
            return inferer::infer_identifier(an_expression, l_scope, ns_name, sub_ns_name);
        }
        else {
            throw std::runtime_error("[compiler error] unexpected expression type in inference engine.");
        }
    }

    /**
     * infer_underscore
     * infers the type instance of an underscopre expression, which is just a type instance without any type
     */
    type_instance inferer::infer_underscore(std::shared_ptr<expr>& an_expression) {
        std::shared_ptr<underscore_expression> const & und_expr = std::static_pointer_cast<underscore_expression>(an_expression);
        und_expr -> set_type_instance(star_instance);
        return star_instance;
    }

    /**
     * infer_literal
     * infers the type instance of a literal
     */
    type_instance inferer::infer_literal(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name) {
        std::shared_ptr<literal_expression> const & lit_expr = std::static_pointer_cast<literal_expression>(an_expression);

        // if the expression already has a type instance set, we return that
        if(lit_expr -> type_instance_from_parser() == false && lit_expr -> has_type_instance() == true) {
            return lit_expr -> get_type_instance();
        }

        // if on the other hand a type instance from the parser was posted, we type check check it
        type_instance parser_type_instance;
        bool has_parser_type_instance = false;
        if(lit_expr -> type_instance_from_parser() == true) {
            has_parser_type_instance = true;
            parser_type_instance = lit_expr -> get_type_instance();

            try {
                // we typecheck the parser type instance
                std::pair<bool,bool> res = type_instance_checker::complex_check(parser_type_instance, l_scope, ns_name);
                // we don't allow parametrized type instances to be bound to expressions
                if (res.second == true) {
                    throw invalid_expression(parser_type_instance.get_token(), "Parametrized types cannot be used on expressions.");
                }
            } catch(invalid_type err) {
                throw invalid_expression(err.get_token(), err.what());
            }
        }
        
        type_instance infered_type_instance;
        if(lit_expr -> get_expression_type() == INTEGER_EXPR) {
            std::shared_ptr<type>& int_type = l_scope -> get_type("*", "int", 0);
            type_instance int_type_instance(int_type_tok, int_type, "*");
            infered_type_instance = int_type_instance;
        }
        else if(lit_expr -> get_expression_type() == DECIMAL_EXPR) {
            std::shared_ptr<type>& dec_type = l_scope -> get_type("*", "dec", 0);
            type_instance dec_type_instance(dec_type_tok, dec_type, "*");
            infered_type_instance = dec_type_instance;
        }
        else if(lit_expr -> get_expression_type() == FLOATING_POINT_EXPR) {
            std::shared_ptr<type>& float_type = l_scope -> get_type("*", "float", 0);
            type_instance float_type_instance(float_type_tok, float_type, "*");
            infered_type_instance = float_type_instance;
        }
        else if(lit_expr -> get_expression_type() == STRING_EXPR) {
            std::shared_ptr<type>& string_type = l_scope -> get_type("*", "string", 0);
            type_instance string_type_instance(string_type_tok, string_type, "*");            
            infered_type_instance = string_type_instance;
        }
        else {
            throw std::runtime_error("[compiler error] unexpected literal expression in inference engine.");
        }

        // typecheck the infered type instance
        try {
            type_instance_checker::complex_check(infered_type_instance, l_scope, ns_name);
        } catch(invalid_type err) {
            throw invalid_expression(err.get_token(), err.what());
        }

        // if we have type instance from the parser we compare it with the infered type
        // and if they equal, we keep the parser type instance
        if(has_parser_type_instance) {
            if(type_instance_weak_compare(parser_type_instance, infered_type_instance) == false) {
                throw invalid_expression(parser_type_instance.get_token(), "The type instance supplied along the expression: <" + mangle_type_instance(parser_type_instance) + "> is not the same as the one deduced by the inference engine: <" + mangle_type_instance(infered_type_instance) + ">.");
            }
            else {
                return parser_type_instance;
            }
        }
        else {
            lit_expr -> set_type_instance(infered_type_instance, false);
            return infered_type_instance;
        }
    }

    /**
     * infer_tuple
     * infers the type instance of a tuple
     */
    type_instance inferer::infer_tuple(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name) {
        std::shared_ptr<tuple_expression> const & tup_expr = std::static_pointer_cast<tuple_expression>(an_expression);
        
        // if the expression already has a type instance set, we return that
        if(tup_expr -> type_instance_from_parser() == false && tup_expr -> has_type_instance() == true) {
            return tup_expr -> get_type_instance();
        }

        // if on the other hand a type instance from the parser was posted, we type check check it
        type_instance parser_type_instance;
        bool has_parser_type_instance = false;
        if(tup_expr -> type_instance_from_parser() == true) {
            has_parser_type_instance = true;
            parser_type_instance = tup_expr -> get_type_instance();

            try {
                // we typecheck the parser type instance
                std::pair<bool,bool> res = type_instance_checker::complex_check(parser_type_instance, l_scope, ns_name);
                // we don't allow parametrized type instances to be bound to expressions
                if (res.second == true) {
                    throw invalid_expression(parser_type_instance.get_token(), "Parametrized types cannot be used on expressions.");
                }
            } catch(invalid_type err) {
                throw invalid_expression(err.get_token(), err.what());
            }
        }

        // we create a type and type instance out of the expression dynamically
        token tok = tup_expr -> get_token();
        std::shared_ptr<type> tup_type = std::make_shared<type>(tok, VALID);
        type_instance infered_type_instance(tok, tup_type, "*");
        infered_type_instance.set_category(TUPLE);

        // we fill in the type instance parameters
        std::vector<std::pair<std::string, std::shared_ptr<expr> > >& elements = tup_expr -> get_elements();
        for(auto& element : elements) {
            type_instance el_instance = inferer::infer(element.second, l_scope, ns_name, ns_name);
            infered_type_instance.add_param(el_instance);
        }

        // typecheck the infered type instance
        try {
            type_instance_checker::complex_check(infered_type_instance, l_scope, ns_name);
        } catch(invalid_type err) {
            throw invalid_expression(err.get_token(), err.what());
        }

        // if we have type instance from the parser we compare it with the infered type
        // and if they equal, we keep the parser type instance
        if(has_parser_type_instance) {
            if(type_instance_weak_compare(parser_type_instance, infered_type_instance) == false) {
                throw invalid_expression(parser_type_instance.get_token(), "The type instance supplied along the expression: <" + mangle_type_instance(parser_type_instance) + "> is not the same as the one deduced by the inference engine: <" + mangle_type_instance(infered_type_instance) + ">.");
            }
            else {
                return parser_type_instance;
            }
        }
        else {
            tup_expr -> set_type_instance(infered_type_instance, false);
            return infered_type_instance;
        }
    }

    /**
     * infer_list
     * infers the type instance of a list
     */
    type_instance inferer::infer_list(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name) {
        std::shared_ptr<list_expression> const & list_expr = std::static_pointer_cast<list_expression>(an_expression);
        
        // if the expression already has a type instance set, we return that
        if(list_expr -> type_instance_from_parser() == false && list_expr -> has_type_instance() == true) {
            return list_expr -> get_type_instance();
        }

        // if on the other hand a type instance from the parser was posted, we type check check it
        type_instance parser_type_instance;
        bool has_parser_type_instance = false;
        if(list_expr -> type_instance_from_parser() == true) {
            has_parser_type_instance = true;
            parser_type_instance = list_expr -> get_type_instance();

            // make sure the type instance is of type LIST
            if(parser_type_instance.get_category() != LIST) {
                throw invalid_expression(parser_type_instance.get_token(), "Excepted a list type instance to be attached to a list expression.");
            }

            try {
                // we typecheck the parser type instance
                std::pair<bool,bool> res = type_instance_checker::complex_check(parser_type_instance, l_scope, ns_name);
                // we don't allow parametrized type instances to be bound to expressions
                if (res.second == true) {
                    throw invalid_expression(parser_type_instance.get_token(), "Parametrized types cannot be used on expressions.");
                }
            } catch(invalid_type err) {
                throw invalid_expression(err.get_token(), err.what());
            }
        }

        // if the list is empty, we return the type instance on the list
        // it can be a parser provided one or the generic type instance (signaling a lack of user provided type instance)
        std::vector<std::shared_ptr<expr> >& elements = list_expr -> get_elements();
        if(elements.size() == 0) {
            return list_expr -> get_type_instance();
        }

        // if we do have at least one element in the list, then its type instance is that of the first element
        // we leave it to the check to make sure other elements have the same type instance as the first one
        std::shared_ptr<expr>& first_element = elements[0];
        type_instance first_element_instance = inferer::infer(first_element, l_scope, ns_name, ns_name);

        // we build the list type instance
        token tok = list_expr -> get_token();
        std::shared_ptr<type> list_type = std::make_shared<type>(tok, VALID);
        type_instance infered_type_instance(tok, list_type, "*");
        infered_type_instance.set_category(LIST);
        infered_type_instance.add_param(first_element_instance);

        // typecheck the infered type instance
        try {
            type_instance_checker::complex_check(infered_type_instance, l_scope, ns_name);
        } catch(invalid_type err) {
            throw invalid_expression(err.get_token(), err.what());
        }

        // if we have type instance from the parser we compare it with the infered type
        // and if they equal, we keep the parser type instance
        if(has_parser_type_instance) {
            if(type_instance_weak_compare(parser_type_instance, infered_type_instance) == false) {
                throw invalid_expression(parser_type_instance.get_token(), "The type instance supplied along the expression: <" + mangle_type_instance(parser_type_instance) + "> is not the same as the one deduced by the inference engine: <" + mangle_type_instance(infered_type_instance) + ">.");
            }
            else {
                return parser_type_instance;
            }
        }
        else {
            list_expr -> set_type_instance(infered_type_instance, false);
            return infered_type_instance;
        }
    }

    /**
     * infer_map
     * infers the type instance of a map
     */
    type_instance inferer::infer_map(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name) {
        std::shared_ptr<map_expression> const & map_expr = std::static_pointer_cast<map_expression>(an_expression);
        
        // if the expression already has a type instance set, we return that
        if(map_expr -> type_instance_from_parser() == false && map_expr -> has_type_instance() == true) {
            return map_expr -> get_type_instance();
        }

        // if on the other hand a type instance from the parser was posted, we type check it
        type_instance parser_type_instance;
        bool has_parser_type_instance = false;
        if(map_expr -> type_instance_from_parser() == true) {
            has_parser_type_instance = true;
            parser_type_instance = map_expr -> get_type_instance();

            // make sure the type instance is of type MAP
            if(parser_type_instance.get_category() != MAP) {
                throw invalid_expression(parser_type_instance.get_token(), "Excepted a map type instance to be attached to a map expression.");
            }

            try {
                // we typecheck the parser type instance
                std::pair<bool,bool> res = type_instance_checker::complex_check(parser_type_instance, l_scope, ns_name);
                // we don't allow parametrized type instances to be bound to expressions
                if (res.second == true) {
                    throw invalid_expression(parser_type_instance.get_token(), "Parametrized types cannot be used on expressions.");
                }
            } catch(invalid_type err) {
                throw invalid_expression(err.get_token(), err.what());
            }
        }

        // if the list is empty, we return the type instance on the list
        // it can be a parser provided one or the generic type instance (signaling a lack of user provided type instance)
        std::vector<std::pair<std::shared_ptr<expr>, std::shared_ptr<expr> > >& elements = map_expr -> get_elements();
        if(elements.size() == 0) {
            return map_expr -> get_type_instance();
        }

        // if we do have at least one element in the list, then its type instance is that of the first element
        // we leave it to the check to make sure other elements have the same type instance as the first one
        std::pair<std::shared_ptr<expr>, std::shared_ptr<expr> >& first_element = elements[0];
        type_instance first_element_key_instance = inferer::infer(first_element.first, l_scope, ns_name, ns_name);
        type_instance first_element_value_instance = inferer::infer(first_element.second, l_scope, ns_name, ns_name);

        // we build the list type instance
        token tok = map_expr -> get_token();
        std::shared_ptr<type> map_type = std::make_shared<type>(tok, VALID);
        type_instance infered_type_instance(tok, map_type, "*");
        infered_type_instance.set_category(MAP);
        infered_type_instance.add_param(first_element_key_instance);
        infered_type_instance.add_param(first_element_value_instance);

        // typecheck the infered type instance
        try {
            type_instance_checker::complex_check(infered_type_instance, l_scope, ns_name);
        } catch(invalid_type err) {
            throw invalid_expression(err.get_token(), err.what());
        }

        // if we have type instance from the parser we compare it with the infered type
        // and if they equal, we keep the parser type instance
        if(has_parser_type_instance) {
            if(type_instance_weak_compare(parser_type_instance, infered_type_instance) == false) {
                throw invalid_expression(parser_type_instance.get_token(), "The type instance supplied along the expression: <" + mangle_type_instance(parser_type_instance) + "> is not the same as the one deduced by the inference engine: <" + mangle_type_instance(infered_type_instance) + ">.");
            }
            else {
                return parser_type_instance;
            }
        }
        else {
            map_expr -> set_type_instance(infered_type_instance, false);
            return infered_type_instance;
        }
    }

    /**
     * infer_call
     * infers the type instance of a call expression
     */
    type_instance inferer::infer_call(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
        std::shared_ptr<call_expression> const & call_expr = std::static_pointer_cast<call_expression>(an_expression);
        
        if(call_expr -> get_expression_type() == DEFAULT_CONSTRUCTOR_EXPR) {
            return inferer::infer_default_constructor(call_expr, l_scope, ns_name, sub_ns_name);
        }
        else if(call_expr -> get_expression_type() == RECORD_CONSTRUCTOR_EXPR) {
            return inferer::infer_record_constructor(call_expr, l_scope, ns_name, sub_ns_name);
        }
        else {
            function new_fun(star_tok);
            return inferer::infer_function_call(new_fun, call_expr, l_scope, ns_name, sub_ns_name);
        }
    }

    /**
     * infer_default_constructor
     * infers the type instance of a default constructor expression
     */
    type_instance inferer::infer_default_constructor(std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
        // if the expression already has a type instance, we return it
        if(call_expr -> type_instance_from_parser() == false && call_expr -> has_type_instance() == true) {
            return call_expr -> get_type_instance();
        }

        // if on the other hand a type instance from the parser was posted, we type check check it
        type_instance parser_type_instance;
        bool has_parser_type_instance = false;
        if(call_expr -> type_instance_from_parser() == true) {
            has_parser_type_instance = true;
            parser_type_instance = call_expr -> get_type_instance();

            try {
                // we typecheck the parser type instance
                std::pair<bool,bool> res = type_instance_checker::complex_check(parser_type_instance, l_scope, ns_name);
                // we don't allow parametrized type instances to be bound to expressions
                if (res.second == true) {
                    throw invalid_expression(parser_type_instance.get_token(), "Parametrized types cannot be used on expressions.");
                }
            } catch(invalid_type err) {
                throw invalid_expression(err.get_token(), err.what());
            }
        }

        std::vector<std::pair<token, std::shared_ptr<expr> > >& cons_args = call_expr -> get_arguments();
        default_constructor& cons = l_scope -> get_default_constructor(sub_ns_name, call_expr -> get_name(), cons_args.size());
        std::vector<type_instance>& cons_params = cons.get_params();
        std::shared_ptr<type>& cons_type = cons.get_type();
        const std::vector<token>& type_params = cons_type -> get_params();
        const token& type_token = cons_type -> get_token();

        // we create the type instance for which we are going to fill parameters
        type_instance infered_type_instance(const_cast<token&>(type_token), cons_type, sub_ns_name);
        // at this point, all of this type instance parameters will be abstract types so the instance should be parametrized
        // but since we are going to be replacing those parameters (or some anyway) with other types, it is easier to go from not parametrized to parametrized
        // the logic is this: we start with not parametrized. the moment we encounter a parametrized type, we set this type instance to parametrized
        // doing this like this is O(1) while doing the reverse would be at best O(n)
        infered_type_instance.is_parametrized(false);
        for(auto& type_param : type_params) {
            type_instance instance_param(const_cast<token&>(type_param), "*");
            instance_param.is_parametrized(true);
            infered_type_instance.add_param(instance_param);
        }

        // we have a type instance with abstract types from the type builder
        // now we need to replace them with actual types infered from the constructor expression
        auto param_it = cons_params.begin(), param_end = cons_params.end();
        auto arg_it = cons_args.begin(), arg_end = cons_args.end();
        for(; param_it != param_end && arg_it != arg_end; ++param_it, ++arg_it) {
            if(arg_it -> second -> is_underscore_expression()) {
                infered_type_instance.is_parametrized(true);
                continue;
            }
            else {
                type_instance arg_type_instance = inferer::infer(arg_it -> second, l_scope, ns_name, sub_ns_name);
                build_type_instance(infered_type_instance, * param_it, arg_type_instance, call_expr -> get_token());
            }
        }

        // typecheck the infered type instance
        try {
            type_instance_checker::complex_check(infered_type_instance, l_scope, ns_name);
        } catch(invalid_type err) {
            throw invalid_expression(err.get_token(), err.what());
        }

        // if we have type instance from the parser we compare it with the infered type
        // and if they equal, we keep the parser type instance
        if(has_parser_type_instance) {
            if(type_instance_weak_compare(parser_type_instance, infered_type_instance) == false) {
                throw invalid_expression(parser_type_instance.get_token(), "The type instance supplied along the expression: <" + mangle_type_instance(parser_type_instance) + "> is not the same as the one deduced by the inference engine: <" + mangle_type_instance(infered_type_instance) + ">.");
            }
            else {
                return parser_type_instance;
            }
        }
        else {
            call_expr -> set_type_instance(infered_type_instance, false);
            return infered_type_instance;
        }
    }

    /**
     * infer_record_constructor
     * infers the type instance of a record constructor expression
     */
    type_instance inferer::infer_record_constructor(std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
        // if the expression already has a type instance, we return it
        if(call_expr -> type_instance_from_parser() == false && call_expr -> has_type_instance() == true) {
            return call_expr -> get_type_instance();
        }

        // if on the other hand a type instance from the parser was posted, we type check check it
        type_instance parser_type_instance;
        bool has_parser_type_instance = false;
        if(call_expr -> type_instance_from_parser() == true) {
            has_parser_type_instance = true;
            parser_type_instance = call_expr -> get_type_instance();

            try {
                // we typecheck the parser type instance
                std::pair<bool,bool> res = type_instance_checker::complex_check(parser_type_instance, l_scope, ns_name);
                // we don't allow parametrized type instances to be bound to expressions
                if (res.second == true) {
                    throw invalid_expression(parser_type_instance.get_token(), "Parametrized types cannot be used on expressions.");
                }
            } catch(invalid_type err) {
                throw invalid_expression(err.get_token(), err.what());
            }
        }

        std::vector<std::pair<token, std::shared_ptr<expr> > >& cons_args = call_expr -> get_arguments();
        record_constructor& cons = l_scope -> get_record_constructor(sub_ns_name, call_expr -> get_name(), cons_args.size());
        std::map<token, type_instance>& cons_params = cons.get_params();
        std::shared_ptr<type>& cons_type = cons.get_type();
        const std::vector<token>& type_params = cons_type -> get_params();
        const token& type_token = cons_type -> get_token();

        // we create the type instance for which we are going to fill parameters
        type_instance infered_type_instance(const_cast<token&>(type_token), cons_type, sub_ns_name);
        // at this point, all of this type instance parameters will be abstract types so the instance should be parametrized
        // but since we are going to be replacing those parameters (or some anyway) with other types, it is easier to go from not parametrized to parametrized
        // the logic is this: we start with not parametrized. the moment we encounter a parametrized type, we set this type instance to parametrized
        // doing this like this is O(1) while doing the reverse would be at best O(n)
        infered_type_instance.is_parametrized(false);
        for(auto& type_param : type_params) {
            type_instance instance_param(const_cast<token&>(type_param), "*");
            instance_param.is_parametrized(true);
            infered_type_instance.add_param(instance_param);
        }

        // we have a type instance with abstract types from the type builder
        // now we need to replace them with actual types infered from the constructor expression
        for(auto& cons_arg : cons_args) {
            type_instance& cons_param = cons_params[cons_arg.first];
            if(cons_arg.second -> is_underscore_expression()) {
                infered_type_instance.is_parametrized(true);
                continue;
            }
            else {
                type_instance arg_type_instance = inferer::infer(cons_arg.second, l_scope, ns_name, sub_ns_name);
                build_type_instance(infered_type_instance, cons_param, arg_type_instance, call_expr -> get_token());
            }
        }

        // typecheck the infered type instance
        try {
            type_instance_checker::complex_check(infered_type_instance, l_scope, ns_name);
        } catch(invalid_type err) {
            throw invalid_expression(err.get_token(), err.what());
        }

        // if we have type instance from the parser we compare it with the infered type
        // and if they equal, we keep the parser type instance
        if(has_parser_type_instance) {
            if(type_instance_weak_compare(parser_type_instance, infered_type_instance) == false) {
                throw invalid_expression(parser_type_instance.get_token(), "The type instance supplied along the expression: <" + mangle_type_instance(parser_type_instance) + "> is not the same as the one deduced by the inference engine: <" + mangle_type_instance(infered_type_instance) + ">.");
            }
            else {
                return parser_type_instance;
            }
        }
        else {
            call_expr -> set_type_instance(infered_type_instance, false);
            return infered_type_instance;
        }
    }

    /**
     * infer_function_call
     * infers the type instance of a function call expression
     */
    type_instance inferer::infer_function_call(function& new_fun, std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
        // we get all the data we can get from the call expression
        const std::string& call_name = call_expr -> get_name();
        std::vector<std::pair<token, std::shared_ptr<expr> > >& call_args = call_expr -> get_arguments();
        type_instance& ret_instance = call_expr -> get_return_type_instance();
        std::vector<type_instance>& constraint_instances = call_expr -> get_specializations();

        // we build the data we need from the call expression data
        std::vector<type_instance> args_instances;
        for(auto& arg : call_args)
            args_instances.push_back(inferer::infer(arg.second, l_scope, ns_name, sub_ns_name));
        std::vector<token> standins;
        std::shared_ptr<function> fun = nullptr;

        // we try to find the function
        try {
            fun = find_function(call_name, args_instances, ret_instance, l_scope, sub_ns_name, standins);
        } catch(symbol_not_found err) {
            throw invalid_expression(call_expr -> get_token(), err.what());
        } catch(symbol_can_collide err) {
            throw invalid_expression(call_expr -> get_token(), err.what());
        } catch(invalid_type err) {
            throw invalid_expression(err.get_token(), "No function declaration that corresponds to this function call was found. Reason: " + std::string(err.what()));
        }

        // we generate a new function from the function we found
        new_fun = * fun;
        function_generator generator(new_fun, constraint_instances, l_scope, sub_ns_name);
        try {
            generator.generate(args_instances, ret_instance);
        } catch(invalid_type err) {
            throw invalid_expression(err.get_token(), err.what());
        } catch(invalid_function err) {
            throw invalid_expression(err.get_token(), err.what());
        }

        // add the specialization to the root function
        new_fun.set_name(mangle_function(new_fun));
        fun -> add_specialization(new_fun);
        fun -> is_used(true);

        // the return type of the function is the type instance of the expression
        type_instance& fun_instance = new_fun.get_return_type_instance();

        // typecheck the infered type instance
        try {
            type_instance_checker::complex_check(fun_instance, l_scope, ns_name);
        } catch(invalid_type err) {
            throw invalid_expression(err.get_token(), err.what());
        }

        // set the type instance on the expression
        call_expr -> set_type_instance(fun_instance, false);

        // we got the function, its return type is the type instance of a function call expression
        return fun_instance;
    }

    /**
     * infer_identifier
     * infers the type instance of an identifier expression
     */
    type_instance inferer::infer_identifier(std::shared_ptr<expr> & an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
        std::shared_ptr<identifier_expression> const & id_expr = std::static_pointer_cast<identifier_expression>(an_expression);
        
        if(id_expr -> get_expression_type() == VAR_EXPR) {
            return inferer::infer_variable(id_expr, l_scope, ns_name, sub_ns_name);
        }
        else {
            return inferer::infer_constructor(id_expr, l_scope, ns_name, sub_ns_name);
        }
    }

    /**
     * infer_variable
     * infers the type instance of a variable expression
     */
    type_instance inferer::infer_variable(std::shared_ptr<identifier_expression> const & id_expr, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
        // if the expression already has a type instance, we return it
        if(id_expr -> type_instance_from_parser() == false && id_expr -> has_type_instance() == true) {
            return id_expr -> get_type_instance();
        }

        // if on the other hand a type instance from the parser was posted, we type check check it
        type_instance parser_type_instance;
        bool has_parser_type_instance = false;
        if(id_expr -> type_instance_from_parser() == true) {
            has_parser_type_instance = true;
            parser_type_instance = id_expr -> get_type_instance();

            try {
                // we typecheck the parser type instance
                std::pair<bool,bool> res = type_instance_checker::complex_check(parser_type_instance, l_scope, ns_name);
                // we don't allow parametrized type instances to be bound to expressions
                if (res.second == true) {
                    throw invalid_expression(parser_type_instance.get_token(), "Parametrized types cannot be used on expressions.");
                }
            } catch(invalid_type err) {
                throw invalid_expression(err.get_token(), err.what());
            }
        }

        try {
            std::shared_ptr<variable>& var_decl = l_scope -> get_variable(sub_ns_name, id_expr -> get_name());
            type_instance& var_instance = var_decl -> get_type_instance();

            // typecheck the infered type instance
            try {
                type_instance_checker::complex_check(var_instance, l_scope, ns_name);
            } catch(invalid_type err) {
                throw invalid_expression(err.get_token(), err.what());
            }

            // if we have a parser type instance, we make sure it is the same as the one we infered
            if(has_parser_type_instance) {
                if(type_instance_weak_compare(parser_type_instance, var_instance) == false)
                    throw invalid_expression(parser_type_instance.get_token(), "The type instance supplied <" + mangle_type_instance(parser_type_instance) + "> is not the same as the one infered <" + mangle_type_instance(var_instance) + ">.");
            }

            // set the type instance on the expression
            id_expr -> set_type_instance(var_instance, false);

            return var_instance;
        } catch(symbol_not_found err) {
            throw invalid_expression(id_expr -> get_token(), err.what());
        }
    }

    /**
     * infer_constructor
     * infers the type instance of an identifier constructor expression
     */
    type_instance inferer::infer_constructor(std::shared_ptr<identifier_expression> const & id_expr, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name) {
        // if the expression already has a type instance, we return it
        if(id_expr -> type_instance_from_parser() == false && id_expr -> has_type_instance() == true) {
            return id_expr -> get_type_instance();
        }

        // if on the other hand a type instance from the parser was posted, we type check check it
        type_instance parser_type_instance;
        bool has_parser_type_instance = false;
        if(id_expr -> type_instance_from_parser() == true) {
            has_parser_type_instance = true;
            parser_type_instance = id_expr -> get_type_instance();

            try {
                // we typecheck the parser type instance
                std::pair<bool,bool> res = type_instance_checker::complex_check(parser_type_instance, l_scope, ns_name);
                // we don't allow parametrized type instances to be bound to expressions
                if (res.second == true) {
                    throw invalid_expression(parser_type_instance.get_token(), "Parametrized types cannot be used on expressions.");
                }
            } catch(invalid_type err) {
                throw invalid_expression(err.get_token(), err.what());
            }
        }

        // we build the type instance from the default constructor we got
        default_constructor& cons = l_scope -> get_default_constructor(sub_ns_name, id_expr -> get_name(), 0);
        std::shared_ptr<type>& cons_type = cons.get_type();
        const token& type_token = cons_type -> get_token();
        type_instance cons_instance(const_cast<token&>(type_token), cons_type, sub_ns_name);

        // typecheck the infered type instance
        try {
            type_instance_checker::complex_check(cons_instance, l_scope, ns_name);
        } catch(invalid_type err) {            
            throw invalid_expression(err.get_token(), err.what());
        }

        // make sure that if the user set their own type instance, it is equal to the one we infered
        if(has_parser_type_instance) {
            if(type_instance_weak_compare(parser_type_instance, cons_instance) == false)
                throw invalid_expression(parser_type_instance.get_token(), "The type instance supplied <" + mangle_type_instance(parser_type_instance) + "> is not the same as the one infered <" + mangle_type_instance(cons_instance) + ">.");
        }

        // set the type instance on the expression
        id_expr -> set_type_instance(cons_instance, false);
        return cons_instance;
    }
}
