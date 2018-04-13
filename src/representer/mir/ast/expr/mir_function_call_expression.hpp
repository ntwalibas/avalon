#ifndef AVALON_MIR_AST_EXPR_FUNCTION_CALL_EXPRESSION_HPP_
#define AVALON_MIR_AST_EXPR_FUNCTION_CALL_EXPRESSION_HPP_

#include <memory>
#include <vector>
#include <string>

#include "representer/mir/ast/expr/mir_function_call_expression.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_function_call_expression : public mir_expr {
    public:
        /**
         * the constructor expects the token with type source information
         */
        mir_function_call_expression(const std::string& name, token& tok, mir_type& l_type);

        /**
         * get_name
         * returns the name of the function to call
         */
        const std::string& get_name() const;

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * get_type
         * returns the type of this expression
         */
        const mir_type& get_type() const;

        /**
         * add_argument
         * adds an argument to the function call
         */
        void add_argument(std::shared_ptr<mir_expr>& arg);

        /**
         * get_arguments
         * returns a vector of all the arguments of the function call
         */
        std::vector<std::shared_ptr<mir_expr> >& get_arguments();
        
        /**
         * is_function_call_expression
         * return true as objects of this class are function call expressions
         */
        virtual bool is_function_call_expression() {
            return true;
        }

    private:
        /*
         * the name of the function to call
         */
        std::string m_name;

        /*
         * the token to be used to show errors
         */
        token m_tok;

        /*
         * type of this expression
         */
        mir_type m_type;

        /*
         * vector arguments passed to the function
         */
        std::vector<std::shared_ptr<mir_expr> > m_args;
    };
}

#endif
