#include <utility>
#include <memory>
#include <string>

#include "representer/hir/ast/expr/map_expression.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    map_expression::map_expression(token& tok) : m_tok(tok), m_type_instance_from_parser(false) {
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& map_expression::get_token() const {
        return m_tok;
    }

    /**
     * set_type_instance
     * sets the type instance of this expression
     */
    void map_expression::set_type_instance(type_instance& instance) {
        m_instance = instance;
    }

    /**
     * set_type_instance
     * sets the type instance of this expression with the possibility of specifying
     * if the parser (true case) set the type instance or the inference engine (false case)
     */
    void map_expression::set_type_instance(type_instance& instance, bool from_parser) {
        m_instance = instance;
        m_type_instance_from_parser = from_parser;
    }

    /**
     * get_type_instance
     * returns the type instance of this expression
     */
    type_instance& map_expression::get_type_instance() {
        return m_instance;
    }

    /**
     * has_type_instance
     * returns true if the type instance is not star
     */
    bool map_expression::has_type_instance() {
        if(m_instance.get_name() == "*")
            return false;
        else
            return true;
    }

    /**
     * type_instance_from_parser
     * returns true if the type instance was set by the parser
     * this will happen when the user specifies a type directly on an expression
     */
    bool map_expression::type_instance_from_parser() const {
        return m_type_instance_from_parser;
    }

    /**
     * add_element
     * add a new key/value pair to the map
     */
    void map_expression::add_element(std::shared_ptr<expr>& key, std::shared_ptr<expr>& value) {
        m_elements.emplace_back(key, value);
    }

    /**
     * get_elements
     * returns a vector of all the elements in the map
     */
    std::vector<std::pair<std::shared_ptr<expr>, std::shared_ptr<expr> > >& map_expression::get_elements() {
        return m_elements;
    }
}
