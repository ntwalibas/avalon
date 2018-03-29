#ifndef AVALON_CHECKER_EXCEPTIONS_INVALID_EXPRESSION_H_
#define AVALON_CHECKER_EXCEPTIONS_INVALID_EXPRESSION_H_

#include <stdexcept>

#include "lexer/token.hpp"


namespace avalon {
    class invalid_expression : public std::runtime_error {
    public:
        /**
         * the constructor expects the expression token token
         * and the message about what exactly went wrong.
         */
        invalid_expression(const token& tok, const std::string& message);

        /**
         * what
         * override the what expression defined by runtime_error
         */
        virtual const char * what() const noexcept;

        /**
         * get_token
         * returns the token to the expression token in the source code
         */
        const token& get_token();

    private:
        /**
         * when a expression fails expression checking,
         * we need the token to the expression token in the source code
         */
        token m_tok;
    };
}

#endif
