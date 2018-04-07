#ifndef AVALON_PROGRAM_AST_STMT_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_STMT_EXPRESSION_HPP_

#include <memory>

#include "hir/ast/stmt/stmt.hpp"
#include "hir/ast/expr/expr.hpp"


namespace avalon {
    class expression_stmt: public stmt {
    public:
        /**
         * the constructor expects nothing
         */
        expression_stmt();

        /**
         * set_expression
         * when storing expressions in the AST, everything must decay into a declaration.
         * this function decays any of those expressions above into a simple expression statement
         * which in turn will decay into a statement declaration.
         */
        void set_expression(std::shared_ptr<expr>& expression);

        /**
         * get_expression
         * returns the specific expression stored into this expression statement
         */
        std::shared_ptr<expr>& get_expression();

        /**
         * is_expression
         * returns true since we are dealing with a statement expression
         */
        virtual bool is_expression() {
            return true;
        }

    private:
        /**
         * pointer to the expression stored into this expression statement
         */
        std::shared_ptr<expr> m_expression;
    };
}

#endif
