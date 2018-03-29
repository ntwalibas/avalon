#include <stdexcept>
#include <memory>
#include <string>

/* Expressions */
#include "program/ast/expr/underscore_expression.hpp"
#include "program/ast/expr/literal_expression.hpp"
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
     * infer_underscore
     * infers the type instance of a literal
     */
    type_instance inferer::infer_literal(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope) {
        std::shared_ptr<literal_expression> const & lit_expr = std::static_pointer_cast<literal_expression>(an_expression);
        
        if(lit_expr -> get_expression_type() == INTEGER_EXPR) {
            std::shared_ptr<type>& int_type = l_scope -> get_type("*", "int", 0);
            type_instance int_type_instance(int_type_tok, int_type, "*");
            return int_type_instance;
        }
        else if(lit_expr -> get_expression_type() == DECIMAL_EXPR) {
            std::shared_ptr<type>& dec_type = l_scope -> get_type("*", "dec", 0);
            type_instance dec_type_instance(dec_type_tok, dec_type, "*");
            return dec_type_instance;
        }
        else if(lit_expr -> get_expression_type() == FLOATING_POINT_EXPR) {
            std::shared_ptr<type>& float_type = l_scope -> get_type("*", "float", 0);
            type_instance float_type_instance(float_type_tok, float_type, "*");
            return float_type_instance;
        }
        else if(lit_expr -> get_expression_type() == STRING_EXPR) {
            std::shared_ptr<type>& string_type = l_scope -> get_type("*", "string", 0);
            type_instance string_type_instance(string_type_tok, string_type, "*");
            return string_type_instance;
        }
        else {
            throw std::runtime_error("[compiler error] unexpected literal expression in inference engine.");
        }
    }
}
