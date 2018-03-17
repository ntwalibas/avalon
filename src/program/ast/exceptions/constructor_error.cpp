#include <stdexcept>

#include "program/ast/exceptions/constructor_error.hpp"

namespace avalon {
    constructor_error::constructor_error(const std::string& message) : std::runtime_error(message) {
    }

    const char * constructor_error::what() const noexcept {
        return std::runtime_error::what();
    }
}
