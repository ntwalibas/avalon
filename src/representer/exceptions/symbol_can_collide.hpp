#ifndef AVALON_HIR_SYMTABLE_SYMBOL_CAN_COLLIDE_HPP_
#define AVALON_HIR_SYMTABLE_SYMBOL_CAN_COLLIDE_HPP_

#include <stdexcept>


namespace avalon {
    class symbol_can_collide : public std::runtime_error {
    public:
        symbol_can_collide(const std::string& message);
        virtual const char * what() const noexcept;
    };
}

#endif
