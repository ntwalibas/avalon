#include <stdexcept>

#include "lexer/token.hpp"
#include "checker/exceptions/invalid_import.hpp"

namespace avalon {
    /**
     * the constructor expects the token of the import that resulted in failure
     * and the message about what exactly went wrong.
     */
    invalid_import::invalid_import(const token& tok, const std::string& message) : std::runtime_error(message), m_tok(tok) {
    }
    
    /**
     * what
     * override the what function defined by runtime_error
     */
    const char * invalid_import::what() const noexcept {
        return std::runtime_error::what();
    }

    /**
     * get_token
     * returns the token corresponding to the import that failed checking
     */
    const token& invalid_import::get_token() {
        return m_tok;
    }
}
