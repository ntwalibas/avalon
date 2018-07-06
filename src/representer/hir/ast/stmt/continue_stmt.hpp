#ifndef AVALON_HIR_AST_STMT_CONTINUE_STMT_HPP_
#define AVALON_HIR_AST_STMT_CONTINUE_STMT_HPP_

#include "representer/hir/ast/stmt/stmt.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class continue_stmt : public stmt {
    public:
        /**
         * the constructor expects the token with source code information
         */
        continue_stmt(token& tok);

        /**
         * copy constructor
         */
        continue_stmt(const std::shared_ptr<continue_stmt>& cont_stmt);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * is_continue
         * returns true as this is a continue statement
         */
        virtual bool is_continue() {
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
