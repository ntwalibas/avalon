#include <memory>

/* AST */
#include "representer/hir/ast/expr/match_expression.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"

/* Builtins */
#include "representer/hir/builtins/avalon_bool.hpp"

/* Lexer */
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects:
     * - the type of match expression this is
     * - the left operand to the match operator
     * - and the right operand to match operator
     */
    match_expression::match_expression(token& tok, match_expression_type expr_type, std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval) : m_tok(tok), m_expr_type(expr_type), m_lval(lval), m_rval(rval) {
        avalon_bool avl_bool;
        m_instance = avl_bool.get_type_instance();
    }

    /**
     * copy constructor
     */
    match_expression::match_expression(const std::shared_ptr<match_expression>& match_expr) : m_tok(match_expr -> get_token()), m_instance(match_expr -> get_type_instance()), m_expr_type(match_expr -> get_expression_type()), m_lval(match_expr -> get_lval() -> copy()), m_rval(match_expr -> get_rval() -> copy()) {
    }

    /**
     * assignment copy operator
     */
    match_expression& match_expression::operator=(const std::shared_ptr<match_expression>& match_expr) {
        m_tok = match_expr -> get_token();
        m_expr_type = match_expr -> get_expression_type();
        m_instance = match_expr -> get_type_instance();
        m_lval = match_expr -> get_lval() -> copy();
        m_rval = match_expr -> get_rval() -> copy();
        return * this;
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& match_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_type_instance
     * returns the type instance of this expression
     */
    type_instance& match_expression::get_type_instance() {
        return m_instance;
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
