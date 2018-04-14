#ifndef AVALON_BUILTINS_UNIT_HPP_
#define AVALON_BUILTINS_UNIT_HPP_

#include <memory>

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "representer/hir/ast/decl/type.hpp"
#include "representer/hir/ast/program.hpp"


namespace avalon {
    class avalon_unit {
    public:
        /**
         * the default constructor expects nothing
         */
        avalon_unit();

        /**
         * get_type
         * returns the unit type
         */
        const std::shared_ptr<type>& get_type() const;

        /**
         * get_type_instance
         * returns the unit type instance
         */
        const type_instance& get_type_instance() const;

        /**
         * get_program
         * returns a program that defines the unit type and all functions that operate on units
         */
        program& get_program();

    private:
        /*
         * the unit type
         */
        std::shared_ptr<type> m_unit_type;

        /*
         * the type instance built from the unit type
         */
        type_instance m_unit_instance;

        /*
         * the program with unitean functions and type
         */
        program m_unit_prog;
    };
}

#endif
