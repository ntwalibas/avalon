#include <cstddef>
#include <memory>


#include "hir/ast/stmt/return_stmt.hpp"
#include "hir/ast/stmt/stmt.hpp"
#include "hir/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    return_stmt::return_stmt(token& tok) : m_tok(tok), m_expression(nullptr) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& return_stmt::get_token() const {
        return m_tok;
    }

    /**
     * set_expression
     * set the expression to be returned if any
     */
    void return_stmt::set_expression(std::shared_ptr<expr>& expression) {
        m_expression = expression;
    }

    /**
     * get_expression
     * returns the expression that is meant to be returned
     */
    std::shared_ptr<expr>& return_stmt::get_expression() {
        return m_expression;
    }
}
