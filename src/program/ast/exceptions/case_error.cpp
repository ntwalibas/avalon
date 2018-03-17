#include <stdexcept>

#include "program/ast/exceptions/case_error.hpp"

namespace avalon {
    case_error::case_error(const std::string& message) : std::runtime_error(message) {
    }

    const char * case_error::what() const noexcept {
        return std::runtime_error::what();
    }
}
