#ifndef AVALON_CHECKER_DECL_FUNCTION_TERMINATION_CHECKER_HPP_
#define AVALON_CHECKER_DECL_FUNCTION_TERMINATION_CHECKER_HPP_

#include <utility>
#include <memory>
#include <vector>
#include <string>

#include "program/ast/stmt/block_stmt.hpp"
#include "program/ast/decl/function.hpp"
#include "program/symtable/scope.hpp"
#include "program/ast/stmt/stmt.hpp"
#include "program/ast/decl/decl.hpp"


namespace avalon {
    class termination_checker {
    public:
        /**
         * the constructor expects nothing
         */
        termination_checker();

        /**
         * check_termination
         * this function makes sure that all declarations in the function's body are reachable and that the function terminates
         */
        void check_termination(std::shared_ptr<function>& function_decl, std::shared_ptr<scope>& l_scope);

    private:
        /**
         * a vector of pairs containing the token to the unreachable declaration and a message to the user
         */
        std::vector<std::pair<token, std::string> > m_unreachables;

        /**
         * analyze_declaration
         * this function makes sure a declaration is reachable and checks if it terminates
         */
        void analyze_declaration(std::shared_ptr<decl>& current_decl, std::shared_ptr<decl>& previous_decl);

        /**
         * analyze_statement
         * this function makes sure a statement declaration is reachable and checks if it terminates
         */
        void analyze_statement(std::shared_ptr<decl>& current_decl, std::shared_ptr<decl>& previous_decl);

        /**
         * analyze_switch
         * this function makes sure a switch statement is reachable and checks if it terminates
         */
        void analyze_switch(std::shared_ptr<stmt>& current_stmt, std::shared_ptr<decl>& current_decl, std::shared_ptr<decl>& previous_decl);

        /**
         * analyze_while
         * this function makes sure a while statement is reachable and checks if it terminates
         */
        void analyze_while(std::shared_ptr<stmt>& current_stmt, std::shared_ptr<decl>& current_decl, std::shared_ptr<decl>& previous_decl);

        /**
         * analyze_if
         * this function makes sure a if statement is reachable and checks if it terminates
         */
        void analyze_if(std::shared_ptr<stmt>& current_stmt, std::shared_ptr<decl>& current_decl, std::shared_ptr<decl>& previous_decl);

        /**
         * analyze_for
         * this function makes sure a for statement is reachable and checks if it terminates
         */
        void analyze_for(std::shared_ptr<stmt>& current_stmt, std::shared_ptr<decl>& current_decl, std::shared_ptr<decl>& previous_decl);

        /**
         * analyze_block
         * this function makes sure a block statement is reachable and checks if it terminates
         */
        bool analyze_block(std::shared_ptr<stmt>& current_stmt);
        bool analyze_block(block_stmt& blc_stmt);

        /**
         * is_reachable
         * given the previous declaration
         * this function returns a boolean indicating whether the current declaration will be reachable
         */
        bool is_reachable(std::shared_ptr<decl>& previous_decl);
    };
}

#endif
