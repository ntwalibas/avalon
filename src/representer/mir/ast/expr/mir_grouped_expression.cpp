#include <memory>

#include "representer/mir/ast/expr/mir_grouped_expression.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the operand of the grouped operator
     */
    mir_grouped_expression::mir_grouped_expression(token& tok, mir_type& l_type, std::shared_ptr<mir_expr>& val) : m_tok(tok), m_type(l_type), m_val(val) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_grouped_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_type
     * returns the type of this expression
     */
    mir_type& mir_grouped_expression::get_type() {
        return m_type;
    }

    /**
     * get_value
     * returns the grouped operator operand
     */
    std::shared_ptr<mir_expr>& mir_grouped_expression::get_value() {
        return m_val;
    }
}
