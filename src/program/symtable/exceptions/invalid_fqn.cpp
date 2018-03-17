#include <stdexcept>

#include "program/symtable/exceptions/invalid_fqn.hpp"


namespace avalon {
    invalid_fqn::invalid_fqn(const std::string& message) : std::runtime_error(message) {
    }

    const char * invalid_fqn::what() const noexcept {
        return std::runtime_error::what();
    }
}
