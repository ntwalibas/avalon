#ifndef AVALON_PROGRAM_AST_DECL_VARIABLE_HPP_
#define AVALON_PROGRAM_AST_DECL_VARIABLE_HPP_

#include <utility>
#include <memory>
#include <string>

#include "lexer/token.hpp"
#include "hir/ast/decl/decl.hpp"
#include "hir/ast/decl/type.hpp"
#include "hir/ast/expr/expr.hpp"


namespace avalon {
    /**
     * forward declarations
     */
    class vtable;
    
    class variable: public decl {
    public:
        /**
         * the constructor expects:
         * - the token with source code information, including the variable name
         * - whether the variable is mutable
         */
        variable(token& tok, bool is_mutable);
        
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
        type_instance get_type_instance() const;

        /**
         * has_type_instance
         * returns true if the variable declaration has a type instance, false otherwise
         */
        bool has_type_instance() const;

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
         * shared pointer to the type instance
         * it is a nullptr if no type instance was provided
         */
        std::shared_ptr<type_instance> m_type_instance;
        
        /*
         * shared pointer to the expression that is held in this variable
         */
        std::shared_ptr<expr> m_value;

        /*
         * whether this type is available for public use outside of the namespace where it was declared
         * true if it is, false otherwise
         */
        bool m_is_public;

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
