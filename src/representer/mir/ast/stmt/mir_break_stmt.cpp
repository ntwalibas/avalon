#include "representer/mir/ast/stmt/mir_break_stmt.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    mir_break_stmt::mir_break_stmt(token& tok) : m_tok(tok) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_break_stmt::get_token() const {
        return m_tok;
    }
}
