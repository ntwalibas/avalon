#ifndef AVALON_MIR_AST_STMT_BREAK_STMT_HPP_
#define AVALON_MIR_AST_STMT_BREAK_STMT_HPP_

#include "representer/mir/ast/stmt/mir_stmt.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_break_stmt : public mir_stmt {
    public:
        /**
         * the constructor expects the token with source code information
         */
        mir_break_stmt(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

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
