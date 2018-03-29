#include <string>

#include "program/ast/expr/tuple_expression.hpp"
#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    tuple_expression::tuple_expression(token& tok) : m_tok(tok) {
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& tuple_expression::get_token() const {
        return m_tok;
    }

    /**
     * set_type_instance
     * sets the type instance of this expression
     */
    void tuple_expression::set_type_instance(type_instance& instance) {
        m_instance = instance;
    }

    /**
     * get_type_instance
     * returns the type instance of this expression
     */
    type_instance& tuple_expression::get_type_instance() {
        return m_instance;
    }

    /**
     * add_element
     * add a new element to the tuple
     */
    void tuple_expression::add_element(std::shared_ptr<expr> element) {
        m_elements.push_back(element);
    }

    /**
     * get_elements
     * returns a vector of all the elements in the tuple
     */
    std::vector<std::shared_ptr<expr> >& tuple_expression::get_elements() {
        return m_elements;
    }
}
