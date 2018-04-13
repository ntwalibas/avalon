#include <cstddef>
#include <memory>

#include "representer/mir/ast/stmt/mir_expression_stmt.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"


namespace avalon {
    /**
     * the constructor expects nothing
     */
    mir_expression_stmt::mir_expression_stmt() : m_expression(nullptr) {
    }

    /**
     * set_expression
     * when storing expressions in the AST, everything must decay into a declaration.
     * this function decays any of those expressions above into a simple expression statement
     * which in turn will decay into a statement declaration.
     */
    void mir_expression_stmt::set_expression(std::shared_ptr<mir_expr>& expression) {
        m_expression = expression;
    }

    /**
     * get_expression
     * returns the specific expression stored into this expression statement
     */
    std::shared_ptr<mir_expr>& mir_expression_stmt::get_expression() {
        return m_expression;
    }
}
