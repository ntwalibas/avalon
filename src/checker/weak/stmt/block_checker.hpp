#ifndef AVALON_CHECKER_LAX_STMT_BLOCK_CHECKER_HPP_
#define AVALON_CHECKER_LAX_STMT_BLOCK_CHECKER_HPP_

#include <memory>
#include <string>

#include "program/ast/stmt/block_stmt.hpp"
#include "program/symtable/scope.hpp"
#include "program/ast/decl/decl.hpp"


namespace avalon {
    class block_checker {
    public:
        /**
         * the constructor expects nothing
         */
        block_checker();

        /**
         * check
         * checking a blocks statement entails the following:
         * - a block can only contain variable and statement declarations
         * - if a block contains a pass statement, it cannot contain any other type of statement or declaration
         * - if a break or continue statement is encountered, they must be within a loop
         * - all declarations within a block must be valid
         */
        void check(block_stmt& l_statement, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

    private:
        /**
         * check_variable
         * given a variable declaration in the block, check if it is valid
         */
        void check_variable(std::shared_ptr<decl>& declaration, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_statement
         * given a statement declaration, check if it is valid
         */
        void check_statement(std::shared_ptr<decl>& declaration, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /*
         * number of declarations in the block
         * this is used when checking a pass statement since it can only the only statement in a block if it appears
         */
        size_t m_decls_count;

        /*
         * flag set to true when we are inside a loop
         * when checking break and continue statements, we need to know that we are inside a loop
         */
        bool m_inside_loop;

        /**
         * check_switch
         * given a statement, check if it a valid switch statement
         */
        void check_switch(std::shared_ptr<stmt>& a_statement);

        /**
         * check_while
         * given a statement, check if it is a vali while statement
         */
        void check_while(std::shared_ptr<stmt>& a_statement);

        /**
         * check_if
         * given a statement, check if it is a valid if statement
         */
        void check_if(std::shared_ptr<stmt>& a_statement);

        /**
         * check_for
         * given a statement, check if it is a valid for statement
         */
        void check_for(std::shared_ptr<stmt>& a_statement);

        /**
         * check_break
         * check if a break statement occurs inside a loop
         */
        void check_break(std::shared_ptr<stmt>& a_statement);

        /**
         * check_continue
         * check if a continue statement occurs inside a loop
         */
        void check_continue(std::shared_ptr<stmt>& a_statement);

        /**
         * check_pass
         * check if a pass statement occurs inside a block
         */
        void check_pass(std::shared_ptr<stmt>& a_statement);

        /**
         * check_return
         * given a statemnt, check if it is a valid return statement
         */
        void check_return(std::shared_ptr<stmt>& a_statement);

        /**
         * check_expression
         * given a statement, check if it is a valid expression
         */
        void check_expression(std::shared_ptr<stmt>& a_statement, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
    };
}

#endif
