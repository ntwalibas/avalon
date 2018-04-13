#include "representer/mir/ast/stmt/mir_continue_stmt.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    mir_continue_stmt::mir_continue_stmt(token& tok) : m_tok(tok) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_continue_stmt::get_token() const {
        return m_tok;
    }
}
