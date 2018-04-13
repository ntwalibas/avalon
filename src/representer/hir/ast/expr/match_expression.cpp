#include <memory>

#include "representer/hir/ast/expr/match_expression.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects:
     * - the type of match expression this is
     * - the left operand to the match operator
     * - and the right operand to match operator
     */
    match_expression::match_expression(token& tok, match_expression_type expr_type, std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval) : m_tok(tok), m_expr_type(expr_type), m_lval(lval), m_rval(rval) {
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& match_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_expression_type
     * returns the type of expression we are dealing
     */
    const match_expression_type& match_expression::get_expression_type() const {
        return m_expr_type;
    }

    /**
     * get_lval
     * returns the left operand to the match operator
     */
    std::shared_ptr<expr>& match_expression::get_lval() {
        return m_lval;
    }

    /**
     * get_rval
     * returns the right operand to the match operator
     */
    std::shared_ptr<expr>& match_expression::get_rval() {
        return m_rval;
    }
}
