#ifndef AVALON_PROGRAM_AST_EXPR_LITERAL_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_LITERAL_EXPRESSION_HPP_

#include <string>

#include "hir/ast/expr/expr.hpp"
#include "hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    enum literal_expression_type {
        INTEGER_EXPR,
        DECIMAL_EXPR,
        FLOATING_POINT_EXPR,
        STRING_EXPR
    };

    class literal_expression : public expr {
    public:
        /**
         * the constructor expects the token with source code information
         */
        literal_expression(token& tok, literal_expression_type expr_type, const std::string& val);

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

        /**
         * get_expression_type
         * returns the type of literal this expression holds
         */
        const literal_expression_type& get_expression_type() const;

        /**
         * get_value
         * returns the string representing the value stored within this literal
         */
        const std::string& get_value() const;
        
        /**
         * is_literal_expression
         * returns true as this is an literal expression
         */
        virtual bool is_literal_expression() {
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

        /*
         * whether the type instance is from the parser
         */
        bool m_type_instance_from_parser;

        /*
         * the type of literal expression this is
         */
        literal_expression_type m_expr_type;

        /*
         * the string representation of the literal
         */
        std::string m_val;
    };
}

#endif
