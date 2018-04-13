#ifndef AVALON_MIR_AST_PROG_H_
#define AVALON_MIR_AST_PROG_H_

#include <memory>
#include <vector>

/* Symtable */
#include "representer/mir/symtable/mir_scope.hpp"
#include "representer/hir/symtable/fqn.hpp"

/* AST */
#include "representer/mir/ast/decl/mir_decl.hpp"

/* FQN */
#include "representer/hir/symtable/fqn.hpp"


namespace avalon {
    class mir_program {
    public:
        /**
         * the default contructor expects nothing
         */
        mir_program();

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
        void set_scope(std::shared_ptr<mir_scope>& l_scope);

        /**
         * get_scope
         * return the scope bound to this program
         */
        std::shared_ptr<mir_scope>& get_scope();

        /**
         * add_declaration
         * a program is made of different types of declarations:
         * - import declarations
         * - namespace declarations
         * 
         * a program sits at the top of the AST and it holds a series of those declarations.
         */
        void add_declaration(std::shared_ptr<mir_decl>& declaration);

        /**
         * get_declarations
         * for futher processing, this function is called to get all the declarations that
         * make up a program.
         */
        std::vector<std::shared_ptr<mir_decl> >& get_declarations();

    private:
        /*
         * the fqn to this program
         */
        fqn m_fqn;

        /*
         * scope introduced by the program, i.e. global scope
         */
        std::shared_ptr<mir_scope> m_scope;

        /*
         * a vector of MIR declarations
         */
        std::vector<std::shared_ptr<mir_decl> > m_declarations;
    };
}

#endif
