#ifndef AVALON_MIR_AST_EXPR_ASSIGNMENT_EXPRESSION_HPP_
#define AVALON_MIR_AST_EXPR_ASSIGNMENT_EXPRESSION_HPP_

#include <memory>

#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_assignment_expression : public mir_expr {
    public:
        /**
         * the constructor expects:
         * - the left side of the equal sign
         * - and the right side of the equal sign
         */
        mir_assignment_expression(token& tok, mir_type& l_type, std::shared_ptr<mir_expr> lval, std::shared_ptr<mir_expr> rval);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * get_type
         * returns the type of this expression
         */
        mir_type& get_type();

        /**
         * get_lval
         * returns the left side of the assignment
         */
        std::shared_ptr<mir_expr>& get_lval();

        /**
         * get_rval
         * returns the right side of the assignment
         */
        std::shared_ptr<mir_expr>& get_rval();
        
        /**
         * is_assignment_expression
         * return true as objects of this class are assignment expression
         */
        virtual bool is_assignment_expression() {
            return true;
        }

    private:
        /*
         * the equal sign token to be used to show errors
         */
        token m_tok;

        /*
         * type of this expression
         */
        mir_type m_type;

        /*
         * the left side of the assignment
         */
        std::shared_ptr<mir_expr> m_lval;

        /*
         * the right side of the assignment
         */
        std::shared_ptr<mir_expr> m_rval;
    };
}

#endif
