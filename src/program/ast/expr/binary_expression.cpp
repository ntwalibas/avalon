#include <memory>

#include "program/ast/expr/binary_expression.hpp"
#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects:
     * - the type of binary expression this is
     * - the left operand to the binary operator
     * - and the right operand to the binary operator
     */
    binary_expression::binary_expression(token& tok, binary_expression_type expr_type, std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval) : m_tok(tok), m_expr_type(expr_type), m_lval(lval), m_rval(rval) {
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& binary_expression::get_token() const {
        return m_tok;
    }

    /**
     * set_type_instance
     * sets the type instance of this expression
     */
    void binary_expression::set_type_instance(type_instance& instance) {
        m_instance = instance;
    }

    /**
     * get_type_instance
     * returns the type instance of this expression
     */
    type_instance& binary_expression::get_type_instance() {
        return m_instance;
    }

    /**
     * has_type_instance
     * returns true if the type instance is not star
     */
    bool binary_expression::has_type_instance() {
        if(m_instance.get_name() == "*" && m_instance.get_namespace() == "*")
            return true;
        else
            return false;
    }

    /**
     * get_expression_type
     * returns the type of expression we are dealing
     */
    const binary_expression_type& binary_expression::get_expression_type() const {
        return m_expr_type;
    }

    /**
     * get_lval
     * returns the left operand to the binary operator
     */
    std::shared_ptr<expr>& binary_expression::get_lval() {
        return m_lval;
    }

    /**
     * get_rval
     * returns the right operand to the binary operator
     */
    std::shared_ptr<expr>& binary_expression::get_rval() {
        return m_rval;
    }
}
