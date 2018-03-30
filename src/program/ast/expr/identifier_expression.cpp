#include <string>

#include "program/ast/expr/identifier_expression.hpp"
#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    identifier_expression::identifier_expression(token& tok) : m_name(tok.get_lexeme()), m_tok(tok) {
    }

    /**
     * get_token
     * returns the name of the identifier
     */
    const std::string& identifier_expression::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& identifier_expression::get_token() const {
        return m_tok;
    }

    /**
     * set_type_instance
     * sets the type instance of this expression
     */
    void identifier_expression::set_type_instance(type_instance& instance) {
        m_instance = instance;
    }

    /**
     * get_type_instance
     * returns the type instance of this expression
     */
    type_instance& identifier_expression::get_type_instance() {
        return m_instance;
    }

    /**
     * has_type_instance
     * returns true if the type instance is not star
     */
    bool identifier_expression::has_type_instance() {
        if(m_instance.get_name() == "*" && m_instance.get_namespace() == "*")
            return true;
        else
            return false;
    }
}
