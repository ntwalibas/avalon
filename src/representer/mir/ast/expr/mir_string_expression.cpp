#include <string>

#include "representer/mir/ast/expr/mir_string_expression.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with type source information
     */
    mir_string_expression::mir_string_expression(token& tok, mir_type& l_type, const std::string& val) : m_tok(tok), m_type(l_type), m_val(val) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_string_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_type
     * returns the type of this expression
     */
    mir_type& mir_string_expression::get_type() {
        return m_type;
    }

    /**
     * get_value
     * returns the string representation of the string expression
     */
    const std::string& mir_string_expression::get_value() const {
        return m_val;
    }
}
