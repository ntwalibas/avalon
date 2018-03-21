#ifndef AVALON_CHECKER_EXCEPTIONS_INVALID_VARIABLE_H_
#define AVALON_CHECKER_EXCEPTIONS_INVALID_VARIABLE_H_

#include <stdexcept>

#include "lexer/token.hpp"


namespace avalon {
    class invalid_variable : public std::runtime_error {
    public:
        /**
         * the constructor expects the variable name token
         * and the message about what exactly went wrong.
         */
        invalid_variable(const token& tok, const std::string& message);

        /**
         * what
         * override the what variable defined by runtime_error
         */
        virtual const char * what() const noexcept;

        /**
         * get_token
         * returns the token to the variable name in the source code
         */
        const token& get_token();

    private:
        /**
         * when a variable fails variable checking,
         * we need the token to the variable name in the source code
         */
        token m_tok;
    };
}

#endif
