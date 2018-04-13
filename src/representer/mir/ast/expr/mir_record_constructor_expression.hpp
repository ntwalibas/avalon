#ifndef AVALON_MIR_AST_EXPR_RECORD_CONSTRUCTOR_EXPRESSION_HPP_
#define AVALON_MIR_AST_EXPR_RECORD_CONSTRUCTOR_EXPRESSION_HPP_

#include <memory>
#include <string>
#include <map>

#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_record_constructor_expression : public mir_expr {
    public:
        /**
         * the constructor expects the token with type source information
         */
        mir_record_constructor_expression(const std::string& name, token& tok, mir_type& l_type);

        /**
         * get_name
         * returns the name of theconstructor
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
        mir_type& get_type();

        /**
         * add_argument
         * adds an argument to the function call
         */
        void add_argument(const std::string& arg_name, std::shared_ptr<mir_expr>& arg_val);

        /**
         * get_arguments
         * returns a map of all the arguments of the constructor
         */
        std::map<std::string, std::shared_ptr<mir_expr> >& get_arguments();
        
        /**
         * is_record_constructor_expression
         * return true as objects of this class are record constructor expressions
         */
        virtual bool is_record_constructor_expression() {
            return true;
        }

    private:
        /*
         * the name of the constructor
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
         * map arguments passed to the constructor
         */
        std::map<std::string, std::shared_ptr<mir_expr> > m_args;
    };
}

#endif