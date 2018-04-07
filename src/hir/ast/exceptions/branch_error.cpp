#include <stdexcept>

#include "hir/ast/exceptions/branch_error.hpp"

namespace avalon {
    branch_error::branch_error(const std::string& message) : std::runtime_error(message) {
    }

    const char * branch_error::what() const noexcept {
        return std::runtime_error::what();
    }
}
