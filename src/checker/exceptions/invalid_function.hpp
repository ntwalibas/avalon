#ifndef AVALON_CHECKER_EXCEPTIONS_INVALID_FUNCTION_HPP_
#define AVALON_CHECKER_EXCEPTIONS_INVALID_FUNCTION_HPP_

#include <stdexcept>

#include "lexer/token.hpp"


namespace avalon {
    class invalid_function : public std::runtime_error {
    public:
        /**
         * the constructor expects the function name token
         * and the message about what exactly went wrong.
         */
        invalid_function(const token& tok, const std::string& message);

        /**
         * what
         * override the what function defined by runtime_error
         */
        virtual const char * what() const noexcept;

        /**
         * get_token
         * returns the token to the function name in the source code
         */
        const token& get_token();

    private:
        /**
         * when a function fails function checking,
         * we need the token to the function name in the source code
         */
        token m_tok;
    };
}

#endif
