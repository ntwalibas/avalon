#ifndef AVALON_PROGRAM_AST_STMT_HPP_
#define AVALON_PROGRAM_AST_STMT_HPP_


namespace avalon {
    class stmt {
    public:
        virtual bool is_switch() {
            return false;
        }
        virtual bool is_while() {
            return false;
        }
        virtual bool is_if() {
            return false;
        }
        virtual bool is_for() {
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
