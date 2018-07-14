#ifndef AVALON_BUILTINS_MAP_HPP_
#define AVALON_BUILTINS_MAP_HPP_

#include <memory>

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "representer/hir/ast/decl/type.hpp"
#include "representer/hir/ast/program.hpp"


namespace avalon {
    class avalon_map {
    public:
        /**
         * the default constructor expects nothing
         */
        avalon_map(type_instance& key_instance, type_instance& value_instance);

        /**
         * get_type
         * returns the map type
         */
        std::shared_ptr<type>& get_type();

        /**
         * get_type_instance
         * returns the map type instance
         */
        type_instance get_type_instance();

        /**
         * get_program
         * returns a program that defines the map type and all functions that operate on maps
         */
        program& get_program();

    private:
        /*
         * the map type
         */
        std::shared_ptr<type> m_map_type;

        /*
         * the type instance built from the map type
         */
        type_instance m_map_instance;

        /*
         * the program with mapean functions and type
         */
        program m_map_prog;
    };
}

#endif
