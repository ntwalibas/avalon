#include <memory>

#include "program/ast/expr/unary_expression.hpp"
#include "program/ast/expr/expr.hpp"
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
     * get_val
     * returns the unary operator operand
     */
    std::shared_ptr<expr>& unary_expression::get_val() {
        return m_val;
    }
}
