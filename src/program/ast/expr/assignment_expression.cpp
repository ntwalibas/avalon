#include <memory>

#include "program/ast/expr/assignment_expression.hpp"
#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects:
     * - the left side of the equal sign
     * - and the right side of the equal sign
     */
    assignment_expression::assignment_expression(token& tok, std::shared_ptr<expr> lval, std::shared_ptr<expr> rval) : m_tok(tok), m_lval(lval), m_rval(rval) {
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& assignment_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_lval
     * returns the left side of the assignment
     */
    std::shared_ptr<expr>& assignment_expression::get_lval() {
        return m_lval;
    }

    /**
     * get_rval
     * returns the right side of the assignment
     */
    std::shared_ptr<expr>& assignment_expression::get_rval() {
        return m_rval;
    }

    /**
     * is_deconstruction
     * returns true if this assignment is a deconstruction expression
     */
    bool assignment_expression::is_deconstruction() {
        if(m_lval -> is_identifier_expression())
            return false;
        else
            return true;
    }
}
