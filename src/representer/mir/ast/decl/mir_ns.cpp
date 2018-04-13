#include <utility>
#include <memory>
#include <vector>
#include <string>

#include "representer/mir/ast/decl/mir_decl.hpp"
#include "representer/mir/ast/decl/mir_ns.hpp"
#include "representer/hir/symtable/fqn.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    mir_ns::mir_ns(const std::string& name, token& tok) : m_name(name), m_tok(tok), m_is_flat(false) {
    }

    /**
     * get_name
     * returns the name of the namespace
     */
    const std::string& mir_ns::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns a token with the namespace source information
     */
    const token& mir_ns::get_token() const {
        return m_tok;
    }

    /**
     * set_fqn
     * sets the fqn where this declaration was found
     */
    void mir_ns::set_fqn(fqn& l_fqn) {
        m_fqn = l_fqn;
    }

    /**
     * get_fqn
     * returns the fqn where this declaration was found
     */
    fqn& mir_ns::get_fqn() {
        return m_fqn;
    }

    /**
     * add_hir_declaration
     * given an HIR declaration name, create the corresponding entry in the HIR vector
     */
    void mir_ns::add_hir_declaration(const std::string& decl_name) {
        // if the HIR declaration already exists, we return early
        for(auto& hir_decl : m_hir_declarations) {
            if(hir_decl.first == decl_name)
                return;
        }

        // we have no such declaration name, we create a new one
        std::vector<std::shared_ptr<mir_decl> > mir_decls;
        m_hir_declarations.emplace_back(decl_name, mir_decls);
        m_is_flat = false;
    }

    /**
     * add_mir_declaration
     * given a MIR declaration name, add it to the HIR-MIR vector
     */
    void mir_ns::add_mir_declaration(const std::string& decl_name, std::shared_ptr<mir_decl>& declaration) {
        for(auto& hir_decl : m_hir_declarations) {
            if(hir_decl.first == decl_name) {
                std::vector<std::shared_ptr<mir_decl> >& mir_decls = hir_decl.second;
                mir_decls.push_back(declaration);
            }
        }
    }

    /**
     * get_declarations
     * for futher processing, this function is called to get all the declarations that can be found in the namespace
     */
    std::vector<std::shared_ptr<mir_decl> >& mir_ns::get_declarations() {
        if(!m_is_flat)
            flatten();
        return m_mir_declarations;
    }

    /**
     * flatten
     * transforms the MIR-MIR map into the MIR vector
     */
    void mir_ns::flatten() {
        // if the HIR-MIR vector has already been flattened, do nothing
        if(m_is_flat)
            return;

        // go over HIR-MIR declarations and insert the same into the MIR declarations vector
        for(auto& hir_decl : m_hir_declarations) {
            std::vector<std::shared_ptr<mir_decl> >& mir_decls = hir_decl.second;
            for(auto& mir_decl : mir_decls)
                m_mir_declarations.push_back(mir_decl);
        }

        // update the flattening flag
        m_is_flat = true;
    }
}
