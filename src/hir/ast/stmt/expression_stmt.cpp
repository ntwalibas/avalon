#include <cstddef>
#include <memory>

#include "hir/ast/expr/expr.hpp"
#include "hir/ast/stmt/expression_stmt.hpp"


namespace avalon {
    /**
     * the constructor expects nothing
     */
    expression_stmt::expression_stmt() : m_expression(nullptr) {
    }

    /**
     * set_expression
     * when storing expressions in the AST, everything must decay into a declaration.
     * this function decays any of those expressions above into a simple expression statement
     * which in turn will decay into a statement declaration.
     */
    void expression_stmt::set_expression(std::shared_ptr<expr>& expression) {
        m_expression = expression;
    }

    /**
     * get_expression
     * returns the specific expression stored into this expression statement
     */
    std::shared_ptr<expr>& expression_stmt::get_expression() {
        return m_expression;
    }
}
