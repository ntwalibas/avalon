#include <stdexcept>

#include "representer/exceptions/symbol_already_declared.hpp"

namespace avalon {
    symbol_already_declared::symbol_already_declared(const std::string& message) : std::runtime_error(message) {
    }

    const char * symbol_already_declared::what() const noexcept {
        return std::runtime_error::what();
    }
}
