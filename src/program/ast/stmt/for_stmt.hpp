#ifndef AVALON_PROGRAM_AST_STMT_FOR_STMT_HPP_
#define AVALON_PROGRAM_AST_STMT_FOR_STMT_HPP_

#include <memory>

#include "program/ast/stmt/block_stmt.hpp"
#include "program/symtable/scope.hpp"
#include "program/ast/stmt/stmt.hpp"
#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class empty_branch;

    class for_stmt : public stmt {
    public:
        /**
         * the constructor expects the token with source code information
         */
        for_stmt(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * set_scope
         * specify the scope bound to this for statement
         */
        void set_scope(std::shared_ptr<scope>& l_scope);

        /**
         * get_scope
         * return the scope bound to this for statement
         */
        std::shared_ptr<scope>& get_scope();

        /**
         * set_iteration
         * sets the iteration that drives the loop
         */
        void set_iteration(std::shared_ptr<expr>& condition);

        /**
         * get_iteration
         * returns the iteration that drives the loop
         */
        std::shared_ptr<expr>& get_iteration();

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
         * set_empty
         * sets the empty branch
         */
        void set_empty(empty_branch& branch);

        /**
         * get_empty
         * returns the empty branch if any
         * throws a "branch_error" if no empty branch was set
         */
        empty_branch get_empty() const;

        /**
         * has_empty
         * returns true if this statement has empty branch
         */
        bool has_empty() const;

        /**
         * is_for
         * returns true as this is a for statement
         */
        virtual bool is_for() {
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
         * the iteration that drives the loop
         */
        std::shared_ptr<expr> m_iteration;

        /*
         * the body of the loop
         */
        block_stmt m_blc;

        /*
         * empty branch if any
         */
        std::shared_ptr<empty_branch> m_empty;
    };


    class empty_branch {
    public:
        /**
         * the constructor expects the token with source code information
         */
        empty_branch(token& tok);

        /**
         * set_scope
         * specify the scope bound to this empty branch
         */
        void set_scope(std::shared_ptr<scope>& l_scope);

        /**
         * get_scope
         * return the scope bound to this empty branch
         */
        std::shared_ptr<scope>& get_scope();

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * set_block
         * set the block of statements to execute
         */
        void set_block(block_stmt& blc);

        /**
         * get_block
         * returns the block of statements that are to be executed for the empty branch
         */
        block_stmt& get_block();

    private:
        /*
         * token with source code information
         */
        token m_tok;

        /*
         * scope introduced by this empty branch
         */
        std::shared_ptr<scope> m_scope;

        /*
         * the body of the empty branch
         */
        block_stmt m_blc;
    };
}

#endif
