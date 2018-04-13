#ifndef AVALON_MIR_AST_DECL_FUNCTION_HPP_
#define AVALON_MIR_AST_DECL_FUNCTION_HPP_

#include <memory>
#include <vector>
#include <string>

#include "representer/mir/ast/stmt/mir_block_stmt.hpp"
#include "representer/mir/ast/decl/mir_variable.hpp"
#include "representer/mir/symtable/mir_scope.hpp"
#include "representer/mir/ast/decl/mir_decl.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "representer/hir/symtable/fqn.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_function : public mir_decl {
    public:
        /**
         * the constructor expects the token with function information
         */
        mir_function(const std::string& name, token& tok, mir_type& return_type);

        /**
         * get_name
         * returns the name of the function
         */
        const std::string& get_name() const;

        /**
         * get_token
         * returns a token with function source information
         */
        const token& get_token() const;

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
         * set_scope
         * specify the scope introduced by this function
         */
        void set_scope(std::shared_ptr<mir_scope>& l_scope);

        /**
         * get_scope
         * return the scope introduced by this function
         */
        std::shared_ptr<mir_scope>& get_scope();

        /**
         * add_param
         * adds a parameter to the function
         */
        void add_param(mir_variable& param);

        /**
         * get_params
         * returns a vector of parameters
         */
        const std::vector<std::pair<std::string, mir_variable> >& get_params() const;

        /**
         * get_return_type
         * return a function return type
         */
        const mir_type& get_return_type() const;

        /**
         * set_body
         * set the body of the function
         */
        void set_body(mir_block_stmt& body);

        /**
         * get_body
         * return the body of the function
         */
        mir_block_stmt& get_body();

        /*
         * is_type
         * returns true as this declaration is indeed a function
         */
        virtual bool is_function() {
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
         * sets and returns a boolean indicating whether this function declaration will be executed
         */
        virtual bool is_reachable() {
            return true;
        }
        virtual void is_reachable(bool reachable) {
        }
        
        /**
         * sets and returns a boolean that indicates whether this function declaration will cause a function to return
         */
        virtual bool terminates() {
            return m_terminates;
        }
        virtual void terminates(bool terminates) {
            m_terminates = terminates;
        }

        /**
         * passes
         * sets and returns a boolean indicating whether this function declaration allows the next declaration to be exeuted
         */
        virtual bool passes() {
            return true;
        }
        virtual void passes(bool passes) {            
        }

    private:
        /*
         * the name of the function
         */
        std::string m_name;

        /*
         * the token with source file information about the function
         */
        token m_tok;

        /*
         * the fqn of the compilation unit where this declaration was found
         */
        fqn m_fqn;

        /*
         * the name of the namespace where this declaration was found
         */
        std::string m_namespace;

        /*
         * scope introduced by the program, i.e. global scope
         */
        std::shared_ptr<mir_scope> m_scope;

        /*
         * map of this function parameters' indexed by their names
         */
        std::vector<std::pair<std::string, mir_variable> > m_params;

        /*
         * the return type of this function
         */
        mir_type m_return_type;

        /*
         * the body of the function
         */
        mir_block_stmt m_body;

        /*
         * whether this type is available for public use outside of the namespace where it was declared
         * true if it is, false otherwise
         */
        bool m_is_public;

        /*
         * this variable is true if this variable declaration can end the execution of a function
         */
        bool m_terminates;
    };
}

#endif
