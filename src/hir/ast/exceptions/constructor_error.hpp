#ifndef AVALON_PROGRAM_AST_CONSTRUCTOR_ERROR_HPP_
#define AVALON_PROGRAM_AST_CONSTRUCTOR_ERROR_HPP_

#include <stdexcept>


namespace avalon {
    class constructor_error : public std::runtime_error {
        public:
            constructor_error(const std::string& message);
            virtual const char * what() const noexcept;
    };
}

#endif
