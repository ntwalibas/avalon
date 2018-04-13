#ifndef AVALON_MIR_AST_EXPR_FLOAT_EXPRESSION_HPP_
#define AVALON_MIR_AST_EXPR_FLOAT_EXPRESSION_HPP_

#include <string>

#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_float_expression : public mir_expr {
    public:
        /**
         * the constructor expects the token with type source information
         */
        mir_float_expression(token& tok, mir_type& l_type, const std::string& val);

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
         * returns the string representation of the float expression
         */
        const std::string& get_value() const;
        
        /**
         * is_float_expression
         * return true as objects of this class are float expressions
         */
        virtual bool is_float_expression() {
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
         * string representation of the float
         */
        std::string m_val;
    };
}

#endif
