#include <string>

#include "program/ast/expr/identifier_expression.hpp"
#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    identifier_expression::identifier_expression(token& tok) : m_name(tok.get_lexeme()), m_tok(tok) {
    }

    /**
     * get_token
     * returns the name of the identifier
     */
    const std::string& identifier_expression::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& identifier_expression::get_token() const {
        return m_tok;
    }
}
