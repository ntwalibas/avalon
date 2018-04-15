#ifndef AVALON_BUILTINS_VOID_HPP_
#define AVALON_BUILTINS_VOID_HPP_

#include <memory>

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "representer/hir/ast/decl/type.hpp"
#include "representer/hir/ast/program.hpp"


namespace avalon {
    class avalon_void {
    public:
        /**
         * the default constructor expects nothing
         */
        avalon_void();

        /**
         * get_type
         * returns the void type
         */
        std::shared_ptr<type>& get_type();

        /**
         * get_type_instance
         * returns the void type instance
         */
        type_instance& get_type_instance();

        /**
         * get_program
         * returns a program that defines the void type and all functions that operate on voids
         */
        program& get_program();

    private:
        /*
         * the void type
         */
        std::shared_ptr<type> m_void_type;

        /*
         * the type instance built from the void type
         */
        type_instance m_void_instance;

        /*
         * the program with voidean functions and type
         */
        program m_void_prog;
    };
}

#endif
