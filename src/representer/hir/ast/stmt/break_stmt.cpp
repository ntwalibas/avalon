#include "representer/hir/ast/stmt/break_stmt.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    break_stmt::break_stmt(token& tok) : m_tok(tok) {        
    }

    /**
     * copy constructor
     */
    break_stmt::break_stmt(const std::shared_ptr<break_stmt>& br_stmt) : m_tok(br_stmt -> get_token()) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& break_stmt::get_token() const {
        return m_tok;
    }
}
