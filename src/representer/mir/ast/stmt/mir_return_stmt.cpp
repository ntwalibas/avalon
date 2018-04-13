#include <cstddef>
#include <memory>


#include "representer/mir/ast/stmt/mir_return_stmt.hpp"
#include "representer/mir/ast/stmt/mir_stmt.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    mir_return_stmt::mir_return_stmt(token& tok) : m_tok(tok), m_expression(nullptr) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_return_stmt::get_token() const {
        return m_tok;
    }

    /**
     * set_expression
     * set the expression to be returned if any
     */
    void mir_return_stmt::set_expression(std::shared_ptr<mir_expr>& expression) {
        m_expression = expression;
    }

    /**
     * get_expression
     * returns the expression that is meant to be returned
     */
    std::shared_ptr<mir_expr>& mir_return_stmt::get_expression() {
        return m_expression;
    }
}
