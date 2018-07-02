#include <memory>
#include <vector>

/* AST */
#include "representer/hir/ast/stmt/expression_stmt.hpp"
#include "representer/hir/ast/stmt/continue_stmt.hpp"
#include "representer/hir/ast/stmt/return_stmt.hpp"
#include "representer/hir/ast/stmt/block_stmt.hpp"
#include "representer/hir/ast/stmt/break_stmt.hpp"
#include "representer/hir/ast/stmt/pass_stmt.hpp"
#include "representer/hir/ast/decl/statement.hpp"
#include "representer/hir/ast/decl/variable.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "representer/hir/ast/decl/decl.hpp"

/* Builtins */
#include "representer/hir/builtins/avalon_void.hpp"

/* Symbol table */
#include "representer/hir/symtable/scope.hpp"

/* Checker */
#include "checker/decl/variable/variable_checker.hpp"
#include "checker/expr/expression_checker.hpp"
#include "checker/stmt/block_checker.hpp"

/* Exceptions */
#include "representer/exceptions/symbol_already_declared.hpp"
#include "representer/exceptions/symbol_can_collide.hpp"
#include "checker/exceptions/invalid_expression.hpp"
#include "checker/exceptions/invalid_statement.hpp"
#include "checker/exceptions/invalid_variable.hpp"
#include "checker/exceptions/invalid_block.hpp"


namespace avalon {
    /**
     * the constructor expects nothing
     */
    block_checker::block_checker(type_instance& ret_instance) : m_ret_instance(ret_instance), m_decls_count(0), m_inside_loop(false) {
    }

    /**
     * check
     * checking a blocks statement entails the following:
     * - a block can only contain variable and statement declarations
     * - if a block contains a pass statement, it cannot contain any other type of statement or declaration
     * - if a break or continue statement is encountered, they must be within a loop
     * - all declarations within a block must be valid
     */
    void block_checker::check(block_stmt& l_statement, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::vector<std::shared_ptr<decl> >& block_decls = l_statement.get_declarations();
        m_decls_count = block_decls.size();

        // we iterate over all declarations, validating variables and statements and ignoring the rest (functions and types)
        for(auto& block_decl : block_decls) {
            // we check variable declarations
            if(block_decl -> is_variable()) {                
                check_variable(block_decl, l_scope, ns_name);
            }
            else if(block_decl -> is_statement()) {
                check_statement(block_decl, l_scope, ns_name);
            }
            
            else {
                throw invalid_block("Block statements must contain variable or statement declarations alone.");
            }
        }
    }

    /**
     * check_variable
     * given a variable declaration in the block, check if it is valid
     */
    void block_checker::check_variable(std::shared_ptr<decl>& declaration, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<variable> variable_decl = std::static_pointer_cast<variable>(declaration);
        variable_checker v_checker;

        // add the variable to the provided scope
        try {
            l_scope -> add_variable(ns_name, variable_decl);
        } catch(symbol_already_declared err) {
            throw invalid_variable(variable_decl -> get_token(), err.what());
        } catch(symbol_can_collide err) {
            throw invalid_variable(variable_decl -> get_token(), err.what());
        }

        // check the variable
        try {
            v_checker.check(variable_decl, l_scope, ns_name);
        } catch(invalid_variable err) {
            throw err;
        }
    }

    /**
     * check_statement
     * given a statement declaration, check if it is valid
     */
    void block_checker::check_statement(std::shared_ptr<decl>& declaration, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<statement> const & stmt_decl = std::static_pointer_cast<statement>(declaration);
        std::shared_ptr<stmt>& l_stmt = stmt_decl -> get_statement();
        
        if(l_stmt -> is_while()) {
            check_while(l_stmt);
        }
        else if(l_stmt -> is_if()) {
            check_if(l_stmt);
        }
        else if(l_stmt -> is_break()) {
            check_break(l_stmt);
        }
        else if(l_stmt -> is_continue()) {
            check_continue(l_stmt);
        }
        else if(l_stmt -> is_pass()) {
            check_pass(l_stmt);
        }
        else if(l_stmt -> is_return()) {
            check_return(l_stmt, l_scope, ns_name);
        }
        else if(l_stmt -> is_expression()) {
            check_expression(l_stmt, l_scope, ns_name);
        }
        else {
            throw std::runtime_error("[compiler error] unexpected statement type in weak block checker.");
        }
    }

