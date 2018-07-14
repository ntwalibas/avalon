#ifndef AVALON_HIR_AST_STMT_HPP_
#define AVALON_HIR_AST_STMT_HPP_

#include "representer/hir/symtable/scope.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class stmt : public std::enable_shared_from_this<stmt> {
    public:
        /**
         * copy
         * copies a statement and returns the copied one
         */
        std::shared_ptr<stmt> copy(std::shared_ptr<scope>& parent_scope);

        /**
         * token
         * returns this statement token
         */
        virtual const token& stmt_token() const = 0;

        virtual bool is_while() {
            return false;
        }
        virtual bool is_if() {
            return false;
        }
        virtual bool is_break() {
            return false;
        }
        virtual bool is_continue() {
            return false;
        }
        virtual bool is_pass() {
            return false;
        }
        virtual bool is_return() {
            return false;
        }
        virtual bool is_block() {
            return false;
        }
        virtual bool is_expression() {
            return false;
        }
    };
}

#endif
