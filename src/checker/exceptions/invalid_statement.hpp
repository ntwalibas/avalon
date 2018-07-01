#ifndef AVALON_CHECKER_EXCEPTIONS_INVALID_STATEMENT_H_
#define AVALON_CHECKER_EXCEPTIONS_INVALID_STATEMENT_H_

#include <stdexcept>

#include "lexer/token.hpp"


namespace avalon {
    class invalid_statement : public std::runtime_error {
    public:
        /**
         * the constructor expects the statement name token
         * and the message about what exactly went wrong.
         */
        invalid_statement(const token& tok, const std::string& message);

        /**
         * what
         * override the what function defined by runtime_error
         */
        virtual const char * what() const noexcept;

        /**
         * get_token
         * returns the token to the statement name in the source code
         */
        const token& get_token();

    private:
        /**
         * when a statement fails statement checking,
         * we need the token to the statement name in the source code
         */
        token m_tok;
    };
}

#endif
