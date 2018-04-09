#ifndef AVALON_HIR_AST_EXPR_TUPLE_EXPRESSION_HPP_
#define AVALON_HIR_AST_EXPR_TUPLE_EXPRESSION_HPP_

#include <memory>
#include <vector>

#include "hir/ast/expr/expr.hpp"
#include "hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class tuple_expression : public expr {
    public:
        /**
         * the constructor expects the token with source code information
         */
        tuple_expression(token& tok);

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
         * add_element
         * add a new element to the tuple
         */
        void add_element(std::shared_ptr<expr> element);

        /**
         * get_elements
         * returns a vector of all the elements in the tuple
         */
        std::vector<std::shared_ptr<expr> >& get_elements();
        
        /**
         * is_tuple_expression
         * returns true as this is an tuple expression
         */
        virtual bool is_tuple_expression() {
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
         * a vector of expressions that make up the tuple
         */
        std::vector<std::shared_ptr<expr> > m_elements;
    };
}

#endif
