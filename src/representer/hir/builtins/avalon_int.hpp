#ifndef AVALON_BUILTINS_INT_HPP_
#define AVALON_BUILTINS_INT_HPP_

#include <memory>

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "representer/hir/ast/decl/type.hpp"
#include "representer/hir/ast/program.hpp"


namespace avalon {
    class avalon_int {
    public:
        /**
         * the default constructor expects nothing
         */
        avalon_int();

        /**
         * get_type
         * returns the int type
         */
        const std::shared_ptr<type>& get_type() const;

        /**
         * get_type_instance
         * returns the int type instance
         */
        const type_instance& get_type_instance() const;

        /**
         * get_program
         * returns a program that defines the int type and all functions that operate on ints
         */
        program& get_program();

    private:
        /*
         * the int type
         */
        std::shared_ptr<type> m_int_type;

        /*
         * the type instance built from the int type
         */
        type_instance m_int_instance;

        /*
         * the program with intean functions and type
         */
        program m_int_prog;
    };
}

#endif
