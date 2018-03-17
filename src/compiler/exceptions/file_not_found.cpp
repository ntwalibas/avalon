#include <stdexcept>

#include "compiler/exceptions/file_not_found.hpp"


namespace avalon {
    file_not_found::file_not_found(const std::string& message) : std::runtime_error(message) {
    }

    const char * file_not_found::what() const noexcept {
        return std::runtime_error::what();
    }
}
