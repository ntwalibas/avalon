#ifndef AVALON_BUILTINS_FLOAT_HPP_
#define AVALON_BUILTINS_FLOAT_HPP_

#include <memory>

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "representer/hir/ast/decl/type.hpp"
#include "representer/hir/ast/program.hpp"


namespace avalon {
    class avalon_float {
    public:
        /**
         * the default constructor expects nothing
         */
        avalon_float();

        /**
         * get_type
         * returns the float type
         */
        std::shared_ptr<type>& get_type();

        /**
         * get_type_instance
         * returns the float type instance
         */
        type_instance& get_type_instance();

        /**
         * get_program
         * returns a program that defines the float type and all functions that operate on floats
         */
        program& get_program();

    private:
        /*
         * the float type
         */
        std::shared_ptr<type> m_float_type;

        /*
         * the type instance built from the float type
         */
        type_instance m_float_instance;

        /*
         * the program with floatean functions and type
         */
        program m_float_prog;
    };
}

#endif
