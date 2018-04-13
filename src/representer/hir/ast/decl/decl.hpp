#ifndef AVALON_HIR_AST_DECL_HPP_
#define AVALON_HIR_AST_DECL_HPP_


namespace avalon {
    /*
     * checking data
     * valid    : the object is in a consistent state
     * invalid  : the object failed type checking
     * unknown  : the object has not been type checked yet
     */
    enum validation_state {
        VALID,
        INVALID,
        UNKNOWN
    };


    class decl {
    public:
        virtual bool is_import() {
            return false;
        }
        virtual bool is_namespace() {
            return false;
        }
        virtual bool is_type() {
            return false;
        }
        virtual bool is_function() {
            return false;
        }
        virtual bool is_variable() {
            return false;
        }
        virtual bool is_statement() {
            return false;
        }

        /**
         * is_public
         * sets and returns a boolean indicating whether this declaration can be imported
         */
        virtual bool is_public() = 0;
        virtual void is_public(bool public_) = 0;

        /**
         * is_reachable
         * sets and returns a boolean indicating whether this declaration will be executed
         */
        virtual bool is_reachable() = 0;
        virtual void is_reachable(bool reachable) = 0;

        /**
         * terminates
         * sets and returns a boolean indicating whether this declaration will cause a function to return
         */
        virtual bool terminates() = 0;
        virtual void terminates(bool terminates) = 0;

        /**
         * passes
         * sets and returns a boolean indicating whether this declaration allows the next declaration to be exeuted
         */
        virtual bool passes() = 0;
        virtual void passes(bool passes) = 0;
    };
}

#endif
