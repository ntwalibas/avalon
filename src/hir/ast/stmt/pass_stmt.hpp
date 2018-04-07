#ifndef AVALON_PROGRAM_AST_STMT_PASS_STMT_HPP_
#define AVALON_PROGRAM_AST_STMT_PASS_STMT_HPP_

#include "hir/ast/stmt/stmt.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class pass_stmt : public stmt {
    public:
        /**
         * the constructor expects the token with source code information
         */
        pass_stmt(token& tok);

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
