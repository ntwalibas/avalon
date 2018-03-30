#ifndef AVALON_PROGRAM_AST_EXPR_IDENTIFIER_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_IDENTIFIER_EXPRESSION_HPP_

#include <string>

#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
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
         * type instance of this expression
         */
        type_instance m_instance;
    };
}

#endif
