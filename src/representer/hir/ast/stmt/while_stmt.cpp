#include <cstddef>
#include <memory>

#include "representer/hir/ast/stmt/while_stmt.hpp"
#include "representer/hir/ast/stmt/block_stmt.hpp"
#include "representer/hir/symtable/scope.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with source code information
     */
    while_stmt::while_stmt(token& tok) : m_tok(tok), m_condition(nullptr) {
    }

    /**
     * copy constructor
     */
    while_stmt::while_stmt(const std::shared_ptr<while_stmt>& wh_stmt, std::shared_ptr<scope>& parent_scope) : m_tok(wh_stmt -> get_token()), m_scope(std::make_shared<scope>(wh_stmt -> get_scope())), m_condition(wh_stmt -> get_condition() -> copy()), m_blc(wh_stmt -> get_block(), m_scope) {
        m_scope -> set_parent(parent_scope);
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& while_stmt::get_token() const {
        return m_tok;
    }

    /**
     * set_scope
     * specify the scope bound to this while statement
     */
    void while_stmt::set_scope(std::shared_ptr<scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope bound to this while statement
     */
    std::shared_ptr<scope>& while_stmt::get_scope() {
        return m_scope;
    }

    /**
     * set_condition
     * sets the condition under which the loop will execute
     */
    void while_stmt::set_condition(std::shared_ptr<expr>& condition) {
        m_condition = condition;
    }

    /**
     * get_condition
     * returns the condition under which the loop will execute
     */
    std::shared_ptr<expr>& while_stmt::get_condition() {
        return m_condition;
    }

    /**
     * set_block
     * set the block of statements to execute
     */
    void while_stmt::set_block(block_stmt& blc) {
        m_blc = blc;
    }

    /**
     * get_block
     * returns the block of statements that are to be executed for this case
     */
    block_stmt& while_stmt::get_block() {
        return m_blc;
    }
}
