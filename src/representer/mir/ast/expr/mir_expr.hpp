#ifndef AVALON_MIR_AST_EXPR_HPP_
#define AVALON_MIR_AST_EXPR_HPP_


namespace avalon {
    class mir_expr {
    public:
        /* assignment expression */
        virtual bool is_assignment_expression() {
            return false;
        }
        /* grouped expression */
        virtual bool is_grouped_expression() {
            return false;
        }
        /* variable expression */
        virtual bool is_variable_expression() {
            return false;
        }
        /* function call expression */
        virtual bool is_function_call_expression() {
            return false;
        }
        /* default constructor expression */
        virtual bool is_default_constructor_expression() {
            return false;
        }
        /* record constructor expression */
        virtual bool is_record_constructor_expression() {
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
        /* match default constructor expression */
        virtual bool is_match_default_constructor_expression() {
            return false;
        }
        /* match record constructor expression */
        virtual bool is_match_record_constructor_expression() {
            return false;
        }
        /* integer expression */
        virtual bool is_integer_expression() {
            return false;
        }
        /* decimal expression */
        virtual bool is_decimal_expression() {
            return false;
        }
        /* floating point expression */
        virtual bool is_float_expression() {
            return false;
        }
        /* string expression */
        virtual bool is_string_expression() {
            return false;
        }
    };
}

#endif
