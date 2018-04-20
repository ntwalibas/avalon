#ifndef AVALON_BUILTINS_LIST_HPP_
#define AVALON_BUILTINS_LIST_HPP_

#include <memory>

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "representer/hir/ast/decl/type.hpp"
#include "representer/hir/ast/program.hpp"


namespace avalon {
    class avalon_list {
    public:
        /**
         * the default constructor expects nothing
         */
        avalon_list(type_instance& param);

        /**
         * get_type
         * returns the list type
         */
        std::shared_ptr<type>& get_type();

        /**
         * get_type_instance
         * returns the list type instance
         */
        type_instance& get_type_instance();

        /**
         * get_program
         * returns a program that defines the list type and all functions that operate on lists
         */
        program& get_program();

    private:
        /*
         * the list type
         */
        std::shared_ptr<type> m_list_type;

        /*
         * the type instance built from the list type
         */
        type_instance m_list_instance;

        /*
         * the program with listean functions and type
         */
        program m_list_prog;
    };
}

#endif
