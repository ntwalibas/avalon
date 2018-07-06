#ifndef AVALON_HIR_AST_EXPR_MAP_EXPRESSION_HPP_
#define AVALON_HIR_AST_EXPR_MAP_EXPRESSION_HPP_

#include <utility>
#include <memory>
#include <vector>

#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class map_expression : public expr {
    public:
        /**
         * the constructor expects the token with source code information
         */
        map_expression(token& tok);

        /**
         * copy constructor
         */
        map_expression(const std::shared_ptr<map_expression>& map_expr);

        /**
         * assignment copy operator
         */
        map_expression& operator=(const std::shared_ptr<map_expression>& map_expr);

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
         * add a new key/value pair to the map
         */
        void add_element(std::shared_ptr<expr>& key, std::shared_ptr<expr>& value);

        /**
         * get_elements
         * returns a vector of all the elements in the map
         */
        std::vector<std::pair<std::shared_ptr<expr>, std::shared_ptr<expr> > >& get_elements();

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
            for(const auto& element : m_elements) {
                if(element.first -> has_match_expression() || element.second -> has_match_expression())
                    return true;
            }

            return false;
        }
        
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
         * type instance of this expression
         */
        type_instance m_instance;

        /*
         * whether the type instance is from the parser
         */
        bool m_type_instance_from_parser;

        /*
         * a vector of expressions that make up the map
         */
        std::vector<std::pair<std::shared_ptr<expr>, std::shared_ptr<expr> > > m_elements;
    };
}

#endif
