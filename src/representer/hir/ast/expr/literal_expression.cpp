#include <memory>

#include "representer/hir/ast/expr/literal_expression.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the operand of the literal operator
     */
    literal_expression::literal_expression(token& tok, literal_expression_type expr_type, const std::string& val) : m_tok(tok), m_type_instance_from_parser(false), m_expr_type(expr_type), m_val(val) {
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& literal_expression::get_token() const {
        return m_tok;
    }

    /**
     * set_type_instance
     * sets the type instance of this expression
     */
    void literal_expression::set_type_instance(type_instance& instance) {
        m_instance = instance;
    }

    /**
     * set_type_instance
     * sets the type instance of this expression with the possibility of specifying
     * if the parser (true case) set the type instance or the inference engine (false case)
     */
    void literal_expression::set_type_instance(type_instance& instance, bool from_parser) {
        m_instance = instance;
        m_type_instance_from_parser = from_parser;
    }

    /**
     * get_type_instance
     * returns the type instance of this expression
     */
    type_instance& literal_expression::get_type_instance() {
        return m_instance;
    }

    /**
     * has_type_instance
     * returns true if the type instance is not star
     */
    bool literal_expression::has_type_instance() {
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
    bool literal_expression::type_instance_from_parser() const {
        return m_type_instance_from_parser;
    }

    /**
     * get_expression_type
     * returns the type of literal this expression holds
     */
    const literal_expression_type& literal_expression::get_expression_type() const {
        return m_expr_type;
    }

    /**
     * get_value
     * returns the literal operator operand
     */
    const std::string& literal_expression::get_value() const {
        return m_val;
    }
}
