#ifndef AVALON_HIR_AST_STMT_WHILE_STMT_HPP_
#define AVALON_HIR_AST_STMT_WHILE_STMT_HPP_

#include <memory>

#include "representer/hir/ast/stmt/block_stmt.hpp"
#include "representer/hir/symtable/scope.hpp"
#include "representer/hir/ast/stmt/stmt.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class while_stmt : public stmt {
    public:
        /**
         * the constructor expects the token with source code information
         */
        while_stmt(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * set_scope
         * specify the scope bound to this while statement
         */
        void set_scope(std::shared_ptr<scope>& l_scope);

        /**
         * get_scope
         * return the scope bound to this while statement
         */
        std::shared_ptr<scope>& get_scope();

        /**
         * set_condition
         * sets the condition under which the loop will execute
         */
        void set_condition(std::shared_ptr<expr>& condition);

        /**
         * get_condition
         * returns the condition under which the loop will execute
         */
        std::shared_ptr<expr>& get_condition();

        /**
         * set_block
         * set the block of statements to execute
         */
        void set_block(block_stmt& blc);

        /**
         * get_block
         * returns the block of statements that are to be executed for this loop
         */
        block_stmt& get_block();

        /**
         * is_while
         * returns true as this is a while statement
         */
        virtual bool is_while() {
            return true;
        }

    private:
        /*
         * token with source code information
         */
        token m_tok;

        /*
         * scope introduced by this case
         */
        std::shared_ptr<scope> m_scope;

        /*
         * the condition undder which the loop will execute its body (or not)
         */
        std::shared_ptr<expr> m_condition;

        /*
         * the body of the loop
         */
        block_stmt m_blc;
    };
}

#endif
