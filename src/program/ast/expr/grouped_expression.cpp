#include <memory>

#include "program/ast/expr/grouped_expression.hpp"
#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the operand of the grouped operator
     */
    grouped_expression::grouped_expression(token& tok, std::shared_ptr<expr>& val) : m_tok(tok), m_val(val) {
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& grouped_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_value
     * returns the grouped operator operand
     */
    std::shared_ptr<expr>& grouped_expression::get_value() {
        return m_val;
    }
}
