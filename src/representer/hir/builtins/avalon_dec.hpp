#ifndef AVALON_BUILTINS_DEC_HPP_
#define AVALON_BUILTINS_DEC_HPP_

#include <memory>

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "representer/hir/ast/decl/type.hpp"
#include "representer/hir/ast/program.hpp"


namespace avalon {
    class avalon_dec {
    public:
        /**
         * the default constructor expects nothing
         */
        avalon_dec();

        /**
         * get_type
         * returns the dec type
         */
        const std::shared_ptr<type>& get_type() const;

        /**
         * get_type_instance
         * returns the dec type instance
         */
        const type_instance& get_type_instance() const;

        /**
         * get_program
         * returns a program that defines the dec type and all functions that operate on decs
         */
        program& get_program();

    private:
        /*
         * the dec type
         */
        std::shared_ptr<type> m_dec_type;

        /*
         * the type instance built from the dec type
         */
        type_instance m_dec_instance;

        /*
         * the program with decean functions and type
         */
        program m_dec_prog;
    };
}

#endif
