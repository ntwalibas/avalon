#include <utility>
#include <memory>
#include <vector>
#include <string>

/* Symtable */
#include "representer/mir/symtable/mir_scope.hpp"
#include "representer/hir/symtable/fqn.hpp"

/* AST */
#include "representer/mir/ast/decl/mir_decl.hpp"

/* Program */
#include "representer/mir/ast/mir_program.hpp"


namespace avalon {
    /**
     * the default contructor expects nothing
     */
    mir_program::mir_program() {        
    }

    /**
     * set_fqn
     * sets the fqn within which this program lives
     */
    void mir_program::set_fqn(fqn& l_fqn) {
        m_fqn = l_fqn;
    }

    /**
     * get_fqn
     * returns the fqn within which this program lives
     */
    fqn& mir_program::get_fqn() {
        return m_fqn;
    }

    /**
     * set_scope
     * specify the scope bound to this program
     */
    void mir_program::set_scope(std::shared_ptr<mir_scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope bound to this program
     */
    std::shared_ptr<mir_scope>& mir_program::get_scope() {
        return m_scope;
    }

    /**
     * add_declaration
     * a program is made of different types of declarations:
     * - import declarations
     * - namespace declarations
     * 
     * a program sits at the top of the AST and it holds a series of those declarations.
     */
    void mir_program::add_declaration(const std::string& decl_name, std::shared_ptr<mir_decl>& declaration) {
        for(auto& hir_decl : m_hir_declarations) {
            if(hir_decl.first == decl_name)
                return;
        }

        m_hir_declarations.emplace_back(decl_name, declaration);
    }

    /**
     * get_declarations
     * for futher processing, this function is called to get all the declarations that
     * make up a program.
     */
    std::vector<std::shared_ptr<mir_decl> >& mir_program::get_declarations() {
        return m_declarations;
    }

    /**
     * flatten
     * transforms the MIR-MIR map into the MIR vector
     */
    void mir_program::flatten() {
        for(auto& hir_decl : m_hir_declarations)
            m_declarations.push_back(hir_decl.second);
    }
}
