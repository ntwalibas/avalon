#ifndef AVALON_HIR_AST_EXPR_CAST_EXPRESSION_HPP_
#define AVALON_HIR_AST_EXPR_CAST_EXPRESSION_HPP_

#include <memory>

#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class cast_expression : public expr {
    public:
        /**
         * the constructor expects the type instance to cast and the expression to cast
         */
        cast_expression(token& tok, type_instance& cast_type_instance, std::shared_ptr<expr> val);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * get_cast_type_instance
         * returns the type we are casting to
         */
        type_instance& get_cast_type_instance();

        /**
         * get_val
         * returns the cast operator operand
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
            return m_cast_type_instance;
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
         * is_cast_expression
         * return true as objects of this class are cast expressions
         */
        virtual bool is_cast_expression() {
            return true;
        }

    private:
        /*
         * the token to be used to show errors
         */
        token m_tok;

        /*
         * the type of cast expression we have
         */
        type_instance m_cast_type_instance;

        /*
         * the operand to the cast operator
         */
        std::shared_ptr<expr> m_val;
    };
}

#endif
