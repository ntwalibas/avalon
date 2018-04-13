#include <string>

#include "lexer/token.hpp"
#include "representer/hir/ast/decl/import.hpp"


namespace avalon {
/**
 * the constructor expects the import token and the fqn to import declaration from
 */
import::import(token tok, const std::string& fqn_name) : m_tok(tok), m_fqn_name(fqn_name) {
}

    /**
     * get_token
     * returns a token with import source information
     */
    const token& import::get_token() const {
        return m_tok;
    }

    /**
     * get_name
     * returns the fully qualified name of the module to import
     */
    std::string& import::get_fqn_name() {
        return m_fqn_name;
    }
}
