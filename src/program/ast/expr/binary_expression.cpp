#include <memory>

#include "program/ast/expr/binary_expression.hpp"
#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects:
     * - the type of binary expression this is
     * - the left operand to the binary operator
     * - and the right operand to the binary operator
     */
    binary_expression::binary_expression(token& tok, binary_expression_type expr_type, std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval) : m_tok(tok), m_expr_type(expr_type), m_lval(lval), m_rval(rval) {
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& binary_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_expression_type
     * returns the type of expression we are dealing
     */
    const binary_expression_type& binary_expression::get_expression_type() const {
        return m_expr_type;
    }

    /**
     * get_lval
     * returns the left operand to the binary operator
     */
    std::shared_ptr<expr>& binary_expression::get_lval() {
        return m_lval;
    }

    /**
     * get_rval
     * returns the right operand to the binary operator
     */
    std::shared_ptr<expr>& binary_expression::get_rval() {
        return m_rval;
    }
}
