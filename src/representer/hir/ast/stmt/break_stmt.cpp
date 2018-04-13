#include "representer/hir/ast/stmt/break_stmt.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    break_stmt::break_stmt(token& tok) : m_tok(tok) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& break_stmt::get_token() const {
        return m_tok;
    }
}
