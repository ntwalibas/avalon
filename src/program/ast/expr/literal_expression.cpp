#include <memory>

#include "program/ast/expr/literal_expression.hpp"
#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the operand of the literal operator
     */
    literal_expression::literal_expression(token& tok, literal_expression_type expr_type, const std::string& val) : m_tok(tok), m_expr_type(expr_type), m_val(val) {
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& literal_expression::get_token() const {
        return m_tok;
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
