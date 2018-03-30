#include <utility>
#include <memory>
#include <string>

#include "program/ast/expr/map_constructor_expression.hpp"
#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    map_constructor_expression::map_constructor_expression(token& tok) : m_tok(tok) {
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& map_constructor_expression::get_token() const {
        return m_tok;
    }

    /**
     * set_type_instance
     * sets the type instance of this expression
     */
    void map_constructor_expression::set_type_instance(type_instance& instance) {
        m_instance = instance;
    }

    /**
     * get_type_instance
     * returns the type instance of this expression
     */
    type_instance& map_constructor_expression::get_type_instance() {
        return m_instance;
    }

    /**
     * has_type_instance
     * returns true if the type instance is not star
     */
    bool map_constructor_expression::has_type_instance() {
        if(m_instance.get_name() == "*" && m_instance.get_namespace() == "*")
            return true;
        else
            return false;
    }

    /**
     * add_element
     * add a new key/value pair to the map
     */
    void map_constructor_expression::add_element(std::shared_ptr<expr>& key, std::shared_ptr<expr>& value) {
        m_elements.emplace_back(key, value);
    }

    /**
     * get_elements
     * returns a vector of all the elements in the map
     */
    std::vector<std::pair<std::shared_ptr<expr>, std::shared_ptr<expr> > >& map_constructor_expression::get_elements() {
        return m_elements;
    }
}
