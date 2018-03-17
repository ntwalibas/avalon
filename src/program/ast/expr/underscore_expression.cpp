#include "program/ast/expr/underscore_expression.hpp"
#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token associated with this expression
     */
    underscore_expression::underscore_expression(token& tok) : m_tok(tok) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& underscore_expression::get_token() const {
        return m_tok;
    }
}
