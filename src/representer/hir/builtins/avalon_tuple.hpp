#ifndef AVALON_BUILTINS_TUPLE_HPP_
#define AVALON_BUILTINS_TUPLE_HPP_

#include <memory>

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "representer/hir/ast/decl/type.hpp"
#include "representer/hir/ast/program.hpp"


namespace avalon {
    class avalon_tuple {
    public:
        /**
         * the default constructor expects nothing
         */
        avalon_tuple();

        /**
         * get_type
         * returns the tuple type
         */
        std::shared_ptr<type>& get_type();

        /**
         * get_type_instance
         * returns the tuple type instance
         */
        type_instance get_type_instance();

        /**
         * get_program
         * returns a program that defines the tuple type and all functions that operate on tuples
         */
        program& get_program();

    private:
        /*
         * the tuple type
         */
        std::shared_ptr<type> m_tuple_type;

        /*
         * the type instance built from the tuple type
         */
        type_instance m_tuple_instance;

        /*
         * the program with tupleean functions and type
         */
        program m_tuple_prog;
    };
}

#endif
