#include <memory>

#include "hir/ast/expr/assignment_expression.hpp"
#include "hir/ast/expr/expr.hpp"
#include "hir/ast/decl/type.hpp"
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
     * set_type_instance
     * sets the type instance of this expression
     */
    void assignment_expression::set_type_instance(type_instance& instance) {
        m_instance = instance;
    }

    /**
     * get_type_instance
     * returns the type instance of this expression
     */
    type_instance& assignment_expression::get_type_instance() {
        return m_instance;
    }

    /**
     * has_type_instance
     * returns true if the type instance is not star
     */
    bool assignment_expression::has_type_instance() {
        if(m_instance.get_name() == "*")
            return false;
        else
            return true;
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
