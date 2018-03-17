#ifndef AVALON_PROGRAM_AST_EXPR_CAST_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_CAST_EXPRESSION_HPP_

#include <memory>

#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
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
