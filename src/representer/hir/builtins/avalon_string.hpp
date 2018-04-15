#ifndef AVALON_BUILTINS_STRING_HPP_
#define AVALON_BUILTINS_STRING_HPP_

#include <memory>

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "representer/hir/ast/decl/type.hpp"
#include "representer/hir/ast/program.hpp"


namespace avalon {
    class avalon_string {
    public:
        /**
         * the default constructor expects nothing
         */
        avalon_string();

        /**
         * get_type
         * returns the string type
         */
        std::shared_ptr<type>& get_type();

        /**
         * get_type_instance
         * returns the string type instance
         */
        type_instance& get_type_instance();

        /**
         * get_program
         * returns a program that defines the string type and all functions that operate on strings
         */
        program& get_program();

    private:
        /*
         * the string type
         */
        std::shared_ptr<type> m_string_type;

        /*
         * the type instance built from the string type
         */
        type_instance m_string_instance;

        /*
         * the program with stringean functions and type
         */
        program m_string_prog;
    };
}

#endif
