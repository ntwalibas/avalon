#include <stdexcept>
#include <memory>
#include <string>

/* Expressions */
#include "program/ast/expr/underscore_expression.hpp"
#include "program/ast/expr/literal_expression.hpp"
#include "program/ast/expr/tuple_expression.hpp"
#include "program/ast/expr/expr.hpp"

/* Symbol table */
#include "program/symtable/scope.hpp"

/* Declarations */
#include "program/ast/decl/type.hpp"

/* Checker */
#include "checker/weak/decl/type/type_checker.hpp"

/* Inferer */
#include "inferer/inferer.hpp"

/* Lexer */
#include "lexer/token.hpp"

/* Exceptions */
#include "checker/exceptions/invalid_type.hpp"


namespace avalon {
    /**
     * infer
     * given an expression, this function infers the type instance of said expression and returns it
     */
    type_instance inferer::infer(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name) {
        if(an_expression -> is_underscore_expression()) {
            return inferer::infer_underscore(an_expression);
        }
        else if(an_expression -> is_literal_expression()) {
            return inferer::infer_literal(an_expression, l_scope, ns_name);
        }
        else if(an_expression -> is_tuple_expression()) {
            return inferer::infer_tuple(an_expression, l_scope, ns_name);
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
        type_instance instance(star_tok, "*");
        und_expr -> set_type_instance(instance);
        return instance;
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
                    throw invalid_type(parser_type_instance.get_token(), "Parametrized types cannot be used on expressions.");
                }
            } catch(invalid_type err) {
                throw err;
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

        // if we have type instance from the parser we compare it with the infered type
        // and if they equal, we keep the parser type instance
        if(has_parser_type_instance) {
            if(type_instance_weak_compare(parser_type_instance, infered_type_instance) == false) {
                throw invalid_type(parser_type_instance.get_token(), "The type instance supplied along the expression: <" + mangle_type_instance(parser_type_instance) + "> is not the same as the one deduced by the inference engine: <" + mangle_type_instance(infered_type_instance) + ">.");
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
                    throw invalid_type(parser_type_instance.get_token(), "Parametrized types cannot be used on expressions.");
                }
            } catch(invalid_type err) {
                throw err;
            }
        }

        // we create a type and type instance out of the expression dynamically
        token tok = tup_expr -> get_token();
        std::shared_ptr<type> tup_type = std::make_shared<type>(tok, VALID);
        type_instance infered_type_instance(tok, tup_type, "*");
        infered_type_instance.set_category(TUPLE);

        // we fill in the type instance parameters
        std::vector<std::shared_ptr<expr> >& elements = tup_expr -> get_elements();
        for(auto& element : elements) {
            type_instance el_instance = inferer::infer(element, l_scope, ns_name);
            infered_type_instance.add_param(el_instance);
        }

        // if we have type instance from the parser we compare it with the infered type
        // and if they equal, we keep the parser type instance
        if(has_parser_type_instance) {
            if(type_instance_weak_compare(parser_type_instance, infered_type_instance) == false) {
                throw invalid_type(parser_type_instance.get_token(), "The type instance supplied along the expression: <" + mangle_type_instance(parser_type_instance) + "> is not the same as the one deduced by the inference engine: <" + mangle_type_instance(infered_type_instance) + ">.");
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
}
