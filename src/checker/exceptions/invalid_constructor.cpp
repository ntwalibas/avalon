#include <stdexcept>

#include "checker/exceptions/invalid_constructor.hpp"

namespace avalon {
    invalid_constructor::invalid_constructor(const std::string& message) : std::runtime_error(message) {
    }

    const char * invalid_constructor::what() const noexcept {
        return std::runtime_error::what();
    }
}
