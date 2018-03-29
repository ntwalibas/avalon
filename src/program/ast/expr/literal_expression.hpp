#ifndef AVALON_PROGRAM_AST_EXPR_LITERAL_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_LITERAL_EXPRESSION_HPP_

#include <string>

#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    enum literal_expression_type {
        INTEGER_EXPR,
        DECIMAL_EXPR,
        FLOATING_POINT_EXPR,
        STRING_EXPR
    };

    class literal_expression : public expr {
    public:
        /**
         * the constructor expects the token with source code information
         */
        literal_expression(token& tok, literal_expression_type expr_type, const std::string& val);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * get_expression_type
         * returns the type of literal this expression holds
         */
        const literal_expression_type& get_expression_type() const;

        /**
         * get_value
         * returns the string representing the value stored within this literal
         */
        const std::string& get_value() const;
        
        /**
         * is_literal_expression
         * returns true as this is an literal expression
         */
        virtual bool is_literal_expression() {
            return true;
        }

    private:
        /*
         * the token with source code information
         */
        token m_tok;

        /*
         * the type of literal expression this is
         */
        literal_expression_type m_expr_type;

        /*
         * the string representation of the literal
         */
        std::string m_val;
    };
}

#endif
