#ifndef AVALON_HIR_AST_DECL_STATEMENT_HPP_
#define AVALON_HIR_AST_DECL_STATEMENT_HPP_

#include <memory>

#include "hir/ast/decl/decl.hpp"
#include "hir/ast/stmt/stmt.hpp"


namespace avalon {
    class statement: public decl {
    public:
        /**
         * the constructor expects nothing
         */
        statement();

        /**
         * set_statement
         * since statements can vary but the AST can only contain declarations
         * we invoke this function to store the actual statement in this declaration.
         */
        void set_statement(std::shared_ptr<stmt>& statement);

        /**
         * get_statement
         * return the statement stored in this declaration
         */
        std::shared_ptr<stmt>& get_statement();

        /**
         * is_statement
         * a virtual function needed for casting a declaration into a statement
         */
        virtual bool is_statement() {
            return true;
        }

        /**
         * is_public
         * sets and returns a boolean indicating whether this declaration can be imported
         */
        virtual bool is_public() {
            return false;
        }
        virtual void is_public(bool public_) {
        }

        /**
         * is_reachable
         * sets and returns a boolean indicating whether this statement declaration will be executed
         */
        virtual bool is_reachable() {
            return m_reachable;
        }
        virtual void is_reachable(bool reachable) {
            m_reachable = reachable;
        }
        
        /**
         * terminates
         * sets and returns a boolean that indicates whether this statement declaration can end the normal execution of a program
         */
        virtual bool terminates() {
            return m_terminates;
        }
        virtual void terminates(bool terminates) {
            m_terminates = terminates;
        }

        /**
         * passes
         * sets and returns a boolean indicating whether this statement declaration allows the next declaration to be exeuted
         */
        virtual bool passes() {
            return m_passes;
        }
        virtual void passes(bool passes) {
            m_passes = passes;
        }

    private:
        /**
         * a pointer to the statement stored in this declaration
         */
        std::shared_ptr<stmt> m_statement;

        /**
         * this variable is true if this statement declaration will be executable
         */
        bool m_reachable;

        /**
         * this variable is true if this statement declaration can end the execution of a function
         */
        bool m_terminates;

        /**
         * this variable is true if this statement declaration will pass control to the next declaration
         */
        bool m_passes;
    };
}

#endif
