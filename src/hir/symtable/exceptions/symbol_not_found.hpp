#ifndef AVALON_HIR_SYMTABLE_SYMBOL_NOT_FOUND_HPP_
#define AVALON_HIR_SYMTABLE_SYMBOL_NOT_FOUND_HPP_

#include <stdexcept>


namespace avalon {
    class symbol_not_found : public std::runtime_error {
        public:
            symbol_not_found(const std::string& message);
            virtual const char * what() const noexcept;
    };
}

#endif