    /**
     * check_while
     * given a statement, check if it is a vali while statement
     */
    void block_checker::check_while(std::shared_ptr<stmt>& a_statement) {

    }

    /**
     * check_if
     * given a statement, check if it is a valid if statement
     */
    void block_checker::check_if(std::shared_ptr<stmt>& a_statement) {

    }

    /**
     * check_break
     * check if a break statement occurs inside a loop
     */
    void block_checker::check_break(std::shared_ptr<stmt>& a_statement) {
        std::shared_ptr<break_stmt> const & br_stmt = std::static_pointer_cast<break_stmt>(a_statement);
        if(m_inside_loop == false) {
            throw invalid_statement(br_stmt -> get_token(), "Unexpected break statement. A break statement can only occur within a loop.");
        }
    }

    /**
     * check_continue
     * check if a continue statement occurs inside a loop
     */
    void block_checker::check_continue(std::shared_ptr<stmt>& a_statement) {
        std::shared_ptr<continue_stmt> const & cont_stmt = std::static_pointer_cast<continue_stmt>(a_statement);
        if(m_inside_loop == false) {
            throw invalid_statement(cont_stmt -> get_token(), "Unexpected continue statement. A continue statement can only occur within a loop.");
        }
    }

    /**
     * check_pass
     * check if a pass statement occurs inside a block
     */
    void block_checker::check_pass(std::shared_ptr<stmt>& a_statement) {
        std::shared_ptr<pass_stmt> const & p_stmt = std::static_pointer_cast<pass_stmt>(a_statement);
        if(m_decls_count > 1)
            throw invalid_statement(p_stmt -> get_token(), "A pass statement cannot occur within a block with other statements.");
    }

    /**
     * check_return
     * given a statemnt, check if it is a valid return statement
     */
    void block_checker::check_return(std::shared_ptr<stmt>& a_statement, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<return_stmt> const & ret_stmt = std::static_pointer_cast<return_stmt>(a_statement);
        expression_checker expr_checker;

        if(ret_stmt -> has_expression()) {
            std::shared_ptr<expr>& ret_expr = ret_stmt -> get_expression();
            try {
                type_instance ret_instance = expr_checker.check(ret_expr, l_scope, ns_name);
                if(type_instance_strong_compare(ret_instance, m_ret_instance) == false) {
                    throw invalid_statement(ret_stmt -> get_token(), "The returned expression is of type <" + mangle_type_instance(ret_instance) + "> while the expected return type instance is <" + mangle_type_instance(m_ret_instance) + ">.");
                }
            } catch(invalid_expression err) {
                throw err;
            }            
        }
        else {
            avalon_void avl_void;
            type_instance& void_instance = avl_void.get_type_instance();
            if(type_instance_strong_compare(m_ret_instance, void_instance) == false) {
                throw invalid_statement(ret_stmt -> get_token(), "The return statement returns no expression while the expected return type instance is <" + mangle_type_instance(m_ret_instance) + ">.");
            }
        }
    }

    /**
     * check_expression
     * given a statement, check if it is a valid expression
     */
    void block_checker::check_expression(std::shared_ptr<stmt>& a_statement, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::shared_ptr<expression_stmt> const & expr_stmt = std::static_pointer_cast<expression_stmt>(a_statement);
        expression_checker expr_checker;

        try {
            expr_checker.check(expr_stmt, l_scope, ns_name);
        } catch(invalid_expression err) {
            throw err;
        }
    }
}
