#ifndef AVALON_MIR_AST_EXPR_GROUPED_EXPRESSION_HPP_
#define AVALON_MIR_AST_EXPR_GROUPED_EXPRESSION_HPP_

#include <memory>

#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_grouped_expression : public mir_expr {
    public:
        /**
         * the constructor expects the operand of the grouped operator
         */
        mir_grouped_expression(token& tok, mir_type& l_type, std::shared_ptr<mir_expr>& val);

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
         * get_value
         * returns the grouped operator operand
         */
        std::shared_ptr<mir_expr>& get_value();
        
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
         * type of this expression
         */
        mir_type m_type;

        /*
         * the operand to the grouped operator
         */
        std::shared_ptr<mir_expr> m_val;
    };
}

#endif
