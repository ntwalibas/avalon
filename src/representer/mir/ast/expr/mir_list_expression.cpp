#include <string>

#include "representer/mir/ast/expr/mir_list_expression.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    mir_list_expression::mir_list_expression(token& tok, mir_type& l_type) : m_tok(tok), m_type(l_type) {

    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_list_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_type
     * returns the type of this expression
     */
    mir_type& mir_list_expression::get_type() {
        return m_type;
    }

    /**
     * add_element
     * add a new element to the list
     */
    void mir_list_expression::add_element(std::shared_ptr<mir_expr> el) {
        m_els.push_back(el);
    }

    /**
     * get_elements
     * returns a vector of all the elements in the list
     */
    std::vector<std::shared_ptr<mir_expr> >& mir_list_expression::get_elements() {
        return m_els;
    }
}
