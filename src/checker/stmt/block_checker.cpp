#include <memory>
#include <vector>

/* AST */
#include "representer/hir/ast/stmt/expression_stmt.hpp"
#include "representer/hir/ast/stmt/block_stmt.hpp"
#include "representer/hir/ast/decl/statement.hpp"
#include "representer/hir/ast/decl/variable.hpp"
#include "representer/hir/ast/decl/decl.hpp"

/* Symbol table */
#include "representer/hir/symtable/scope.hpp"

/* Checker */
#include "checker/expr/expression_checker.hpp"
#include "checker/stmt/block_checker.hpp"

/* Exceptions */
#include "representer/exceptions/symbol_already_declared.hpp"
#include "checker/exceptions/invalid_expression.hpp"
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
            // if we encounter a variable declaration, we just add it to the current scope but perform checking when it is actually used
            if(block_decl -> is_variable()) {                
                std::shared_ptr<variable> variable_decl = std::static_pointer_cast<variable>(block_decl);
                try {
                    l_scope -> add_variable(ns_name, variable_decl);
                } catch(symbol_already_declared err) {
                    throw err;
                }
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
            check_return(l_stmt);
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

    }

    /**
     * check_continue
     * check if a continue statement occurs inside a loop
     */
    void block_checker::check_continue(std::shared_ptr<stmt>& a_statement) {

    }

    /**
     * check_pass
     * check if a pass statement occurs inside a block
     */
    void block_checker::check_pass(std::shared_ptr<stmt>& a_statement) {

    }

    /**
     * check_return
     * given a statemnt, check if it is a valid return statement
     */
    void block_checker::check_return(std::shared_ptr<stmt>& a_statement) {

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
