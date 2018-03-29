#ifndef AVALON_CHECKER_EXCEPTIONS_INVALID_BLOCK_H_
#define AVALON_CHECKER_EXCEPTIONS_INVALID_BLOCK_H_

#include <stdexcept>
#include <string>


namespace avalon {
    class invalid_block : public std::runtime_error {
    public:
        /**
         * the block expects the message about what exactly went wrong.
         */
        invalid_block(const std::string& message);

        /**
         * what
         * override the what function defined by runtime_error
         */
        virtual const char * what() const noexcept;
    };
}

#endif
