#ifndef AVALON_HIR_AST_EXPR_UNARY_EXPRESSION_HPP_
#define AVALON_HIR_AST_EXPR_UNARY_EXPRESSION_HPP_

#include <memory>

#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    enum unary_expression_type {
        PLUS_SIGN_EXPR,
        MINUS_SIGN_EXPR,
        BITWISE_NOT_EXPR,
        LOGICAL_NOT_EXPR
    };

    class unary_expression : public expr {
    public:
        /**
         * the constructor expects the type of unary expression and the operand of the unary operator
         */
        unary_expression(token& tok, unary_expression_type expr_type, std::shared_ptr<expr> val);

        /**
         * copy constructor
         */
        unary_expression(const std::shared_ptr<unary_expression>& un_expr);

        /**
         * assignment copy operator
         */
        unary_expression& operator=(const std::shared_ptr<unary_expression>& un_expr);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * get_expression_type
         * returns the type of expression we are dealing
         */
        const unary_expression_type& get_expression_type() const;

        /**
         * set_type_instance
         * sets the type instance of this expression
         */
        void set_type_instance(type_instance& instance);

        /**
         * has_type_instance
         * returns true if the type instance is not star
         */
        bool has_type_instance();

        /**
         * get_type_instance
         * returns the type instance of this expression
         */
        type_instance& get_type_instance();

        /**
         * get_val
         * returns the unary operator operand
         */
        std::shared_ptr<expr>& get_val();

        /**
         * token
         * returns this expression token
         * this is useful because it prevents us from casting each expression just to display error messages
         */
        virtual const token& expr_token() const {
            return m_tok;
        }

        /**
         * expr_type_instance
         * returns the type instance of this expression, bypassing casting
         */
        virtual type_instance& expr_type_instance() {
            return m_instance;
        }

        /**
         * has_match_expression
         * returns true if the current expression depends on a match expression
         * this is useful during checking to make sure that variables and function parameters are not initialized with expressions containing match expressions
         */
        virtual bool has_match_expression() {
            return m_val -> has_match_expression();
        }
        
        /**
         * is_unary_expression
         * return true as objects of this class are unary expressions
         */
        virtual bool is_unary_expression() {
            return true;
        }

    private:
        /*
         * the token to be used to show errors
         */
        token m_tok;

        /*
         * the type of unary expression we have
         */
        unary_expression_type m_expr_type;

        /*
         * type instance of this expression
         */
        type_instance m_instance;

        /*
         * the operand to the unary operator
         */
        std::shared_ptr<expr> m_val;
    };
}

#endif
