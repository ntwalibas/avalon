#ifndef AVALON_PROGRAM_AST_EXPR_GROUPED_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_GROUPED_EXPRESSION_HPP_

#include <memory>

#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class grouped_expression : public expr {
    public:
        /**
         * the constructor expects the operand of the grouped operator
         */
        grouped_expression(token& tok, std::shared_ptr<expr>& val);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * get_value
         * returns the grouped operator operand
         */
        std::shared_ptr<expr>& get_value();
        
        /**
         * is_grouped_expression
         * return true as objects of this class are grouped expressions
         */
        virtual bool is_grouped_expression() {
            return true;
        }

    private:
        /*
         * the token to be used to show errors
         */
        token m_tok;

        /*
         * the operand to the grouped operator
         */
        std::shared_ptr<expr> m_val;
    };
}

#endif
