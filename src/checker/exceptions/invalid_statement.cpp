#include <stdexcept>

#include "lexer/token.hpp"
#include "checker/exceptions/invalid_statement.hpp"

namespace avalon {
    /**
     * the constructor expects the statement name token
     * and the message about what exactly went wrong.
     */
    invalid_statement::invalid_statement(const token& tok, const std::string& message) : std::runtime_error(message), m_tok(tok) {
    }
    
    /**
     * what
     * override the what function defined by runtime_error
     */
    const char * invalid_statement::what() const noexcept {
        return std::runtime_error::what();
    }

    /**
     * get_token
     * returns the token to the statement name in the source code
     */
    const token& invalid_statement::get_token() {
        return m_tok;
    }
}
