#ifndef AVALON_HIR_AST_EXPR_HPP_
#define AVALON_HIR_AST_EXPR_HPP_


namespace avalon {
    class expr {
    public:
        /** assignment expression **/
        virtual bool is_assignment_expression() {
            return false;
        }
        /** unary expression **/
        virtual bool is_unary_expression() {
            return false;
        }
        /** binary expression **/
        virtual bool is_binary_expression() {
            return false;
        }
        /** cast expression **/
        virtual bool is_cast_expression() {
            return false;
        }
        /** primary expressions **/
        virtual bool is_grouping_expression() {
            return false;
        }
        /* identifier expressions */
        virtual bool is_identifier_expression() {
            return false;
        }
        /* call expressions */
        virtual bool is_call_expression() {
            return false;
        }
        /* tuple expressions */
        virtual bool is_tuple_expression() {
            return false;
        }
        /* list expressions */
        virtual bool is_list_expression() {
            return false;
        }
        /* map expression */
        virtual bool is_map_expression() {
            return false;
        }
        /* literal expressions */
        virtual bool is_literal_expression() {
            return false;
        }
        /* undescore expression */
        virtual bool is_underscore_expression() {
            return false;
        }

        /*
         * NOTES:
         * UNARY AND BINARY EXPRESSIONS:
         * you will notice that even though we have many operators,
         * we do not have a one to one mapping between them and expression classes.
         * this is because many operators decay into function calls.
         * therefore, for many operators, we just need to convert them into function call expressions.
         *
         * CAST EXPRESSIONS:
         * cast expressions are a special case because even though they rely on a unary operator
         * and do decay to function calls, we still need to pass them the type to cast to and
         * this is not supposed by the unary expression class because type instances are not expressions.
         *
         * IDENTIFIED EXPRESSIONS:
         * default constructor expressions without arguments and variables expressions all are represented by
         * a single identifier. so during parsing, we have no way to know which is which.
         * we leave it to the checker to disambigue them.
         *
         * CALL EXPRESSIONS:
         * function calls and constructor expressions with arguments all have the same syntax; an identifier
         * followed by an argument list. hence when parsing, we can't dinstinguish them.
         * we group all of them into a single expression and leave it to the checker to disambigue them.
         */
    };
}

#endif
