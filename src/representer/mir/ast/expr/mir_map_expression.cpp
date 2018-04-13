#include <utility>
#include <memory>
#include <string>

#include "representer/mir/ast/expr/mir_map_expression.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    mir_map_expression::mir_map_expression(token& tok, mir_type& l_type) : m_tok(tok), m_type(l_type) {
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_map_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_type
     * returns the type of this expression
     */
    mir_type& mir_map_expression::get_type() {
        return m_type;
    }

    /**
     * add_element
     * add a new key/value pair to the map
     */
    void mir_map_expression::add_element(std::shared_ptr<mir_expr>& key, std::shared_ptr<mir_expr>& value) {
        m_els.emplace_back(key, value);
    }

    /**
     * get_elements
     * returns a vector of all the elements in the map
     */
    std::vector<std::pair<std::shared_ptr<mir_expr>, std::shared_ptr<mir_expr> > >& mir_map_expression::get_elements() {
        return m_els;
    }
}
