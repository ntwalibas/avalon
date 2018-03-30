#ifndef AVALON_PROGRAM_AST_EXPR_LIST_CONSTRUCTOR_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_LIST_CONSTRUCTOR_EXPRESSION_HPP_

#include <memory>
#include <vector>

#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class list_constructor_expression : public expr {
    public:
        /**
         * the constructor expects the token with source code information
         */
        list_constructor_expression(token& tok);

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
         * add_element
         * add a new element to the list
         */
        void add_element(std::shared_ptr<expr>& element);

        /**
         * get_elements
         * returns a vector of all the elements in the list
         */
        std::vector<std::shared_ptr<expr> >& get_elements();
        
        /**
         * is_list_constructor_expression
         * returns true as this is a list constructor expression
         */
        virtual bool is_list_constructor_expression() {
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
         * a vector of expressions that make up the list
         */
        std::vector<std::shared_ptr<expr> > m_elements;
    };
}

#endif
