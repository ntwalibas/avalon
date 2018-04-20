#include <stdexcept>

#include "lexer/token.hpp"
#include "checker/exceptions/invalid_type.hpp"

namespace avalon {
    /**
     * the constructor expects the token of the type that resulted in failure
     * and the message about what exactly went wrong.
     */
    invalid_type::invalid_type(const token& tok, const std::string& message) : std::runtime_error(message), m_tok(tok) {
    }
    
    /**
     * what
     * override the what function defined by runtime_error
     */
    const char * invalid_type::what() const noexcept {
        return std::runtime_error::what();
    }

    /**
     * get_token
     * returns the token corresponding to the type that failed checking
     */
    const token& invalid_type::get_token() {
        return m_tok;
    }
}
