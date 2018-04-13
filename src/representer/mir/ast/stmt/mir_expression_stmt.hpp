#ifndef AVALON_MIR_AST_STMT_EXPRESSION_HPP_
#define AVALON_MIR_AST_STMT_EXPRESSION_HPP_

#include <memory>

#include "representer/mir/ast/stmt/mir_stmt.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"


namespace avalon {
    class mir_expression_stmt: public mir_stmt {
    public:
        /**
         * the constructor expects nothing
         */
        mir_expression_stmt();

        /**
         * set_expression
         * when storing expressions in the AST, everything must decay into a declaration.
         * this function decays any of those expressions above into a simple expression statement
         * which in turn will decay into a statement declaration.
         */
        void set_expression(std::shared_ptr<mir_expr>& expression);

        /**
         * get_expression
         * returns the specific expression stored into this expression statement
         */
        std::shared_ptr<mir_expr>& get_expression();

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
        std::shared_ptr<mir_expr> m_expression;
    };
}

#endif
