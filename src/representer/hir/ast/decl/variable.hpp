#ifndef AVALON_HIR_AST_DECL_VARIABLE_HPP_
#define AVALON_HIR_AST_DECL_VARIABLE_HPP_

#include <utility>
#include <memory>
#include <string>

#include "lexer/token.hpp"
#include "representer/hir/ast/decl/decl.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "representer/hir/ast/expr/expr.hpp"


namespace avalon {    
    class variable: public decl {
    public:
        /**
         * the constructor expects:
         * - the token with source code information, including the variable name
         * - whether the variable is mutable
         */
        variable(token& tok, bool is_mutable);

        /**
         * the constructor expects:
         * - the token with source code information, including the variable name
         * - whether the variable is mutable
         * - the validation state of the variable
         */
        variable(token& tok, bool is_mutable, validation_state is_valid);

        /**
         * copy constructor
         */
        variable(const std::shared_ptr<variable>& a_variable);
        
        /**
         * set_name
         * updates the name of this variable
         */
        void set_name(const std::string& name);

        /**
         * get_name
         * returns the name of the variable
         */
        const std::string& get_name() const;

        /**
         * get_token
         * returns a token with the variable source information
         */
        const token& get_token() const;

        /**
         * is_mutable
         * returns true if the variable is mutable, false otherwise
         */
        const bool is_mutable() const;

        /**
         * set_fqn
         * sets the fqn where this declaration was found
         */
        void set_fqn(fqn& l_fqn);

        /**
         * get_fqn
         * returns the fqn where this declaration was found
         */
        fqn& get_fqn();

        /**
         * set_namespace
         * set the namespace where this declaration was found
         */
        void set_namespace(const std::string& namespace_name);

        /**
         * get_namespace
         * returns the namespace where this declaration was found
         */
        const std::string& get_namespace() const;

        /**
         * set_type_instance
         * sets the type instance for this variable.
         * this function is called either directly while parsing
         * or when the variable is initialized
         */
        void set_type_instance(type_instance& instance);

        /**
         * get_type_instance
         * returns the type instance of this variable
         * throws a "type_error" exception if no type instance has been set.
         */
        type_instance& get_type_instance();
        const type_instance& get_type_instance() const;

        /**
         * has_type_instance
         * returns true if the variable declaration has a type instance, false otherwise
         */
        bool has_type_instance();

        /**
         * set_value
         * called to set the value of the variable
         * throws a "mutabity_error" exception if the variable is not mutable
         * and its value has already been set.
         */
        void set_value(std::shared_ptr<expr>& value);

        /**
         * get_value
         * returns the value contained within the variable
         */
        std::shared_ptr<expr>& get_value();

        /**
         * check_initializer
         * returns true if we can skip the variable initializer checking
         */
        void check_initializer(bool check_initializer_);
        bool check_initializer();

        /**
         * set_is_valid
         * updates the variable validation state.
         */
        void set_is_valid(validation_state state);

        /**
         * is_valid
         * given a validation state,
         * this function returns true if the variable has the given validation state.
         */
        bool is_valid(validation_state state);
        validation_state is_valid();

        /**
         * is_variable
         * a function used to allow safe casting of pointers from decl to this class
         */
        virtual bool is_variable() {
            return true;
        }

        /**
         * is_public
         * sets and returns a boolean indicating whether this declaration can be imported
         */
        virtual bool is_public() {
            return m_is_public;
        }
        virtual void is_public(bool public_) {
            m_is_public = public_;
        }

        /**
         * is_global
         * sets and returns a boolean indicating whether this variable is a global variable
         */
        bool is_global();
        void is_global(bool is_global);

        /**
         * is_used
         * sets and returns a boolean indicating whether this declaration was used anywhere
         * this is useful during code generation to emit messages about unused declarations and to avoid generating dead code
         */
        void is_used(bool used);
        bool is_used() const;

        /**
         * is_reachable
         * sets and returns a boolean indicating whether this variable declaration will be executed
         */
        virtual bool is_reachable() {
            return m_reachable;
        }
        virtual void is_reachable(bool reachable) {
            m_reachable = reachable;
        }
        
        /**
         * terminates
         * sets and returns a boolean that indicates whether this variable declaration can end the normal execution of a program
         */
        virtual bool terminates() {
            return m_terminates;
        }
        virtual void terminates(bool terminates) {
            m_terminates = terminates;
        }

        /**
         * passes
         * sets and returns a boolean indicating whether this variable declaration allows the next declaration to be exeuted
         */
        virtual bool passes() {
            return true;
        }
        virtual void passes(bool passes) {            
        }

    private:
        /*
         * name of the variable
         */
        std::string m_name;
        
        /*
         * token with source code information
         */
        token m_tok;
        
        /*
         * mutability flag
         */
        bool m_is_mutable;

        /*
         * the fqn of the compilation unit where this declaration was found
         */
        fqn m_fqn;

        /*
         * the name of the namespace where this declaration was found
         */
        std::string m_namespace;
        
        /*
         * shared pointer to the type instance
         * it is a nullptr if no type instance was provided
         */
        type_instance m_type_instance;
        
        /*
         * shared pointer to the expression that is held in this variable
         */
        std::shared_ptr<expr> m_value;

        /*
         * the current validation state of the variable
         */
        validation_state m_is_valid;

        /*
         * true if we are to skip the initializer checks
         */
        bool m_check_initializer;

        /*
         * whether this variable is available for public use outside of the namespace where it was declared
         * true if it is, false otherwise
         */
        bool m_is_public;

        /*
         * whether this variable is a global one
         */
        bool m_is_global;

        /*
         * whether this variable has been used for any expression
         */
        bool m_is_used;

        /*
         * this variable is true if this variable declaration will be executable
         */
        bool m_reachable;

        /*
         * this variable is true if this variable declaration can end the execution of a function
         */
        bool m_terminates;
    };
}


#endif
