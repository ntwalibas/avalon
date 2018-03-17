#ifndef AVALON_PROGRAM_AST_DECL_FUNCTION_HPP_
#define AVALON_PROGRAM_AST_DECL_FUNCTION_HPP_

#include <memory>
#include <map>

#include "program/ast/stmt/block_stmt.hpp"
#include "program/ast/decl/variable.hpp"
#include "program/symtable/scope.hpp"
#include "program/ast/decl/decl.hpp"
#include "program/ast/decl/type.hpp"
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
        void add_constraint(parameter_constraint constraint);

        /**
         * get_constraints
         * returns the map of constraints indexed by the name of the constraint
         */
        const std::map<std::string, parameter_constraint>& get_constraints();

        /**
         * add_param
         * adds a parameter to the function
         */
        void add_param(variable& param);

        /**
         * get_params
         * returns a map of parameters indexed by their names
         */
        std::map<std::string, variable>& get_params();

        /**
         * get_params_as_vector
         * returns a map of parameters indexed by their names
         */
        std::vector<type_instance>& get_params_as_vector();

        /**
         * set_return_type_instance
         * i a return type instance was specified, we set it here
         */
        void set_return_type_instance(type_instance return_type_instance);

        /**
         * get_return_type_instance
         * if a return type instance was specified, we return it.
         * else, we throw a "type_error".
         */
        type_instance get_return_type_instance() const;

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
         * set_is_parametrized
         * if the any of the function parameters is parametrized,
         * this function marks the functions as parametrized as well
         */
        void set_is_parametrized(bool is_parametrized);

        /**
         * is_parametrized
         * returns true if this function is parametrized, false otherwise.
         */
        bool is_parametrized() const;

        /**
         * is_abstract
         * if the function's parameters don't rely on any concrete type but only on abstract types,
         * we use this function to set this flag on or return the flag
         */
        void is_abstract(bool abstract);
        bool is_abstract() const;

        /**
         * is_concrete
         * if the function's parameters rely only on concrete type,
         * we use this function to set this flag on or return the flag
         */
        void is_concrete(bool concrete);
        bool is_concrete() const;

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
         * scope introduced by the program, i.e. global scope
         */
        std::shared_ptr<scope> m_scope;

        /*
         * map of parameters' constraints index by their names this function depends on
         */
        std::map<std::string, parameter_constraint> m_constraints;

        /*
         * map of this function parameters' indexed by their names
         */
        std::map<std::string, variable> m_params;

        /*
         * a vector of parameters' types
         */
        std::vector<type_instance> m_params_vector;

        /*
         * the return type of this function if any was specified
         */
        std::shared_ptr<type_instance> m_return_type_instance;

        /*
         * the functional type created by this function declaration
         */
        std::shared_ptr<type_instance> m_instance;

        /*
         * the body of the function
         */
        block_stmt m_body;

        /*
         * if any of the function's parameters is parametrized, we mark the function as parametrized as well.
         * even though we allow parametric types, if two functions can collide parameters wise, we require that
         * the return types not be parametric.
         */
        bool m_is_parametrized;

        /*
         * flag set to true if the function depends entirely on abstract types
         */
        bool m_is_abstract;

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


    class parameter_constraint {
    public:
        /**
         * the constructor expects the token with parameter constraint source information
         */
        parameter_constraint(token tok);

        /**
         * set_name
         * updates the name of this parameter constraint
         */
        void set_name(const std::string& name);

        /**
         * get_name
         * returns the name of the parameter constraint
         */
        const std::string& get_name() const;

        /**
         * get_token
         * returns a token with parameter constraint source information
         */
        const token& get_token() const;

    private:
        /*
         * the name of the parameter constraint
         */
        std::string m_name;

        /*
         * the token with source file information about the parameter constraint
         */
        token m_tok;
    };
}

#endif
