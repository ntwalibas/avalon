#include <stdexcept>

#include "representer/exceptions/symbol_can_collide.hpp"

namespace avalon {
    symbol_can_collide::symbol_can_collide(const std::string& message) : std::runtime_error(message) {
    }

    const char * symbol_can_collide::what() const noexcept {
        return std::runtime_error::what();
    }
}
