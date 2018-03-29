#include <string>

#include "program/ast/expr/list_expression.hpp"
#include "program/ast/expr/expr.hpp"
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
