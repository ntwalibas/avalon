#ifndef AVALON_MIR_AST_STMT_PASS_STMT_HPP_
#define AVALON_MIR_AST_STMT_PASS_STMT_HPP_

#include "representer/mir/ast/stmt/mir_stmt.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_pass_stmt : public mir_stmt {
    public:
        /**
         * the constructor expects the token with source code information
         */
        mir_pass_stmt(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

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
