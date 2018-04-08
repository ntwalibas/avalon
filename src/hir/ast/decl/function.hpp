#ifndef AVALON_PROGRAM_AST_DECL_FUNCTION_HPP_
#define AVALON_PROGRAM_AST_DECL_FUNCTION_HPP_

#include <memory>
#include <vector>
#include <map>

#include "hir/ast/stmt/block_stmt.hpp"
#include "hir/ast/decl/variable.hpp"
#include "hir/symtable/scope.hpp"
#include "hir/ast/decl/decl.hpp"
#include "hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * foward declaration of classes needed below
     */
    class parameter_constraint;


    class function : public decl {
    public:
        /**
         * the constructor expects the token with function information
         */
        function(token& tok);

        /**
         * the constructor expects the token with function information and the validation state
         */
        function(token& tok, validation_state is_valid);

        /**
         * set_name
         * updates the name of this function
         */
        void set_name(const std::string& name);

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
        void set_scope(std::shared_ptr<scope>& l_scope);

        /**
         * get_scope
         * return the scope introduced by this function
         */
        std::shared_ptr<scope>& get_scope();

        /**
         * add_constraint
         * add a constraint to the function
         */
        void add_constraint(token& constraint);

        /**
         * get_constraints
         * returns the vector of all constraints
         */
        std::vector<token>& get_constraints();

        /**
         * add_param
         * adds a parameter to the function
         */
        void add_param(variable& param);

        /**
         * get_params
         * returns a vector of parameters
         */
        std::vector<std::pair<std::string, variable> >& get_params();
        const std::vector<std::pair<std::string, variable> >& get_params() const;

        /**
         * set_return_type_instance
         * sets the function return type instance
         */
        void set_return_type_instance(type_instance& return_type_instance);

        /**
         * get_return_type_instance
         * return a function return type instance
         */
        type_instance& get_return_type_instance();
        const type_instance& get_return_type_instance() const;

        /**
         * set_body
         * set the body of the function
         */
        void set_body(block_stmt& body);

        /**
         * get_body
         * return the body of the function
         */
        block_stmt& get_body();

        /**
         * collides_with
         * return true if this function can collide with the given function
         */
        bool collides_with(function& other);

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
        std::shared_ptr<scope> m_scope;

        /*
         * vector of type parameters if this type was parametrized
         */
        std::vector<token> m_constraints;

        /*
         * map of this function parameters' indexed by their names
         */
        std::vector<std::pair<std::string, variable> > m_params;

        /*
         * the return type of this function
         */
        type_instance m_return_type_instance;

        /*
         * the body of the function
         */
        block_stmt m_body;

        /*
         * the current validation state of the type
         */
        validation_state m_is_valid;

        /*
         * whether this type is available for public use outside of the namespace where it was declared
         * true if it is, false otherwise
         */
        bool m_is_public;

        /*
         * flag set to true if the function depends entirely on concrete types
         */
        bool m_is_concrete;

        /*
         * this variable is true if this variable declaration can end the execution of a function
         */
        bool m_terminates;
    };
    
    /**
     * operator==
     * compares two functions for equality
     */
    inline bool operator==(const function& lhs, const function& rhs) {
        const std::vector<std::pair<std::string, variable> >& lhs_params = lhs.get_params();
        const std::vector<std::pair<std::string, variable> >& rhs_params = rhs.get_params();

        // if both functions have different names, they are not equal
        if(lhs.get_name() != rhs.get_name())
            return false;

        // if they admit a different number of parameters, they are not equal
        if(lhs_params.size() != rhs_params.size())
            return false;

        // if any of their parameters have different type instances, they are not equal
        auto lhs_it = lhs_params.begin(), lhs_end = lhs_params.end();
        auto rhs_it = rhs_params.begin(), rhs_end = rhs_params.end();
        for(; lhs_it != lhs_end && rhs_it != rhs_end; ++lhs_it, ++rhs_it) {
            if(type_instance_strong_compare(lhs_it -> second.get_type_instance(), rhs_it -> second.get_type_instance()) == false)
                return false;
        }

        // if their return types are not the same, they are not equal
        if(type_instance_strong_compare(lhs.get_return_type_instance(), rhs.get_return_type_instance()) == false)
            return false;

        return true;
    }
    
    /**
     * operator!=
     * compares two functions for inequality
     */
    inline bool operator!=(const function& lhs, const function& rhs) {
        return !(lhs == rhs);
    }
}

#endif
