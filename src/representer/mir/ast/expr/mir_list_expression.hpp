#ifndef AVALON_MIR_AST_EXPR_LIST_EXPRESSION_HPP_
#define AVALON_MIR_AST_EXPR_LIST_EXPRESSION_HPP_

#include <memory>
#include <vector>

#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_list_expression : public mir_expr {
    public:
        /**
         * the constructor expects the token with source code information
         */
        mir_list_expression(token& tok, mir_type& l_type);

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
         * add a new element to the list
         */
        void add_element(std::shared_ptr<mir_expr> el);

        /**
         * get_elements
         * returns a vector of all the elements in the list
         */
        std::vector<std::shared_ptr<mir_expr> >& get_elements();
        
        /**
         * is_list_expression
         * returns true as this is an list expression
         */
        virtual bool is_list_expression() {
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
         * a vector of expressions that make up the list
         */
        std::vector<std::shared_ptr<mir_expr> > m_els;
    };
}

#endif
