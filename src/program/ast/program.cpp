#include <memory>
#include <vector>

/* Symtable */
#include "program/symtable/scope.hpp"
#include "program/symtable/fqn.hpp"

/* AST */
#include "program/ast/decl/decl.hpp"
#include "program/ast/program.hpp"


namespace avalon {
/**
 * the default contructor expects nothing
 */
program::program() : m_scope(nullptr) {
}

    /**
     * set_fqn
     * sets the fqn within which this program lives
     */
    void program::set_fqn(fqn& l_fqn) {
        m_fqn = l_fqn;
    }

    /**
     * get_fqn
     * returns the fqn within which this program lives
     */
    fqn& program::get_fqn() {
        return m_fqn;
    }

    /**
     * set_scope
     * specify the scope bound to this program
     */
    void program::set_scope(std::shared_ptr<scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope bound to this program
     */
    std::shared_ptr<scope>& program::get_scope() {
        return m_scope;
    }
    
    /**
     * add_declaration
     * a program is made of different types of declarations:
     * - import declarations
     * - namespace declarations
     */
    void program::add_declaration(std::shared_ptr<decl>& declaration) {
        m_declarations.push_back(declaration);
    }

    /**
     * get_declarations
     * for futher processing, this function is called to get all the declarations that
     * make up a program.
     */
    std::vector<std::shared_ptr<decl> >& program::get_declarations() {
        return m_declarations;
    }
}
