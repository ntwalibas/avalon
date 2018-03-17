#ifndef AVALON_PROGRAM_AST_EXPR_CALL_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_CALL_EXPRESSION_HPP_

#include <utility>
#include <memory>
#include <vector>
#include <string>

#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class call_expression : public expr {
    public:
        /**
         * the constructor expects the token with type source information
         */
        call_expression(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * get_name
         * returns the name of the function to call
         */
        const std::string& get_name() const;

        /**
         * add_argument
         * adds an argument to the function call
         */
        void add_argument(token& arg_name, std::shared_ptr<expr>& arg_value);

        /**
         * get_arguments
         * returns a vector of all the arguments of the function call
         */
        std::vector<std::pair<token, std::shared_ptr<expr> > >& get_arguments();

        /**
         * set_return_type_instance
         * set the expected return type instance of the function that corresponds to this function call
         */
        void set_return_type_instance(type_instance& return_type);

        /**
         * get_return_type_instance
         * returns the expected return type instance of the function that corresponds to this function call
         */
        type_instance& get_return_type_instance();
        
        /**
         * is_call_expression
         * return true as objects of this class are function call expressions
         */
        virtual bool is_call_expression() {
            return true;
        }

    private:
        /*
         * the token to be used to show errors
         */
        token m_tok;

        /*
         * the name of the function to call
         */
        std::string m_name;

        /*
         * vector arguments passed to the function
         */
        std::vector<std::pair<token, std::shared_ptr<expr> > > m_arguments;

        /*
         * the return type of the function call
         */
        type_instance m_return_type;
    };
}

#endif
