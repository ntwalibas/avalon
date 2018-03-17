#ifndef AVALON_CHECKER_EXCEPTIONS_INVALID_CONSTRUCTOR_HPP_
#define AVALON_CHECKER_EXCEPTIONS_INVALID_CONSTRUCTOR_HPP_

#include <stdexcept>


namespace avalon {
    class invalid_constructor : public std::runtime_error {
    public:
        invalid_constructor(const std::string& message);
        virtual const char * what() const noexcept;
    };
}

#endif
