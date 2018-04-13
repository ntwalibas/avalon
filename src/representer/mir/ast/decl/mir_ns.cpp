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
    mir_ns::mir_ns(const std::string& name, token& tok) : m_name(name), m_tok(tok) {
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
     * add_declaration
     * a namespace can contain the following declarations:
     * - type declarations
     * - function declarations
     * - variable declarations
     */
    void mir_ns::add_declaration(std::shared_ptr<mir_decl>& declaration) {
        m_declarations.push_back(declaration);
    }

    /**
     * get_declarations
     * for futher processing, this function is called to get all the declarations that can be found in the namespace
     */
    std::vector<std::shared_ptr<mir_decl> >& mir_ns::get_declarations() {
        return m_declarations;
    }
}
