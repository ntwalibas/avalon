#ifndef AVALON_HIR_AST_EXPR_IDENTIFIER_EXPRESSION_HPP_
#define AVALON_HIR_AST_EXPR_IDENTIFIER_EXPRESSION_HPP_

#include <string>

#include "hir/ast/expr/expr.hpp"
#include "hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class identifier_expression : public expr {
    public:
        /**
         * the constructor expects the token with source code information
         */
        identifier_expression(token& tok);

        /**
         * get_token
         * returns the name of the identifier
         */
        const std::string& get_name() const;

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
         * is_identifier_expression
         * returns true as this is an identifier expression
         */
        virtual bool is_identifier_expression() {
            return true;
        }

    private:
        /*
         * the name of the identifier expression
         */
        std::string m_name;
        
        /*
         * the token with source code information
         */
        token m_tok;

        /*
         * whether the type instance is from the parser
         */
        bool m_type_instance_from_parser;

        /*
         * type instance of this expression
         */
        type_instance m_instance;
    };
}

#endif
