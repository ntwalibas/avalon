#ifndef AVALON_HIR_AST_EXPR_ASSIGNMENT_EXPRESSION_HPP_
#define AVALON_HIR_AST_EXPR_ASSIGNMENT_EXPRESSION_HPP_

#include <memory>

#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"
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
         * copy constructor
         */
        assignment_expression(const std::shared_ptr<assignment_expression>& assign_expr);

        /**
         * assignment copy operator
         */
        assignment_expression& operator=(const std::shared_ptr<assignment_expression>& assign_expr);

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
         * token
         * returns this expression token
         * this is useful because it prevents us from casting each expression just to display error messages
         */
        virtual const token& expr_token() const {
            return m_tok;
        }

        /**
         * expr_type_instance
         * returns the type instance of this expression, bypassing casting
         */
        virtual type_instance& expr_type_instance() {
            return m_instance;
        }

        /**
         * has_match_expression
         * returns true if the current expression depends on a match expression
         * this is useful during checking to make sure that variables and function parameters are not initialized with expressions containing match expressions
         */
        virtual bool has_match_expression() {
            if(m_lval -> has_match_expression())
                return true;

            if(m_rval -> has_match_expression())
                return true;

            return false;
        }
        
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
