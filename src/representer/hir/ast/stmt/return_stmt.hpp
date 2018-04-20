#ifndef AVALON_HIR_AST_STMT_RETURN_STMT_HPP_
#define AVALON_HIR_AST_STMT_RETURN_STMT_HPP_

#include <memory>

#include "representer/hir/ast/stmt/stmt.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class return_stmt : public stmt {
    public:
        /**
         * the constructor expects the token with source code information
         */
        return_stmt(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * set_expression
         * set the expression to be returned if any
         */
        void set_expression(std::shared_ptr<expr>& expression);

        /**
         * get_expression
         * returns the expression that is meant to be returned
         */
        std::shared_ptr<expr>& get_expression();

        /**
         * has_expression
         * return true if an expression was supplied to the return statement
         */
        bool has_expression();

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
        std::shared_ptr<expr> m_expression;
    };
}

#endif
