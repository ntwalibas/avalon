#include <stdexcept>
#include <utility>
#include <memory>
#include <vector>
#include <string>

#include "representer/hir/ast/expr/tuple_expression.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    tuple_expression::tuple_expression(token& tok) : m_tok(tok), m_type_instance_from_parser(false) {
    }

    /**
     * copy constructor
     */
    tuple_expression::tuple_expression(const std::shared_ptr<tuple_expression>& tup_expr) : m_tok(tup_expr -> get_token()), m_instance(tup_expr -> get_type_instance()), m_type_instance_from_parser(tup_expr -> type_instance_from_parser()) {
        std::vector<std::pair<std::string, std::shared_ptr<expr> > >& elements = tup_expr -> get_elements();
        for(const auto& element : elements)
            m_elements.emplace_back(element.first, element.second -> copy());
    }

    /**
     * assignment copy operator
     */
    tuple_expression& tuple_expression::operator=(const std::shared_ptr<tuple_expression>& tup_expr) {
        m_tok = tup_expr -> get_token();
        m_instance = tup_expr -> get_type_instance();
        m_type_instance_from_parser = tup_expr -> type_instance_from_parser();
        std::vector<std::pair<std::string, std::shared_ptr<expr> > >& elements = tup_expr -> get_elements();
        for(const auto& element : elements)
            m_elements.emplace_back(element.first, element.second -> copy());
        return * this;
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
     * set_type_instance
     * sets the type instance of this expression with the possibility of specifying
     * if the parser (true case) set the type instance or the inference engine (false case)
     */
    void tuple_expression::set_type_instance(type_instance& instance, bool from_parser) {
        m_instance = instance;
        m_type_instance_from_parser = from_parser;
    }

    /**
     * get_type_instance
     * returns the type instance of this expression
     */
    type_instance& tuple_expression::get_type_instance() {
        return m_instance;
    }

    /**
     * has_type_instance
     * returns true if the type instance is not star
     */
    bool tuple_expression::has_type_instance() {
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
    bool tuple_expression::type_instance_from_parser() const {
        return m_type_instance_from_parser;
    }

    /**
     * add_element
     * add a new element to the tuple
     */
    void tuple_expression::add_element(const std::string& el_name, std::shared_ptr<expr> el_val) {
        for(auto& el : m_elements) {
            if(el.first == el_name)
                throw std::invalid_argument("The element name given in tuple expression is already in use.");
        }
        if(el_name == "*") {
            std::string new_el_name = std::to_string(m_elements.size());
            m_elements.emplace_back(new_el_name, el_val);
        }
        else {
            m_elements.emplace_back(el_name, el_val);
        }
    }

    /**
     * get_elements
     * returns a vector of all the elements in the tuple
     */
    std::vector<std::pair<std::string, std::shared_ptr<expr> > >& tuple_expression::get_elements() {
        return m_elements;
    }
}
