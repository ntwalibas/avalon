#include <cstddef>
#include <memory>
#include <vector>

#include "representer/exceptions/branch_error.hpp"
#include "representer/mir/ast/stmt/mir_block_stmt.hpp"
#include "representer/mir/ast/stmt/mir_if_stmt.hpp"
#include "representer/mir/symtable/mir_scope.hpp"
#include "representer/mir/ast/stmt/mir_stmt.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
/**
 * the constructor expects the token with source code information
 */
mir_if_stmt::mir_if_stmt(token& tok) : m_tok(tok), m_condition(nullptr), m_else(nullptr) {    
}

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_if_stmt::get_token() const {
        return m_tok;
    }

    /**
     * set_scope
     * specify the scope bound to the main branch
     */
    void mir_if_stmt::set_scope(std::shared_ptr<mir_scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope bound to the main branch
     */
    std::shared_ptr<mir_scope>& mir_if_stmt::get_scope() {
        return m_scope;
    }

    /**
     * set_condition
     * sets the condition under which the conditional will execute
     */
    void mir_if_stmt::set_condition(std::shared_ptr<mir_expr>& condition) {
        m_condition = condition;
    }

    /**
     * get_condition
     * returns the condition under which the conditional will execute
     */
    std::shared_ptr<mir_expr>& mir_if_stmt::get_condition() {
        return m_condition;
    }

    /**
     * set_block
     * set the block of statements to execute
     */
    void mir_if_stmt::set_block(mir_block_stmt& blc) {
        m_blc = blc;
    }

    /**
     * get_block
     * returns the block of statements that are to be executed for this case
     */
    mir_block_stmt& mir_if_stmt::get_block() {
        return m_blc;
    }

    /**
     * add_elif
     * add an elif branch to this statement
     */
    void mir_if_stmt::add_elif(mir_elif_branch& branch) {
        m_elifs.push_back(branch);
    }

    /**
     * get_elifs
     * returns a vector with all the elif branches
     */
    std::vector<mir_elif_branch>& mir_if_stmt::get_elifs() {
        return m_elifs;
    }

    /**
     * set_else
     * sets the else branch
     */
    void mir_if_stmt::set_else(mir_else_branch& branch) {
        m_else = std::make_shared<mir_else_branch>(branch);
    }

    /**
     * get_else
     * returns the else branch if any
     * throws a "branch_error" if no else branch was set
     */
    mir_else_branch mir_if_stmt::get_else() const {
        if(m_else == nullptr)
            throw branch_error("This if statement has no else branch.");
        else
            return * m_else;
    }

    /**
     * has_else
     * returns true if this statement has else branch
     */
    bool mir_if_stmt::has_else() const {
        if(m_else == nullptr)
            return false;
        else
            return true;
    }

/**
 * the constructor expects the token with source code information
 */
mir_elif_branch::mir_elif_branch(token& tok) : m_tok(tok) {    
}

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_elif_branch::get_token() const {
        return m_tok;
    }

    /**
     * set_scope
     * specify the scope bound to this elif branch
     */
    void mir_elif_branch::set_scope(std::shared_ptr<mir_scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope bound to this elif branch
     */
    std::shared_ptr<mir_scope>& mir_elif_branch::get_scope() {
        return m_scope;
    }

    /**
     * set_condition
     * sets the condition under which the elif branch will execute
     */
    void mir_elif_branch::set_condition(std::shared_ptr<mir_expr>& condition) {
        m_condition = condition;
    }

    /**
     * get_condition
     * returns the condition under which the elif branch will execute
     */
    std::shared_ptr<mir_expr>& mir_elif_branch::get_condition() {
        return m_condition;
    }

    /**
     * set_block
     * set the block of statements to execute
     */
    void mir_elif_branch::set_block(mir_block_stmt& blc) {
        m_blc = blc;
    }

    /**
     * get_block
     * returns the block of statements that are to be executed for this elif branch
     */
    mir_block_stmt& mir_elif_branch::get_block() {
        return m_blc;
    }

/**
 * the constructor expects the token with source code information
 */
mir_else_branch::mir_else_branch(token& tok) : m_tok(tok) {    
}

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_else_branch::get_token() const {
        return m_tok;
    }

    /**
     * set_scope
     * specify the scope bound to this else branch
     */
    void mir_else_branch::set_scope(std::shared_ptr<mir_scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope bound to this else branch
     */
    std::shared_ptr<mir_scope>& mir_else_branch::get_scope() {
        return m_scope;
    }

    /**
     * set_block
     * set the block of statements to execute
     */
    void mir_else_branch::set_block(mir_block_stmt& blc) {
        m_blc = blc;
    }

    /**
     * get_block
     * returns the block of statements that are to be executed for the else branch
     */
    mir_block_stmt& mir_else_branch::get_block() {
        return m_blc;
    }
}
