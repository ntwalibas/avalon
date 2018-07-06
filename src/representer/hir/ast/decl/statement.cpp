#include <cstddef>
#include <memory>

#include "representer/hir/ast/decl/statement.hpp"
#include "representer/hir/symtable/scope.hpp"
#include "representer/hir/ast/stmt/stmt.hpp"


namespace avalon {
    /**
     * the constructor expects nothing
     */
    statement::statement() : m_statement(nullptr), m_reachable(false), m_terminates(false), m_passes(true) {        
    }

    /**
     * copy constructor
     */
    statement::statement(const std::shared_ptr<statement>& a_statement, std::shared_ptr<scope>& parent_scope) : m_statement(nullptr), m_reachable(a_statement -> is_reachable()), m_terminates(a_statement -> terminates()), m_passes(a_statement -> passes()) {
        std::shared_ptr<stmt>& l_statement = a_statement -> get_statement();
        m_statement = l_statement -> copy(parent_scope);
    }

    /**
     * set_statement
     * since statements can vary but the AST can only contain declarations
     * we invoke this function to store the actual statement in this declaration.
     */
    void statement::set_statement(std::shared_ptr<stmt>& l_statement) {
        m_statement = l_statement;
    }

    /**
     * get_statement
     * return the statement stored in this declaration
     */
    std::shared_ptr<stmt>& statement::get_statement() {
        return m_statement;
    }
}
