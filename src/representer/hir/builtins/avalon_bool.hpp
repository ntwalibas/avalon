#ifndef AVALON_BUILTINS_BOOL_HPP_
#define AVALON_BUILTINS_BOOL_HPP_

#include <memory>

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "representer/hir/ast/decl/type.hpp"
#include "representer/hir/ast/program.hpp"


namespace avalon {
    class avalon_bool {
    public:
        /**
         * the default constructor expects nothing
         */
        avalon_bool();

        /**
         * get_type
         * returns the bool type
         */
        const std::shared_ptr<type>& get_type() const;

        /**
         * get_type_instance
         * returns the bool type instance
         */
        const type_instance& get_type_instance() const;

        /**
         * get_program
         * returns a program that defines the bool type and all functions that operate on bools
         */
        program& get_program();

    private:
        /*
         * the bool type
         */
        std::shared_ptr<type> m_bool_type;

        /*
         * the type instance built from the bool type
         */
        type_instance m_bool_instance;

        /*
         * the program with boolean functions and type
         */
        program m_bool_prog;
    };
}

#endif
