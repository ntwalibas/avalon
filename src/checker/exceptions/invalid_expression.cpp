#include <stdexcept>

#include "lexer/token.hpp"
#include "checker/exceptions/invalid_expression.hpp"

namespace avalon {
    /**
     * the constructor expects the expression token token
     * and the message about what exactly went wrong.
     */
    invalid_expression::invalid_expression(const token& tok, const std::string& message) : std::runtime_error(message), m_tok(tok) {
    }
    
    /**
     * what
     * override the what expression defined by runtime_error
     */
    const char * invalid_expression::what() const noexcept {
        return std::runtime_error::what();
    }

    /**
     * get_token
     * returns the token to the expression token in the source code
     */
    const token& invalid_expression::get_token() {
        return m_tok;
    }
}
