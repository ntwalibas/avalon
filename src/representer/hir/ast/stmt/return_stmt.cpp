#include <cstddef>
#include <memory>


#include "representer/hir/ast/stmt/return_stmt.hpp"
#include "representer/hir/ast/stmt/stmt.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    return_stmt::return_stmt(token& tok) : m_tok(tok), m_expression(nullptr) {        
    }

    /**
     * copy constructor
     */
    return_stmt::return_stmt(const std::shared_ptr<return_stmt>& ret_stmt) : m_tok(ret_stmt -> get_token()), m_expression(ret_stmt -> get_expression() -> copy()) {        
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

    /**
     * has_expression
     * return true if an expression was supplied to the return statement
     */
    bool return_stmt::has_expression() {
        if(m_expression == nullptr)
            return false;
        else
            return true;
    }
}
