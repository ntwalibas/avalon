#include <memory>

#include "program/ast/expr/unary_expression.hpp"
#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the type of unary expression and the operand of the unary operator
     */
    unary_expression::unary_expression(token& tok, unary_expression_type expr_type, std::shared_ptr<expr> val) : m_tok(tok), m_expr_type(expr_type), m_val(val) {
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& unary_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_expression_type
     * returns the type of expression we are dealing
     */
    const unary_expression_type& unary_expression::get_expression_type() const {
        return m_expr_type;
    }

    /**
     * set_type_instance
     * sets the type instance of this expression
     */
    void unary_expression::set_type_instance(type_instance& instance) {
        m_instance = instance;
    }

    /**
     * has_type_instance
     * returns true if the type instance is not star
     */
    bool unary_expression::has_type_instance() {
        if(m_instance.get_name() == "*" && m_instance.get_namespace() == "*")
            return true;
        else
            return false;
    }

    /**
     * get_type_instance
     * returns the type instance of this expression
     */
    type_instance& unary_expression::get_type_instance() {
        return m_instance;
    }

    /**
     * get_val
     * returns the unary operator operand
     */
    std::shared_ptr<expr>& unary_expression::get_val() {
        return m_val;
    }
}
