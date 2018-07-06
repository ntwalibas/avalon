#include "representer/hir/ast/stmt/continue_stmt.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    continue_stmt::continue_stmt(token& tok) : m_tok(tok) {        
    }

    /**
     * copy constructor
     */
    continue_stmt::continue_stmt(const std::shared_ptr<continue_stmt>& cont_stmt) : m_tok(cont_stmt -> get_token()) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& continue_stmt::get_token() const {
        return m_tok;
    }
}
