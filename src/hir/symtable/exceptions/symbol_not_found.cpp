#include <stdexcept>

#include "hir/symtable/exceptions/symbol_not_found.hpp"

namespace avalon {
    symbol_not_found::symbol_not_found(const std::string& message) : std::runtime_error(message) {
    }

    const char * symbol_not_found::what() const noexcept {
        return std::runtime_error::what();
    }
}
