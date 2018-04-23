#ifndef AVALON_HIR_AST_EXPR_CALL_EXPRESSION_HPP_
#define AVALON_HIR_AST_EXPR_CALL_EXPRESSION_HPP_

#include <utility>
#include <memory>
#include <vector>
#include <string>

#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    enum call_expression_type {
        FUNCTION_CALL_EXPR,
        RECORD_CONSTRUCTOR_EXPR,
        DEFAULT_CONSTRUCTOR_EXPR
    };

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
         * returns the name of the function/constructor to call
         */
        const std::string& get_name() const;

        /**
         * set_type_instance
         * sets the type instance of this expression
         */
        void set_type_instance(type_instance& instance);

        /**
         * set_type_instance
         * sets the type instance of this expression with the possibility of specifying
         * if the parser (true case) set the type instance or the inference engine (false case)
         */
        void set_type_instance(type_instance& instance, bool from_parser);

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
         * type_instance_from_parser
         * returns true if the type instance was set by the parser
         * this will happen when the user specifies a type directly on an expression
         */
        bool type_instance_from_parser() const;

        /*
         * set_expression_type
         * sets the type of expression this is
         */
        void set_expression_type(call_expression_type expr_type);

        /**
         * get_expression_type
         * returns the type of expression we are dealing
         */
        const call_expression_type& get_expression_type() const;

        /**
         * add_specialization
         * add a type a type instance that corresponds to a constraint
         */
        void add_specialization(type_instance& specialization);

        /**
         * get_specializations
         * returns a vector of type instances that will be used to specialize a function against constraints
         */
        std::vector<type_instance>& get_specializations();

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
         * has_return_type_instance
         * returns true if this call expression has a return type instance specified
         */
        bool has_return_type_instance();

        /**
         * has_record_syntax
         * returns true if arguments named, false otherwise
         */
        bool has_record_syntax();

        /**
         * token
         * returns this expression token
         * this is useful because it prevents us from casting each expression just to display error messages
         */
        virtual const token& expr_token() const {
            return m_tok;
        }
        
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
         * type instance of this expression
         */
        type_instance m_instance;

        /*
         * if we have a function call, the user might need to give a list of type instances
         * that will help select the function to call as in f<t_1, t_2, ..., t_n>(...)
         * these must correspond to the list of constraints passed to the function declaration
         */
        std::vector<type_instance> m_specializations;

        /*
         * the type of call this expression is
         */
        call_expression_type m_expr_type;

        /*
         * whether the type instance is from the parser
         */
        bool m_type_instance_from_parser;

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
