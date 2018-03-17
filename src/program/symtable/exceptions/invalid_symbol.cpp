#include <stdexcept>

#include "program/symtable/exceptions/invalid_symbol.hpp"

namespace avalon {
    invalid_symbol::invalid_symbol(const std::string& message) : std::runtime_error(message) {
    }

    const char * invalid_symbol::what() const noexcept {
        return std::runtime_error::what();
    }
}
