#ifndef AVALON_PROGRAM_AST_STMT_SWITCH_STMT_HPP_
#define AVALON_PROGRAM_AST_STMT_SWITCH_STMT_HPP_

#include <memory>
#include <vector>
#include <map>

#include "hir/ast/stmt/block_stmt.hpp"
#include "hir/symtable/scope.hpp"
#include "hir/ast/stmt/stmt.hpp"
#include "hir/ast/expr/expr.hpp"
#include "hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class switch_case;
    class switch_default;


    class switch_stmt : public stmt {
    public:
        /**
         * the constructor expects the token with source code information
         */
        switch_stmt(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * add_param
         * add a parameter
         */
        void add_param(std::shared_ptr<expr> param);

        /**
         * get_params
         * return a vector of parameters
         */
        std::vector<std::shared_ptr<expr> >& get_params();

        /**
         * add_case
         * add a case
         */
        void add_case(switch_case cas);

        /**
         * get_cases
         * returns all the cases under this statement
         */
        std::vector<switch_case>& get_cases();

        /**
         * set_default
         * set the default case
         */
        void set_default(switch_default cas);

        /**
         * get_default
         * returns the default case if any
         * throws a "case_error" if the default case doesn't exist
         */
        switch_default get_default();

        /**
         * has_default
         * returns true if this switch statement has a default case
         */
        bool has_default();

        /**
         * is_switch
         * returns true as this is a switch statement
         */
        virtual bool is_switch() {
            return true;
        }

    private:
        /*
         * token with source code information
         */
        token m_tok;

        /*
         * vector of parameters
         */
        std::vector<std::shared_ptr<expr> > m_params;
        
        /*
         * vector of cases
         */
        std::vector<switch_case> m_cases;

        /*
         * the default case if any
         */
        std::shared_ptr<switch_default> m_default;
    };


    class switch_case {
    public:
        /**
         * the constructor expects the token with source code information
         */
        switch_case(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * set_scope
         * specify the scope bound to this case
         */
        void set_scope(std::shared_ptr<scope>& l_scope);

        /**
         * get_scope
         * return the scope bound to this case
         */
        std::shared_ptr<scope>& get_scope();

        /**
         * add_match
         * add an expression this case can match
         */
        void add_match(std::shared_ptr<expr> match);

        /**
         * get_matches
         * return a vector of expressions this case can match
         */
        std::vector<std::shared_ptr<expr> >& get_matches();

        /**
         * set_block
         * set the block of statements to execute
         */
        void set_block(block_stmt& blc);

        /**
         * get_block
         * returns the block of statements that are to be executed for this case
         */
        block_stmt& get_block();

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
         * vector of expressions this case can match
         */
        std::vector<std::shared_ptr<expr> > m_matches;

        /*
         * the block of statements to execute
         */
        block_stmt m_blc;
    };

    class switch_default {
    public:
        /**
         * the constructor expects the token with source code information
         */
        switch_default(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * set_scope
         * specify the scope bound to this default case
         */
        void set_scope(std::shared_ptr<scope>& l_scope);

        /**
         * get_scope
         * return the scope bound to this default case
         */
        std::shared_ptr<scope>& get_scope();

        /**
         * set_block
         * set the block of statements to execute
         */
        void set_block(block_stmt& blc);

        /**
         * get_block
         * returns the block of statements that are to be executed for the default case
         */
        block_stmt& get_block();

    private:
        /*
         * token with source code information
         */
        token m_tok;

        /*
         * scope introduced by this default case
         */
        std::shared_ptr<scope> m_scope;

        /*
         * the block of statements to execute
         */
        block_stmt m_blc;
    };
}

#endif
