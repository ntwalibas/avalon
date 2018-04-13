#ifndef AVALON_MIR_AST_EXPR_MAP_EXPRESSION_HPP_
#define AVALON_MIR_AST_EXPR_MAP_EXPRESSION_HPP_

#include <utility>
#include <memory>
#include <vector>

#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_map_expression : public mir_expr {
    public:
        /**
         * the constructor expects the token with source code information
         */
        mir_map_expression(token& tok, mir_type& l_type);

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
         * add_element
         * add a new key/value pair to the map
         */
        void add_element(std::shared_ptr<mir_expr>& key, std::shared_ptr<mir_expr>& value);

        /**
         * get_elements
         * returns a vector of all the elements in the map
         */
        std::vector<std::pair<std::shared_ptr<mir_expr>, std::shared_ptr<mir_expr> > >& get_elements();
        
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
         * type of this expression
         */
        mir_type m_type;

        /*
         * a vector of expressions that make up the map
         */
        std::vector<std::pair<std::shared_ptr<mir_expr>, std::shared_ptr<mir_expr> > > m_els;
    };
}

#endif
