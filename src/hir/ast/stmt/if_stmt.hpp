#ifndef AVALON_HIR_AST_STMT_IF_STMT_HPP_
#define AVALON_HIR_AST_STMT_IF_STMT_HPP_

#include <memory>
#include <vector>

#include "hir/ast/stmt/block_stmt.hpp"
#include "hir/symtable/scope.hpp"
#include "hir/ast/stmt/stmt.hpp"
#include "hir/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class elif_branch;
    class else_branch;

    class if_stmt : public stmt {
    public:
        /**
         * the constructor expects the token with source code information
         */
        if_stmt(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * set_scope
         * specify the scope bound to the main branch
         */
        void set_scope(std::shared_ptr<scope>& l_scope);

        /**
         * get_scope
         * return the scope bound to the main branch
         */
        std::shared_ptr<scope>& get_scope();

        /**
         * set_condition
         * sets the condition under which the conitional will execute
         */
        void set_condition(std::shared_ptr<expr>& condition);

        /**
         * get_condition
         * returns the condition under which the conditional will execute
         */
        std::shared_ptr<expr>& get_condition();

        /**
         * set_block
         * set the block of statements to execute
         */
        void set_block(block_stmt& blc);

        /**
         * get_block
         * returns the block of statements that are to be executed for this conditional
         */
        block_stmt& get_block();

        /**
         * add_elif
         * add an elif branch to this statement
         */
        void add_elif(elif_branch& branch);

        /**
         * get_elifs
         * returns a vector with all the elif branches
         */
        std::vector<elif_branch>& get_elifs();

        /**
         * set_else
         * sets the else branch
         */
        void set_else(else_branch& branch);

        /**
         * get_else
         * returns the else branch if any
         * throws a "branch_error" if no else branch was set
         */
        else_branch get_else() const;

        /**
         * has_else
         * returns true if this statement has else branch
         */
        bool has_else() const;

        /**
         * is_if
         * returns true as this is an if statement
         */
        virtual bool is_if() {
            return true;
        }

    private:
        /*
         * token with source code information
         */
        token m_tok;

        /*
         * scope introduced by the main branch
         */
        std::shared_ptr<scope> m_scope;

        /*
         * the condition undder which the condition will execute its body (or not)
         */
        std::shared_ptr<expr> m_condition;

        /*
         * the body of the conditional
         */
        block_stmt m_blc;

        /*
         * vector of elif branches
         */
        std::vector<elif_branch> m_elifs;

        /*
         * else branch if any
         */
        std::shared_ptr<else_branch> m_else;
    };


    class elif_branch {
    public:
        /**
         * the constructor expects the token with source code information
         */
        elif_branch(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * set_scope
         * specify the scope bound to this elif branch
         */
        void set_scope(std::shared_ptr<scope>& l_scope);

        /**
         * get_scope
         * return the scope bound to this elif branch
         */
        std::shared_ptr<scope>& get_scope();

        /**
         * set_condition
         * sets the condition under which the elif branch will execute
         */
        void set_condition(std::shared_ptr<expr>& condition);

        /**
         * get_condition
         * returns the condition under which the elif branch will execute
         */
        std::shared_ptr<expr>& get_condition();

        /**
         * set_block
         * set the block of statements to execute
         */
        void set_block(block_stmt& blc);

        /**
         * get_block
         * returns the block of statements that are to be executed for this elif branch
         */
        block_stmt& get_block();

    private:
        /*
         * token with source code information
         */
        token m_tok;

        /*
         * scope introduced by this elif branch
         */
        std::shared_ptr<scope> m_scope;

        /*
         * the condition undder which the elif branch will execute its body (or not)
         */
        std::shared_ptr<expr> m_condition;

        /*
         * the body of the elif branch
         */
        block_stmt m_blc;
    };


    class else_branch {
    public:
        /**
         * the constructor expects the token with source code information
         */
        else_branch(token& tok);

        /**
         * set_scope
         * specify the scope bound to this else branch
         */
        void set_scope(std::shared_ptr<scope>& l_scope);

        /**
         * get_scope
         * return the scope bound to this else branch
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
         * returns the block of statements that are to be executed for the else branch
         */
        block_stmt& get_block();

    private:
        /*
         * token with source code information
         */
        token m_tok;

        /*
         * scope introduced by this else branch
         */
        std::shared_ptr<scope> m_scope;

        /*
         * the body of the else branch
         */
        block_stmt m_blc;
    };
}

#endif
