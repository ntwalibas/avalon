#ifndef AVALON_HIR_AST_STMT_PASS_STMT_HPP_
#define AVALON_HIR_AST_STMT_PASS_STMT_HPP_

#include "representer/hir/ast/stmt/stmt.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class pass_stmt : public stmt {
    public:
        /**
         * the constructor expects the token with source code information
         */
        pass_stmt(token& tok);

        /**
         * copy constructor
         */
        pass_stmt(const std::shared_ptr<pass_stmt>& pas_stmt);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * token
         * returns this statement token
         */
        virtual const token& stmt_token() const {
            return m_tok;
        }

        /**
         * is_pass_stmt
         * returns true as this is a pass statement
         */
        virtual bool is_pass() {
            return true;
        }

    private:
        /*
         * token with source code information
         */
        token m_tok;
    };
}

#endif
