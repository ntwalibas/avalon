#ifndef AVALON_MIR_AST_STMT_RETURN_STMT_HPP_
#define AVALON_MIR_AST_STMT_RETURN_STMT_HPP_

#include <memory>

#include "representer/mir/ast/stmt/mir_stmt.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_return_stmt : public mir_stmt {
    public:
        /**
         * the constructor expects the token with source code information
         */
        mir_return_stmt(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * set_expression
         * set the expression to be returned if any
         */
        void set_expression(std::shared_ptr<mir_expr>& expression);

        /**
         * get_expression
         * returns the expression that is meant to be returned
         */
        std::shared_ptr<mir_expr>& get_expression();

        /**
         * is_return
         * returns true as this is a return statement
         */
        virtual bool is_return() {
            return true;
        }

    private:
        /*
         * token with source code information
         */
        token m_tok;

        /*
         * the expression to be returned by a function
         */
        std::shared_ptr<mir_expr> m_expression;
    };
}

#endif
