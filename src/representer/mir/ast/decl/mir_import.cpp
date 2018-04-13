#include <string>

#include "representer/mir/ast/decl/mir_import.hpp"
#include "representer/mir/ast/decl/mir_decl.hpp"
#include "representer/hir/symtable/fqn.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the import token and the fqn to import declaration from
     */
    mir_import::mir_import(token& tok, const std::string& fqn_name) : m_tok(tok), m_fqn_name(fqn_name) {
    }

    /**
     * get_token
     * returns a token with import source information
     */
    const token& mir_import::get_token() const {
        return m_tok;
    }

    /**
     * get_name
     * returns the fully qualified name of the module to import
     */
    const std::string& mir_import::get_fqn_name() {
        return m_fqn_name;
    }

    /**
     * set_fqn
     * sets the fqn where this declaration was found
     */
    void mir_import::set_fqn(fqn& l_fqn) {
        m_fqn = l_fqn;
    }

    /**
     * get_fqn
     * returns the fqn where this declaration was found
     */
    fqn& mir_import::get_fqn() {
        return m_fqn;
    }
}
