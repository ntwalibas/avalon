#include "representer/mir/ast/stmt/mir_pass_stmt.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    mir_pass_stmt::mir_pass_stmt(token& tok) : m_tok(tok) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_pass_stmt::get_token() const {
        return m_tok;
    }
}
