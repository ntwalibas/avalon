#include <memory>

#include "representer/hir/ast/expr/grouped_expression.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the operand of the grouped operator
     */
    grouped_expression::grouped_expression(token& tok, std::shared_ptr<expr>& val) : m_tok(tok), m_val(val){
    }

    /**
     * copy constructor
     */
    grouped_expression::grouped_expression(const std::shared_ptr<grouped_expression>& gr_expr) : m_tok(gr_expr -> get_token()), m_instance(gr_expr -> get_type_instance()), m_val(gr_expr -> get_value() -> copy()) {
    }

    /**
     * assignment copy operator
     */
    grouped_expression& grouped_expression::operator=(const std::shared_ptr<grouped_expression>& gr_expr) {
        m_tok = gr_expr -> get_token();
        m_instance = gr_expr -> get_type_instance();
        m_val = gr_expr -> get_value() -> copy();
        return * this;
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& grouped_expression::get_token() const {
        return m_tok;
    }

    /**
     * set_type_instance
     * sets the type instance of this expression
     */
    void grouped_expression::set_type_instance(type_instance& instance) {
        m_instance = instance;
    }

    /**
     * get_type_instance
     * returns the type instance of this expression
     */
    type_instance& grouped_expression::get_type_instance() {
        return m_instance;
    }

    /**
     * has_type_instance
     * returns true if the type instance is not star
     */
    bool grouped_expression::has_type_instance() {
        if(m_instance.get_name() == "*")
            return false;
        else
            return true;
    }

    /**
     * get_value
     * returns the grouped operator operand
     */
    std::shared_ptr<expr>& grouped_expression::get_value() {
        return m_val;
    }
}
