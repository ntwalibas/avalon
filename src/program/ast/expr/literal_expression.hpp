#ifndef AVALON_PROGRAM_AST_EXPR_LITERAL_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_LITERAL_EXPRESSION_HPP_

#include <string>

#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class literal_expression : public expr {
    public:
        /**
         * the constructor expects the token with source code information
         */
        literal_expression(token& tok, const std::string& val);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

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
         * the string representation of the literal
         */
        std::string m_val;
    };
}

#endif
