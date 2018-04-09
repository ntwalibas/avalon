#ifndef AVALON_HIR_AST_BRANCH_ERROR_HPP_
#define AVALON_HIR_AST_BRANCH_ERROR_HPP_

#include <stdexcept>


namespace avalon {
    class branch_error : public std::runtime_error {
        public:
            branch_error(const std::string& message);
            virtual const char * what() const noexcept;
    };
}

#endif
