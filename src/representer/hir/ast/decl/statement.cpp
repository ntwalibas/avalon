#include <cstddef>
#include <memory>

#include "representer/hir/ast/decl/statement.hpp"
#include "representer/hir/ast/stmt/stmt.hpp"


namespace avalon {
    /**
     * the constructor expects nothing
     */
    statement::statement() : m_statement(nullptr), m_reachable(false), m_terminates(false), m_passes(true) {        
    }

    /**
     * set_statement
     * since statements can vary but the AST can only contain declarations
     * we invoke this function to store the actual statement in this declaration.
     */
    void statement::set_statement(std::shared_ptr<stmt>& statement) {
        m_statement = statement;
    }

    /**
     * get_statement
     * return the statement stored in this declaration
     */
    std::shared_ptr<stmt>& statement::get_statement() {
        return m_statement;
    }
}
