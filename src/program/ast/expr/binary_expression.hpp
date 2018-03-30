#ifndef AVALON_PROGRAM_AST_EXPR_BINARY_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_BINARY_EXPRESSION_HPP_

#include <memory>

#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    enum binary_expression_type {
        IS_EXPR,
        IN_EXPR,
        DOT_EXPR,
        MUL_EXPR,
        DIV_EXPR,
        MOD_EXPR,
        PLUS_EXPR,
        CAST_EXPR,
        LESS_EXPR,
        MINUS_EXPR,
        NOT_IN_EXPR,
        IS_NOT_EXPR,
        NEXT_IN_EXPR,
        PREV_IN_EXPR,
        GREATER_EXPR,
        SUBSCRIPT_EXPR,
        NOT_EQUAL_EXPR,
        LOGICAL_OR_EXPR,
        BITWISE_OR_EXPR,
        LESS_EQUAL_EXPR,
        LEFT_SHIFT_EXPR,
        RIGHT_SHIFT_EXPR,
        EQUAL_EQUAL_EXPR,
        LOGICAL_AND_EXPR,
        BITWISE_AND_EXPR,
        BITWISE_XOR_EXPR,
        GREATER_EQUAL_EXPR,
    };

    class binary_expression : public expr {
    public:
        /**
         * the constructor expects:
         * - the type of binary expression this is
         * - the left operand to the binary operator
         * - and the right operand to binary operator
         */
        binary_expression(token& tok, binary_expression_type expr_type, std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * set_type_instance
         * sets the type instance of this expression
         */
        void set_type_instance(type_instance& instance);

        /**
         * get_type_instance
         * returns the type instance of this expression
         */
        type_instance& get_type_instance();

        /**
         * has_type_instance
         * returns true if the type instance is not star
         */
        bool has_type_instance();

        /**
         * get_expression_type
         * returns the type of expression we are dealing
         */
        const binary_expression_type& get_expression_type() const;

        /**
         * get_lval
         * returns the left operand to the binary operator
         */
        std::shared_ptr<expr>& get_lval();

        /**
         * get_rval
         * returns the right operand to the binary operator
         */
        std::shared_ptr<expr>& get_rval();
        
        /**
         * is_binary_expression
         * return true as objects of this class are binary expression
         */
        virtual bool is_binary_expression() {
            return true;
        }

    private:
        /*
         * the equal sign token to be used to show errors
         */
        token m_tok;

        /*
         * type instance of this expression
         */
        type_instance m_instance;

        /*
         * the type of binary expression we have
         */
        binary_expression_type m_expr_type;

        /*
         * the left operand to the binary operator
         */
        std::shared_ptr<expr> m_lval;

        /*
         * the right operand to the binary operator
         */
        std::shared_ptr<expr> m_rval;
    };
}

#endif
