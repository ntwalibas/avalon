#ifndef AVALON_PROGRAM_AST_EXPR_MAP_CONSTRUCTOR_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_MAP_CONSTRUCTOR_EXPRESSION_HPP_

#include <utility>
#include <memory>
#include <vector>

#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class map_constructor_expression : public expr {
    public:
        /**
         * the constructor expects the token with source code information
         */
        map_constructor_expression(token& tok);

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
         * add a new key/value pair to the map
         */
        void add_element(std::shared_ptr<expr>& key, std::shared_ptr<expr>& value);

        /**
         * get_elements
         * returns a vector of all the elements in the map
         */
        std::vector<std::pair<std::shared_ptr<expr>, std::shared_ptr<expr> > >& get_elements();
        
        /**
         * is_map_constructor_expression
         * returns true as this is an map expression
         */
        virtual bool is_map_constructor_expression() {
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
         * a vector of expressions that make up the map
         */
        std::vector<std::pair<std::shared_ptr<expr>, std::shared_ptr<expr> > > m_elements;
    };
}

#endif
