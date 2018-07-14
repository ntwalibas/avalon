#ifndef AVALON_HIR_AST_STMT_BREAK_STMT_HPP_
#define AVALON_HIR_AST_STMT_BREAK_STMT_HPP_

#include "representer/hir/ast/stmt/stmt.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class break_stmt : public stmt {
    public:
        /**
         * the constructor expects the token with source code information
         */
        break_stmt(token& tok);

        /**
         * copy constructor
         */
        break_stmt(const std::shared_ptr<break_stmt>& br_stmt);

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
         * is_break
         * returns true as this is a break statement
         */
        virtual bool is_break() {
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
