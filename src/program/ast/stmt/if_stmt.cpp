#include <cstddef>
#include <memory>
#include <vector>

#include "program/ast/exceptions/branch_error.hpp"
#include "program/ast/stmt/block_stmt.hpp"
#include "program/ast/stmt/if_stmt.hpp"
#include "program/symtable/scope.hpp"
#include "program/ast/stmt/stmt.hpp"
#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
/**
 * the constructor expects the token with source code information
 */
if_stmt::if_stmt(token& tok) : m_tok(tok), m_condition(nullptr), m_else(nullptr) {    
}

    /**
     * get_token
     * returns a token with type source information
     */
    const token& if_stmt::get_token() const {
        return m_tok;
    }

    /**
     * set_scope
     * specify the scope bound to the main branch
     */
    void if_stmt::set_scope(std::shared_ptr<scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope bound to the main branch
     */
    std::shared_ptr<scope>& if_stmt::get_scope() {
        return m_scope;
    }

    /**
     * set_condition
     * sets the condition under which the conditional will execute
     */
    void if_stmt::set_condition(std::shared_ptr<expr>& condition) {
        m_condition = condition;
    }

    /**
     * get_condition
     * returns the condition under which the conditional will execute
     */
    std::shared_ptr<expr>& if_stmt::get_condition() {
        return m_condition;
    }

    /**
     * set_block
     * set the block of statements to execute
     */
    void if_stmt::set_block(block_stmt& blc) {
        m_blc = blc;
    }

    /**
     * get_block
     * returns the block of statements that are to be executed for this case
     */
    block_stmt& if_stmt::get_block() {
        return m_blc;
    }

    /**
     * add_elif
     * add an elif branch to this statement
     */
    void if_stmt::add_elif(elif_branch& branch) {
        m_elifs.push_back(branch);
    }

    /**
     * get_elifs
     * returns a vector with all the elif branches
     */
    std::vector<elif_branch>& if_stmt::get_elifs() {
        return m_elifs;
    }

    /**
     * set_else
     * sets the else branch
     */
    void if_stmt::set_else(else_branch& branch) {
        m_else = std::make_shared<else_branch>(branch);
    }

    /**
     * get_else
     * returns the else branch if any
     * throws a "branch_error" if no else branch was set
     */
    else_branch if_stmt::get_else() const {
        if(m_else == nullptr)
            throw branch_error("This if statement has no else branch.");
        else
            return * m_else;
    }

    /**
     * has_else
     * returns true if this statement has else branch
     */
    bool if_stmt::has_else() const {
        if(m_else == nullptr)
            return false;
        else
            return true;
    }

/**
 * the constructor expects the token with source code information
 */
elif_branch::elif_branch(token& tok) : m_tok(tok) {    
}

    /**
     * get_token
     * returns a token with type source information
     */
    const token& elif_branch::get_token() const {
        return m_tok;
    }

    /**
     * set_scope
     * specify the scope bound to this elif branch
     */
    void elif_branch::set_scope(std::shared_ptr<scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope bound to this elif branch
     */
    std::shared_ptr<scope>& elif_branch::get_scope() {
        return m_scope;
    }

    /**
     * set_condition
     * sets the condition under which the elif branch will execute
     */
    void elif_branch::set_condition(std::shared_ptr<expr>& condition) {
        m_condition = condition;
    }

    /**
     * get_condition
     * returns the condition under which the elif branch will execute
     */
    std::shared_ptr<expr>& elif_branch::get_condition() {
        return m_condition;
    }

    /**
     * set_block
     * set the block of statements to execute
     */
    void elif_branch::set_block(block_stmt& blc) {
        m_blc = blc;
    }

    /**
     * get_block
     * returns the block of statements that are to be executed for this elif branch
     */
    block_stmt& elif_branch::get_block() {
        return m_blc;
    }

/**
 * the constructor expects the token with source code information
 */
else_branch::else_branch(token& tok) : m_tok(tok) {    
}

    /**
     * get_token
     * returns a token with type source information
     */
    const token& else_branch::get_token() const {
        return m_tok;
    }

    /**
     * set_scope
     * specify the scope bound to this else branch
     */
    void else_branch::set_scope(std::shared_ptr<scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope bound to this else branch
     */
    std::shared_ptr<scope>& else_branch::get_scope() {
        return m_scope;
    }

    /**
     * set_block
     * set the block of statements to execute
     */
    void else_branch::set_block(block_stmt& blc) {
        m_blc = blc;
    }

    /**
     * get_block
     * returns the block of statements that are to be executed for the else branch
     */
    block_stmt& else_branch::get_block() {
        return m_blc;
    }
}
