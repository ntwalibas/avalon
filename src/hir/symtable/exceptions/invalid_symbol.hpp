#ifndef AVALON_PROGRAM_SYMTABLE_INVALID_SYMBOL_HPP_
#define AVALON_PROGRAM_SYMTABLE_INVALID_SYMBOL_HPP_

#include <stdexcept>


namespace avalon {
    class invalid_symbol : public std::runtime_error {
        public:
            invalid_symbol(const std::string& message);
            virtual const char * what() const noexcept;
    };
}

#endif
