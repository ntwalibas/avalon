#ifndef AVALON_MIR_AST_DECL_VARIABLE_HPP_
#define AVALON_MIR_AST_DECL_VARIABLE_HPP_

#include <memory>
#include <string>

#include "representer/mir/ast/decl/mir_decl.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/hir/symtable/fqn.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_variable: public mir_decl {
    public:
        /**
         * the constructor expects:
         * - the token with source code information, including the variable name
         * - whether the variable is mutable
         */
        mir_variable(const std::string& name, token& tok, mir_type& l_type, bool is_mutable);

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
         * get_type
         * returns the type of this variable
         */
        const mir_type& get_type() const;

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
        void set_namespace(const std::string& l_namespace);

        /**
         * get_namespace
         * returns the namespace where this declaration was found
         */
        const std::string& get_namespace() const;

        /**
         * set_value
         * called to set the value of the variable
         */
        void set_value(std::shared_ptr<mir_expr>& value);

        /**
         * get_value
         * returns the value contained within the variable
         */
        std::shared_ptr<mir_expr>& get_value();

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
         * the type of the variable
         */
        mir_type m_type;
        
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
         * shared pointer to the expression that is held in this variable
         */
        std::shared_ptr<mir_expr> m_value;

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