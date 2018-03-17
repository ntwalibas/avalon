#include <cstddef>
#include <memory>

#include "program/ast/decl/decl.hpp"
#include "program/ast/decl/ns.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    ns::ns(token tok) : m_name(tok.get_lexeme()), m_tok(tok) {        
    }

    /**
     * get_name
     * returns the name of the namespace
     */
    const std::string& ns::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns a token with namespace source information
     */
    const token& ns::get_token() const {
        return m_tok;
    }
    
    /**
     * add_declaration
     * a namespace can contain the following declarations:
     * - type declarations
     * - function declarations
     * - variable declarations
     */
    void ns::add_declaration(std::shared_ptr<decl>& declaration) {
        m_declarations.push_back(declaration);
    }

    /**
     * get_declarations
     * for futher processing, this function is called to get all the declarations that can be found in the namespace
     */
    std::vector<std::shared_ptr<decl> >& ns::get_declarations() {
        return m_declarations;
    }
}
