#include <memory>

#include "representer/hir/ast/expr/cast_expression.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the type instance to cast and the expression to cast
     */
    cast_expression::cast_expression(token& tok, type_instance& cast_type_instance, std::shared_ptr<expr> val) : m_tok(tok), m_cast_type_instance(cast_type_instance), m_val(val) {
    }

    /**
     * copy constructor
     */
    cast_expression::cast_expression(const std::shared_ptr<cast_expression>& cast_expr) : m_tok(cast_expr -> get_token()), m_cast_type_instance(cast_expr -> get_cast_type_instance()), m_val(cast_expr -> get_val() -> copy()) {        
    }

    /**
     * assignment copy operator
     */
    cast_expression& cast_expression::operator=(const std::shared_ptr<cast_expression>& cast_expr) {
        m_tok = cast_expr -> get_token();
        m_cast_type_instance = cast_expr -> get_cast_type_instance();
        m_val = cast_expr -> get_val() -> copy();
        return * this;
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& cast_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_cast_type_instance
     * returns the type we are casting to
     */
    type_instance& cast_expression::get_cast_type_instance() {
        return m_cast_type_instance;
    }

    /**
     * get_val
     * returns the cast operator operand
     */
    std::shared_ptr<expr>& cast_expression::get_val() {
        return m_val;
    }
}
