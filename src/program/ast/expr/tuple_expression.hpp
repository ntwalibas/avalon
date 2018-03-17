#ifndef AVALON_PROGRAM_AST_EXPR_TUPLE_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_TUPLE_EXPRESSION_HPP_

#include <memory>
#include <vector>

#include "program/ast/expr/expr.hpp"
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
         * a vector of expressions that make up the tuple
         */
        std::vector<std::shared_ptr<expr> > m_elements;
    };
}

#endif
