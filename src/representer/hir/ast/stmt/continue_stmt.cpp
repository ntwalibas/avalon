#include "representer/hir/ast/stmt/continue_stmt.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    continue_stmt::continue_stmt(token& tok) : m_tok(tok) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& continue_stmt::get_token() const {
        return m_tok;
    }
}
