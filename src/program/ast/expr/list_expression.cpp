#include <string>

#include "program/ast/expr/list_expression.hpp"
#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    list_expression::list_expression(token& tok) : m_tok(tok) {
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& list_expression::get_token() const {
        return m_tok;
    }

    /**
     * set_type_instance
     * sets the type instance of this expression
     */
    void list_expression::set_type_instance(type_instance& instance) {
        m_instance = instance;
    }

    /**
     * get_type_instance
     * returns the type instance of this expression
     */
    type_instance& list_expression::get_type_instance() {
        return m_instance;
    }

    /**
     * has_type_instance
     * returns true if the type instance is not star
     */
    bool list_expression::has_type_instance() {
        if(m_instance.get_name() == "*" && m_instance.get_namespace() == "*")
            return true;
        else
            return false;
    }

    /**
     * add_element
     * add a new element to the list
     */
    void list_expression::add_element(std::shared_ptr<expr> element) {
        m_elements.push_back(element);
    }

    /**
     * get_elements
     * returns a vector of all the elements in the list
     */
    std::vector<std::shared_ptr<expr> >& list_expression::get_elements() {
        return m_elements;
    }
}
