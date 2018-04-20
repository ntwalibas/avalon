#include <stdexcept>

#include "lexer/token.hpp"
#include "checker/exceptions/invalid_variable.hpp"

namespace avalon {
    /**
     * the constructor expects the variable name token
     * and the message about what exactly went wrong.
     */
    invalid_variable::invalid_variable(const token& tok, const std::string& message) : std::runtime_error(message), m_tok(tok) {
    }
    
    /**
     * what
     * override the what variable defined by runtime_error
     */
    const char * invalid_variable::what() const noexcept {
        return std::runtime_error::what();
    }

    /**
     * get_token
     * returns the token to the variable name in the source code
     */
    const token& invalid_variable::get_token() {
        return m_tok;
    }
}
