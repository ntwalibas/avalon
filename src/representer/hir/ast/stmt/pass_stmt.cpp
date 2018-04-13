#include "representer/hir/ast/stmt/pass_stmt.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    pass_stmt::pass_stmt(token& tok) : m_tok(tok) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& pass_stmt::get_token() const {
        return m_tok;
    }
}
