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

/* Inference engine */
#include "inferer/inferer.hpp"

/* Lexer */
#include "lexer/token.hpp"


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
            return inferer::infer_literal(an_expression, l_scope);
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
    type_instance inferer::infer_literal(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope) {
        std::shared_ptr<literal_expression> const & lit_expr = std::static_pointer_cast<literal_expression>(an_expression);

        // if the expression already has a type instance set, we return that
        if(lit_expr -> has_type_instance()) {
            return lit_expr -> get_type_instance();
        }
        
        if(lit_expr -> get_expression_type() == INTEGER_EXPR) {
            std::shared_ptr<type>& int_type = l_scope -> get_type("*", "int", 0);
            type_instance int_type_instance(int_type_tok, int_type, "*");
            lit_expr -> set_type_instance(int_type_instance);
            return int_type_instance;
        }
        else if(lit_expr -> get_expression_type() == DECIMAL_EXPR) {
            std::shared_ptr<type>& dec_type = l_scope -> get_type("*", "dec", 0);
            type_instance dec_type_instance(dec_type_tok, dec_type, "*");
            lit_expr -> set_type_instance(dec_type_instance);
            return dec_type_instance;
        }
        else if(lit_expr -> get_expression_type() == FLOATING_POINT_EXPR) {
            std::shared_ptr<type>& float_type = l_scope -> get_type("*", "float", 0);
            type_instance float_type_instance(float_type_tok, float_type, "*");
            lit_expr -> set_type_instance(float_type_instance);
            return float_type_instance;
        }
        else if(lit_expr -> get_expression_type() == STRING_EXPR) {
            std::shared_ptr<type>& string_type = l_scope -> get_type("*", "string", 0);
            type_instance string_type_instance(string_type_tok, string_type, "*");
            lit_expr -> set_type_instance(string_type_instance);
            return string_type_instance;
        }
        else {
            throw std::runtime_error("[compiler error] unexpected literal expression in inference engine.");
        }
    }

    /**
     * infer_tuple
     * infers the type instance of a tuple
     */
    type_instance inferer::infer_tuple(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name) {
        std::shared_ptr<tuple_expression> const & tup_expr = std::static_pointer_cast<tuple_expression>(an_expression);
        
        // if the expression already has a type instance set, we return that
        if(tup_expr -> has_type_instance()) {
            return tup_expr -> get_type_instance();
        }

        // we create a type and type instance out of the expression dynamically
        token tok = tup_expr -> get_token();
        std::shared_ptr<type> tup_type = std::make_shared<type>(tok, VALID);
        type_instance instance(tok, tup_type, "*");

        // we fill in the type instance parameters
        std::vector<std::shared_ptr<expr> >& elements = tup_expr -> get_elements();
        for(auto& element : elements) {
            type_instance el_instance = inferer::infer(element, l_scope, ns_name);
            instance.add_param(el_instance);
        }

        tup_expr -> set_type_instance(instance);
        return instance;
    }
}
