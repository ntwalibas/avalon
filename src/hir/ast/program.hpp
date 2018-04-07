#ifndef AVALON_PROGRAM_AST_PROG_H_
#define AVALON_PROGRAM_AST_PROG_H_

#include <memory>
#include <vector>

/* Symtable */
#include "hir/symtable/scope.hpp"
#include "hir/symtable/fqn.hpp"

/* AST */
#include "hir/ast/decl/decl.hpp"
#include "hir/ast/program.hpp"


namespace avalon {
    class program {
    public:
        /**
         * the default contructor expects the scope bound to the program
         */
        program();

        /**
         * set_fqn
         * sets the fqn within which this program lives
         */
        void set_fqn(fqn& l_fqn);

        /**
         * get_fqn
         * returns the fqn within which this program lives
         */
        fqn& get_fqn();

        /**
         * set_scope
         * specify the scope bound to this program
         */
        void set_scope(std::shared_ptr<scope>& l_scope);

        /**
         * get_scope
         * return the scope bound to this program
         */
        std::shared_ptr<scope>& get_scope();

        /**
         * add_declaration
         * a program is made of different types of declarations:
         * - import declarations
         * - namespace declarations
         * 
         * a program sits at the top of the AST and it holds a series of those declarations.
         */
        void add_declaration(std::shared_ptr<decl>& declaration);

        /**
         * get_declarations
         * for futher processing, this function is called to get all the declarations that
         * make up a program.
         */
        std::vector<std::shared_ptr<decl> >& get_declarations();

    private:
        /*
         * the fqn to this program
         */
        fqn m_fqn;

        /*
         * scope introduced by the program, i.e. global scope
         */
        std::shared_ptr<scope> m_scope;

        /*
         * a vector of declarations representing declarations as they were found
         */
        std::vector<std::shared_ptr<decl> > m_declarations;
    };
}

#endif
