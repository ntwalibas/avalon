#include <stdexcept>
#include <string>

/* Error */
#include "error/error.hpp"

/* Lexer */
#include "lexer/token.hpp"

/* Error display */
#include "checker/check_error.hpp"


namespace avalon {
    /**
     * the contructor expects:
     * - the error handler to be used to show errors
     * - the token where the error occured
     * - the message to show the user
     */
    check_error::check_error(error& error_handler, token tok, bool fatal, const std::string& message) : std::runtime_error(message), m_error_handler(error_handler), m_tok(tok), m_fatal(fatal) {
    }
    
    /**
     * what
     * get the error message
     */
    const char * check_error::what() const noexcept {
        return std::runtime_error::what();
    }

    /**
     * is_fatal
     * returns true if the error is fatal, false otherwise
     */
    bool check_error::is_fatal() {
        return m_fatal;
    }

    /**
     * show
     * calls the error handler error reporting function to display the error
     */
    void check_error::show() {
        m_error_handler.log(m_tok . get_line(), m_tok . get_column(), what());
    }
}
