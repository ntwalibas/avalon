#ifndef AVALON_PROGRAM_AST_EXPR_UNDERSCORE_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_UNDERSCORE_EXPRESSION_HPP_

#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class underscore_expression : public expr {
    public:
        /**
         * the constructor expects the token associated with this expression
         */
        underscore_expression(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;
        
        /**
         * is_underscore_expression
         * this function is used during deconstruction assignment to find out if we have an underscore expression
         */
        virtual bool is_underscore_expression() {
            return true;
        }

    private:
        /*
         * the token with source code information
         */
        token m_tok;
    };
}

#endif
