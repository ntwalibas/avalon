#ifndef AVALON_MIR_AST_STMT_WHILE_STMT_HPP_
#define AVALON_MIR_AST_STMT_WHILE_STMT_HPP_

#include <memory>

#include "representer/mir/ast/stmt/mir_block_stmt.hpp"
#include "representer/mir/symtable/mir_scope.hpp"
#include "representer/mir/ast/stmt/mir_stmt.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_while_stmt : public mir_stmt {
    public:
        /**
         * the constructor expects the token with source code information
         */
        mir_while_stmt(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * set_scope
         * specify the scope bound to this while statement
         */
        void set_scope(std::shared_ptr<mir_scope>& l_scope);

        /**
         * get_scope
         * return the scope bound to this while statement
         */
        std::shared_ptr<mir_scope>& get_scope();

        /**
         * set_condition
         * sets the condition under which the loop will execute
         */
        void set_condition(std::shared_ptr<mir_expr>& condition);

        /**
         * get_condition
         * returns the condition under which the loop will execute
         */
        std::shared_ptr<mir_expr>& get_condition();

        /**
         * set_block
         * set the block of statements to execute
         */
        void set_block(mir_block_stmt& blc);

        /**
         * get_block
         * returns the block of statements that are to be executed for this loop
         */
        mir_block_stmt& get_block();

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
        std::shared_ptr<mir_scope> m_scope;

        /*
         * the condition undder which the loop will execute its body (or not)
         */
        std::shared_ptr<mir_expr> m_condition;

        /*
         * the body of the loop
         */
        mir_block_stmt m_blc;
    };
}

#endif
