#ifndef AVALON_HIR_AST_EXPR_MATCH_EXPRESSION_HPP_
#define AVALON_HIR_AST_EXPR_MATCH_EXPRESSION_HPP_

#include <memory>

#include "hir/ast/expr/expr.hpp"
#include "hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    enum match_expression_type {
        MATCH_EXPR,
        NOT_MATCH_EXPR,
    };

    class match_expression : public expr {
    public:
        /**
         * the constructor expects:
         * - the type of match expression this is
         * - the left operand to the match operator
         * - and the right operand to match operator
         */
        match_expression(token& tok, match_expression_type expr_type, std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * get_expression_type
         * returns the type of expression we are dealing
         */
        const match_expression_type& get_expression_type() const;

        /**
         * get_lval
         * returns the left operand to the match operator
         */
        std::shared_ptr<expr>& get_lval();

        /**
         * get_rval
         * returns the right operand to the match operator
         */
        std::shared_ptr<expr>& get_rval();
        
        /**
         * is_match_expression
         * return true as objects of this class are match expression
         */
        virtual bool is_match_expression() {
            return true;
        }

    private:
        /*
         * the equal sign token to be used to show errors
         */
        token m_tok;

        /*
         * the type of match expression we have
         */
        match_expression_type m_expr_type;

        /*
         * the left operand to the match operator
         */
        std::shared_ptr<expr> m_lval;

        /*
         * the right operand to the match operator
         */
        std::shared_ptr<expr> m_rval;
    };
}

#endif
