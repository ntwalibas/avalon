#ifndef AVALON_PROGRAM_AST_TYPE_ERROR_HPP_
#define AVALON_PROGRAM_AST_TYPE_ERROR_HPP_

#include <stdexcept>


namespace avalon {
    class type_error : public std::runtime_error {
        public:
            type_error(const std::string& message);
            virtual const char * what() const noexcept;
    };
}

#endif
