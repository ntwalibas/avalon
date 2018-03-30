#ifndef AVALON_PROGRAM_AST_EXPR_ASSIGNMENT_EXPRESSION_HPP_
#define AVALON_PROGRAM_AST_EXPR_ASSIGNMENT_EXPRESSION_HPP_

#include <memory>

#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class assignment_expression : public expr {
    public:
        /**
         * the constructor expects:
         * - the left side of the equal sign
         * - and the right side of the equal sign
         */
        assignment_expression(token& tok, std::shared_ptr<expr> lval, std::shared_ptr<expr> rval);

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
         * get_lval
         * returns the left side of the assignment
         */
        std::shared_ptr<expr>& get_lval();

        /**
         * get_rval
         * returns the right side of the assignment
         */
        std::shared_ptr<expr>& get_rval();

        /**
         * is_deconstruction
         * returns true if this assignment is a deconstruction expression
         */
        bool is_deconstruction();
        
        /**
         * is_assignment_expression
         * return true as objects of this class are assignment expression
         */
        virtual bool is_assignment_expression() {
            return true;
        }

    private:
        /*
         * the equal sign token to be used to show errors
         */
        token m_tok;

        /*
         * type instance of this expression
         */
        type_instance m_instance;

        /*
         * the left side of the assignment
         */
        std::shared_ptr<expr> m_lval;

        /*
         * the right side of the assignment
         */
        std::shared_ptr<expr> m_rval;
    };
}

#endif
