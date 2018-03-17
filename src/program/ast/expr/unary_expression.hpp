#ifndef AVALON_PROGRAM_AST_EXPR_UNARY_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_UNARY_EXPRESSION_HPP_

#include <memory>

#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    enum unary_expression_type {
        PLUS_SIGN_EXPR,
        MINUS_SIGN_EXPR,
        BITWISE_NOT_EXPR,
        LOGICAL_NOT_EXPR
    };

    class unary_expression : public expr {
    public:
        /**
         * the constructor expects the type of unary expression and the operand of the unary operator
         */
        unary_expression(token& tok, unary_expression_type expr_type, std::shared_ptr<expr> val);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * get_expression_type
         * returns the type of expression we are dealing
         */
        const unary_expression_type& get_expression_type() const;

        /**
         * get_val
         * returns the unary operator operand
         */
        std::shared_ptr<expr>& get_val();
        
        /**
         * is_unary_expression
         * return true as objects of this class are unary expressions
         */
        virtual bool is_unary_expression() {
            return true;
        }

    private:
        /*
         * the token to be used to show errors
         */
        token m_tok;

        /*
         * the type of unary expression we have
         */
        unary_expression_type m_expr_type;

        /*
         * the operand to the unary operator
         */
        std::shared_ptr<expr> m_val;
    };
}

#endif
