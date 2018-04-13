#include <utility>
#include <memory>
#include <vector>
#include <string>

#include "representer/mir/ast/expr/mir_tuple_expression.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    mir_tuple_expression::mir_tuple_expression(token& tok, mir_type& l_type) : m_tok(tok), m_type(l_type) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_tuple_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_type
     * returns the type of this expression
     */
    mir_type& mir_tuple_expression::get_type() {
        return m_type;
    }

    /**
     * add_element
     * add a new element to the tuple
     */
    void mir_tuple_expression::add_element(const std::string& el_name, std::shared_ptr<mir_expr> el_val) {
        m_els.emplace_back(el_name, el_val);
    }

    /**
     * get_elements
     * returns a map of all the elements in the tuple
     */
    std::vector<std::pair<std::string, std::shared_ptr<mir_expr> > >& mir_tuple_expression::get_elements() {
        return m_els;
    }
}
