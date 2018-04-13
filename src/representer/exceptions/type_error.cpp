#include <stdexcept>

#include "representer/exceptions/type_error.hpp"

namespace avalon {
    type_error::type_error(const std::string& message) : std::runtime_error(message) {
    }

    const char * type_error::what() const noexcept {
        return std::runtime_error::what();
    }
}
