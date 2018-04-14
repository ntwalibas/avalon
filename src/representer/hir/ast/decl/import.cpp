#include <string>

#include "representer/hir/ast/decl/import.hpp"
#include "representer/hir/symtable/fqn.hpp"
#include "lexer/token.hpp"


namespace avalon {
/**
 * the constructor expects the import token and the fqn to import declaration from
 */
import::import(token& tok, const std::string& fqn_name) : m_tok(tok), m_fqn_name(fqn_name) {
}

    /**
     * get_token
     * returns a token with import source information
     */
    const token& import::get_token() const {
        return m_tok;
    }

    /**
     * set_fqn
     * sets the fqn where this namespace was found
     */
    void import::set_fqn(fqn& l_fqn) {
        m_fqn = l_fqn;
    }

    /**
     * get_fqn
     * returns the fqn where this namespace was found
     */
    fqn& import::get_fqn() {
        return m_fqn;
    }

    /**
     * get_name
     * returns the fully qualified name of the module to import
     */
    std::string& import::get_fqn_name() {
        return m_fqn_name;
    }
}
