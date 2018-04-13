#ifndef AVALON_MIR_AST_STMT_HPP_
#define AVALON_MIR_AST_STMT_HPP_


namespace avalon {
    class mir_stmt {
    public:
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
        virtual bool is_expression() {
            return false;
        }
    };
}

#endif
