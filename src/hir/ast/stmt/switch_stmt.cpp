#include <utility>
#include <cstddef>
#include <memory>
#include <vector>

#include "hir/ast/exceptions/case_error.hpp"
#include "hir/ast/stmt/switch_stmt.hpp"
#include "hir/ast/stmt/block_stmt.hpp"
#include "hir/symtable/scope.hpp"
#include "hir/ast/stmt/stmt.hpp"
#include "hir/ast/expr/expr.hpp"
#include "hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
/**
 * the constructor expects the token with source code information
 */
switch_stmt::switch_stmt(token& tok) : m_tok(tok) {        
}

    /**
     * get_token
     * returns a token with type source information
     */
    const token& switch_stmt::get_token() const {
        return m_tok;
    }

    /**
     * add_param
     * add a parameter
     */
    void switch_stmt::add_param(std::shared_ptr<expr> param) {
        m_params.push_back(param);
    }

    /**
     * get_params
     * return a vector of arguments
     */
    std::vector<std::shared_ptr<expr> >& switch_stmt::get_params() {
        return m_params;
    }

    /**
     * add_case
     * add a case
     */
    void switch_stmt::add_case(switch_case cas) {
        m_cases.push_back(cas);
    }

    /**
     * get_cases
     * returns all the cases under this statement
     */
    std::vector<switch_case>& switch_stmt::get_cases() {
        return m_cases;
    }

    /**
     * set_default
     * set the default case
     */
    void switch_stmt::set_default(switch_default cas) {
        m_default = std::make_shared<switch_default>(cas);
    }

    /**
     * get_default
     * returns the default case if any
     * throws a "case_error" if the default case doesn't exist
     */
    switch_default switch_stmt::get_default() {
        if(m_default == nullptr)
            throw case_error("Switch statement doesn't have a default case.");

        return (* m_default);
    }

    /**
     * has_default
     * returns true if this switch statement has a default case
     */
    bool switch_stmt::has_default() {
        if(m_default == nullptr)
            return false;
        return true;
    }


/**
 * the constructor expects the token with source code information
 */
switch_case::switch_case(token& tok) : m_tok(tok) {    
}

    /**
     * get_token
     * returns a token with type source information
     */
    const token& switch_case::get_token() const {
        return m_tok;
    }

    /**
     * set_scope
     * specify the scope bound to this case
     */
    void switch_case::set_scope(std::shared_ptr<scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope bound to this case
     */
    std::shared_ptr<scope>& switch_case::get_scope() {
        return m_scope;
    }

    /**
     * add_match
     * add an expression this case can match
     */
    void switch_case::add_match(std::shared_ptr<expr> match) {
        m_matches.push_back(match);
    }

    /**
     * get_matches
     * return a vector of expressions this case can match
     */
    std::vector<std::shared_ptr<expr> >& switch_case::get_matches() {
        return m_matches;
    }

    /**
     * set_block
     * set the block of statements to execute
     */
    void switch_case::set_block(block_stmt& blc) {
        m_blc = blc;
    }

    /**
     * get_block
     * returns the block of statements that are to be executed for this case
     */
    block_stmt& switch_case::get_block() {
        return m_blc;
    }


/**
 * the constructor expects the token with source code information
 */
switch_default::switch_default(token& tok) : m_tok(tok) {    
}

    /**
     * get_token
     * returns a token with type source information
     */
    const token& switch_default::get_token() const {
        return m_tok;
    }

    /**
     * set_scope
     * specify the scope bound to this default case
     */
    void switch_default::set_scope(std::shared_ptr<scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope bound to this default case
     */
    std::shared_ptr<scope>& switch_default::get_scope() {
        return m_scope;
    }

    /**
     * set_block
     * set the block of statements to execute
     */
    void switch_default::set_block(block_stmt& blc) {
        m_blc = blc;
    }

    /**
     * get_block
     * returns the block of statements that are to be executed for the default case
     */
    block_stmt& switch_default::get_block() {
        return m_blc;
    }
}
