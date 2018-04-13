#include <memory>

#include "representer/mir/ast/expr/mir_assignment_expression.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects:
     * - the left side of the equal sign
     * - and the right side of the equal sign
     */
    mir_assignment_expression::mir_assignment_expression(token& tok, mir_type& l_type, std::shared_ptr<mir_expr> lval, std::shared_ptr<mir_expr> rval) : m_tok(tok), m_type(l_type), m_lval(lval), m_rval(rval) {
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_assignment_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_type
     * returns the type of this expression
     */
    mir_type& mir_assignment_expression::get_type() {
        return m_type;
    }

    /**
     * get_lval
     * returns the left side of the assignment
     */
    std::shared_ptr<mir_expr>& mir_assignment_expression::get_lval() {
        return m_lval;
    }

    /**
     * get_rval
     * returns the right side of the assignment
     */
    std::shared_ptr<mir_expr>& mir_assignment_expression::get_rval() {
        return m_rval;
    }
}
