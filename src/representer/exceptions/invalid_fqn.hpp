#ifndef AVALON_HIR_SYMTABLE_INVALID_FQN_HPP_
#define AVALON_HIR_SYMTABLE_INVALID_FQN_HPP_

#include <stdexcept>


namespace avalon {
    class invalid_fqn : public std::runtime_error {
        public:
            invalid_fqn(const std::string& message);
            virtual const char * what() const noexcept;
    };
}

#endif
