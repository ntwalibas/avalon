#ifndef AVALON_HIR_AST_EXPR_UNDERSCORE_EXPRESSION_HPP_
#define AVALON_HIR_AST_EXPR_UNDERSCORE_EXPRESSION_HPP_

#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class underscore_expression : public expr {
    public:
        /**
         * the constructor expects the token associated with this expression
         */
        underscore_expression(token& tok);

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
         * token
         * returns this expression token
         * this is useful because it prevents us from casting each expression just to display error messages
         */
        virtual const token& expr_token() const {
            return m_tok;
        }
        
        /**
         * is_underscore_expression
         * this function is used during deconstruction assignment to find out if we have an underscore expression
         */
        virtual bool is_underscore_expression() {
            return true;
        }

    private:
        /*
         * the token with source code information
         */
        token m_tok;

        /*
         * type instance of this expression
         */
        type_instance m_instance;
    };
}

#endif
