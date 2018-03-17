#include <stdexcept>

#include "compiler/exceptions/invalid_directory.hpp"


namespace avalon {
    invalid_directory::invalid_directory(const std::string& message) : std::runtime_error(message) {
    }

    const char * invalid_directory::what() const noexcept {
        return std::runtime_error::what();
    }
}
