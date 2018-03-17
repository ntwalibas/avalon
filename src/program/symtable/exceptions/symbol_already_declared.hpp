#ifndef AVALON_PROGRAM_SYMTABLE_SYMBOL_ALREADY_DECLARED_HPP_
#define AVALON_PROGRAM_SYMTABLE_SYMBOL_ALREADY_DECLARED_HPP_

#include <stdexcept>


namespace avalon {
    class symbol_already_declared : public std::runtime_error {
        public:
            symbol_already_declared(const std::string& message);
            virtual const char * what() const noexcept;
    };
}

#endif
