#include <stdexcept>

#include "checker/exceptions/invalid_block.hpp"

namespace avalon {
    /**
     * the block expects the message about what exactly went wrong.
     */
    invalid_block::invalid_block(const std::string& message) : std::runtime_error(message) {
    }

    /**
     * what
     * override the what function defined by runtime_error
     */
    const char * invalid_block::what() const noexcept {
        return std::runtime_error::what();
    }
}
