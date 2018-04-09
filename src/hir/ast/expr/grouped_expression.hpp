#ifndef AVALON_HIR_AST_EXPR_GROUPED_EXPRESSION_HPP_
#define AVALON_HIR_AST_EXPR_GROUPED_EXPRESSION_HPP_

#include <memory>

#include "hir/ast/expr/expr.hpp"
#include "hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class grouped_expression : public expr {
    public:
        /**
         * the constructor expects the operand of the grouped operator
         */
        grouped_expression(token& tok, std::shared_ptr<expr>& val);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * get_value
         * returns the grouped operator operand
         */
        std::shared_ptr<expr>& get_value();

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
         * is_grouped_expression
         * return true as objects of this class are grouped expressions
         */
        virtual bool is_grouped_expression() {
            return true;
        }

    private:
        /*
         * the token to be used to show errors
         */
        token m_tok;

        /*
         * type instance of this expression
         */
        type_instance m_instance;

        /*
         * the operand to the grouped operator
         */
        std::shared_ptr<expr> m_val;
    };
}

#endif
