#include <memory>

#include "representer/hir/ast/expr/underscore_expression.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token associated with this expression
     */
    underscore_expression::underscore_expression(token& tok) : m_tok(tok) {
    }

    /**
     * the copy constructor expects a pointer to the resource to copy
     */
    underscore_expression::underscore_expression(std::shared_ptr<underscore_expression>const & under_expr) : m_tok(under_expr -> get_token()), m_instance(under_expr -> get_type_instance()) {
    }

    /**
     * the copy assignment operator expects a pointer to the source resource to copy
     */
    underscore_expression& underscore_expression::operator=(std::shared_ptr<underscore_expression>const & under_expr) {        
        m_tok = under_expr -> get_token();
        m_instance = under_expr -> get_type_instance();
        return * this;
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& underscore_expression::get_token() const {
        return m_tok;
    }

    /**
     * set_type_instance
     * sets the type instance of this expression
     */
    void underscore_expression::set_type_instance(type_instance& instance) {
        m_instance = instance;
    }

    /**
     * get_type_instance
     * returns the type instance of this expression
     */
    type_instance& underscore_expression::get_type_instance() {
        return m_instance;
    }
}
