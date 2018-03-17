#include <cstddef>
#include <memory>

#include "program/ast/exceptions/branch_error.hpp"
#include "program/ast/stmt/block_stmt.hpp"
#include "program/ast/stmt/for_stmt.hpp"
#include "program/symtable/scope.hpp"
#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
/**
 * the constructor expects the token with source code information
 */
for_stmt::for_stmt(token& tok) : m_tok(tok), m_iteration(nullptr), m_empty(nullptr) {    
}

    /**
     * get_token
     * returns a token with type source information
     */
    const token& for_stmt::get_token() const {
        return m_tok;
    }

    /**
     * set_scope
     * specify the scope bound to this for statement
     */
    void for_stmt::set_scope(std::shared_ptr<scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope bound to this for statement
     */
    std::shared_ptr<scope>& for_stmt::get_scope() {
        return m_scope;
    }

    /**
     * set_iteration
     * sets the iteration that drives the loop
     */
    void for_stmt::set_iteration(std::shared_ptr<expr>& iteration) {
        m_iteration = iteration;
    }

    /**
     * get_iteration
     * returns the iteration that drives the loop
     */
    std::shared_ptr<expr>& for_stmt::get_iteration() {
        return m_iteration;
    }

    /**
     * set_block
     * set the block of statements to execute
     */
    void for_stmt::set_block(block_stmt& blc) {
        m_blc = blc;
    }

    /**
     * get_block
     * returns the block of statements that are to be executed for this case
     */
    block_stmt& for_stmt::get_block() {
        return m_blc;
    }

    /**
     * set_empty
     * sets the empty branch
     */
    void for_stmt::set_empty(empty_branch& branch) {
        m_empty = std::make_shared<empty_branch>(branch);
    }

    /**
     * get_empty
     * returns the empty branch if any
     * throws a "branch_error" if no empty branch was set
     */
    empty_branch for_stmt::get_empty() const {
        if(m_empty == nullptr)
            throw branch_error("This for statement has no empty branch.");
        else
            return * m_empty;
    }

    /**
     * has_empty
     * returns true if this statement has empty branch
     */
    bool for_stmt::has_empty() const {
        if(m_empty == nullptr)
            return false;
        else
            return true;
    }

/**
 * the constructor expects the token with source code information
 */
empty_branch::empty_branch(token& tok) : m_tok(tok) {    
}

    /**
     * get_token
     * returns a token with type source information
     */
    const token& empty_branch::get_token() const {
        return m_tok;
    }

    /**
     * set_scope
     * specify the scope bound to this empty branch
     */
    void empty_branch::set_scope(std::shared_ptr<scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope bound to this empty branch
     */
    std::shared_ptr<scope>& empty_branch::get_scope() {
        return m_scope;
    }

    /**
     * set_block
     * set the block of statements to execute
     */
    void empty_branch::set_block(block_stmt& blc) {
        m_blc = blc;
    }

    /**
     * get_block
     * returns the block of statements that are to be executed for the empty branch
     */
    block_stmt& empty_branch::get_block() {
        return m_blc;
    }
}
