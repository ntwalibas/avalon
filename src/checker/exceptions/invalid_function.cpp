#include <stdexcept>

#include "lexer/token.hpp"
#include "checker/exceptions/invalid_function.hpp"

namespace avalon {
    /**
     * the constructor expects the function name token
     * and the message about what exactly went wrong.
     */
    invalid_function::invalid_function(const token& tok, const std::string& message) : std::runtime_error(message), m_tok(tok) {
    }
    
    /**
     * what
     * override the what function defined by runtime_error
     */
    const char * invalid_function::what() const noexcept {
        return std::runtime_error::what();
    }

    /**
     * get_token
     * returns the token to the function name in the source code
     */
    const token& invalid_function::get_token() {
        return m_tok;
    }
}
