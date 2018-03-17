#ifndef AVALON_CHECKER_EXCEPTIONS_INVALID_TYPE_HPP_
#define AVALON_CHECKER_EXCEPTIONS_INVALID_TYPE_HPP_

#include <stdexcept>

#include "lexer/token.hpp"


namespace avalon {
    class invalid_type : public std::runtime_error {
    public:
        /**
         * the constructor expects the token of the type that resulted in failure
         * and the message about what exactly went wrong.
         */
        invalid_type(const token& tok, const std::string& message);

        /**
         * what
         * override the what function defined by runtime_error
         */
        virtual const char * what() const noexcept;

        /**
         * get_token
         * returns the token corresponding to the type that failed checking
         */
        const token& get_token();

    private:
        /**
         * token to the type name
         */
        token m_tok;
    };
}

#endif
