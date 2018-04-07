#ifndef AVALON_PROGRAM_AST_CASE_ERROR_HPP_
#define AVALON_PROGRAM_AST_CASE_ERROR_HPP_

#include <stdexcept>


namespace avalon {
    class case_error : public std::runtime_error {
        public:
            case_error(const std::string& message);
            virtual const char * what() const noexcept;
    };
}

#endif
