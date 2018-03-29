#ifndef AVALON_PROGRAM_AST_EXPR_MAP_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_MAP_EXPRESSION_HPP_

#include <utility>
#include <memory>
#include <vector>

#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class map_expression : public expr {
    public:
        /**
         * the constructor expects the token with source code information
         */
        map_expression(token& tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

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
         * is_map_expression
         * returns true as this is an map expression
         */
        virtual bool is_map_expression() {
            return true;
        }

    private:
        /*
         * the token with source code information
         */
        token m_tok;

        /*
         * a vector of expressions that make up the map
         */
        std::vector<std::pair<std::shared_ptr<expr>, std::shared_ptr<expr> > > m_elements;
    };
}

#endif
