#ifndef AVALON_PROGRAM_AST_EXPR_IDENTIFIER_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_IDENTIFIER_EXPRESSION_HPP_

#include <string>

#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class identifier_expression : public expr {
    public:
        /**
         * the constructor expects the token with source code information
         */
        identifier_expression(token& tok);

        /**
         * get_token
         * returns the name of the identifier
         */
        const std::string& get_name() const;

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;
        
        /**
         * is_identifier_expression
         * returns true as this is an identifier expression
         */
        virtual bool is_identifier_expression() {
            return true;
        }

    private:
        /*
         * the name of the identifier expression
         */
        std::string m_name;
        
        /*
         * the token with source code information
         */
        token m_tok;
    };
}

#endif
