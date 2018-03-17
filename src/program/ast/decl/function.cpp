#include <algorithm>
#include <cstddef>
#include <utility>
#include <memory>
#include <map>

#include "program/ast/exceptions/type_error.hpp"
#include "program/ast/stmt/block_stmt.hpp"
#include "program/ast/decl/variable.hpp"
#include "program/ast/decl/function.hpp"
#include "program/symtable/scope.hpp"
#include "program/ast/decl/decl.hpp"
#include "program/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
static bool instances_collide(std::vector<type_instance>& this_instances, std::vector<type_instance>& that_instances) {
    for(
        auto this_it = this_instances.begin(), this_end = this_instances.end(),
        that_it = that_instances.begin(), that_end = that_instances.end()
        ; this_it != this_end && that_it != that_end
        ; ++this_it, ++that_it
    ) {
        if(type_instance_strong_compare(* this_it, * that_it) == false) {
            if(type_instance_weak_compare(* this_it, * that_it) == false)
                return false;

            std::vector<type_instance>& this_params = this_it -> get_params();
            std::vector<type_instance>& that_params = that_it -> get_params();
            if(instances_collide(this_params, that_params) == false)
                return false;
        }
    }

    return true;
}

/**
 * the constructor expects the token with function information
 */
function::function(token& tok) : m_name(tok.get_lexeme()), m_tok(tok), m_return_type_instance(nullptr), m_instance(nullptr), m_is_parametrized(false), m_is_abstract(true), m_is_concrete(true), m_terminates(false) {    
}

    /**
     * set_name
     * updates the name of this function
     */
    void function::set_name(const std::string& name) {
        m_name = name;
    }

    /**
     * get_name
     * returns the name of the function
     */
    const std::string& function::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns a token with function source information
     */
    const token& function::get_token() const {
        return m_tok;
    }

    /**
     * set_scope
     * specify the scope introduced by this function
     */
    void function::set_scope(std::shared_ptr<scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope introduced by this function
     */
    std::shared_ptr<scope>& function::get_scope() {
        return m_scope;
    }

    /**
     * add_constraint
     * add a constraint to the function
     */
    void function::add_constraint(parameter_constraint constraint) {
        m_constraints.emplace(constraint.get_name(), constraint);
    }

    /**
     * get_constraints
     * returns the map of constraints indexed by the name of the constraint
     */
    const std::map<std::string, parameter_constraint>& function::get_constraints() {
        return m_constraints;
    }

    /**
     * add_param
     * adds a parameter to the function
     */
    void function::add_param(variable& param) {
        m_params.emplace(param.get_name(), param);
    }

    /**
     * get_params
     * returns a map of parameters indexed by their names
     */
    std::map<std::string, variable>& function::get_params() {
        return m_params;
    }

    /**
     * get_params_as_vector
     * returns a map of parameters indexed by their names
     */
    std::vector<type_instance>& function::get_params_as_vector() {
        if(m_params_vector.empty())
            for(auto it = m_params.begin(), end = m_params.end(); it != end; ++it)
                m_params_vector.push_back(it -> second.get_type_instance());

        return m_params_vector;
    }

    /**
     * set_return_type_instance
     * i a return type instance was specified, we set it here
     */
    void function::set_return_type_instance(type_instance return_type_instance) {
        m_return_type_instance = std::make_shared<type_instance>(return_type_instance);
    }

    /**
     * get_return_type_instance
     * if a return type instance was specified, we return it.
     * else, we throw a "type_error".
     */
    type_instance function::get_return_type_instance() const {
        if(m_return_type_instance == nullptr)
            throw type_error("No return type instance was provided for this function.");
        else
            return * m_return_type_instance;
    }

    /**
     * set_body
     * set the body of the function
     */
    void function::set_body(block_stmt& body) {
        m_body = body;
    }

    /**
     * get_body
     * return the body of the function
     */
    block_stmt& function::get_body() {
        return m_body;
    }

    /**
     * set_is_parametrized
     * if the any of the function parameters is parametrized,
     * this function marks the functions as parametrized as well
     */
    void function::set_is_parametrized(bool is_parametrized) {
        m_is_parametrized = is_parametrized;
    }

    /**
     * is_parametrized
     * returns true if this type instance is parametrized, false otherwise.
     */
    bool function::is_parametrized() const {
        return m_is_parametrized;
    }

    /**
     * is_abstract
     * if the function's parameters don't rely on any concrete type but only on abstract types,
     * we use this function to set this flag on or return the flag
     */
    void function::is_abstract(bool abstract) {
        m_is_abstract = abstract;
    }

    bool function::is_abstract() const {
        return m_is_abstract;
    }

    /**
     * is_concrete
     * if the function's parameters rely only on concrete type,
     * we use this function to set this flag on or return the flag
     */
    void function::is_concrete(bool concrete) {
        m_is_concrete = concrete;
    }

    bool function::is_concrete() const {
        return m_is_concrete;
    }

    /**
     * collides_with
     * return true if this function can collide with the given function
     */
    bool function::collides_with(function& that) {
        if(m_is_abstract) {
            if(that.is_abstract())
                if(type_instance_strong_compare(* m_return_type_instance, that.get_return_type_instance()))
                    return true;
            return false;
        }

        std::vector<type_instance>& this_instances = get_params_as_vector();
        std::vector<type_instance>& that_instances = that.get_params_as_vector();
        if(this_instances.size() != that_instances.size())
            return false;

        if(instances_collide(this_instances, that_instances)) {
            if(type_instance_strong_compare(* m_return_type_instance, that.get_return_type_instance()))
                return true;
            return false;
        }

        return false;
    }

/**
 * the constructor expects the token with function information
 */
parameter_constraint::parameter_constraint(token tok) : m_name(tok.get_lexeme()), m_tok(tok) {
}

    /**
     * set_name
     * updates the name of this parameter constraint
     */
    void parameter_constraint::set_name(const std::string& name) {
        m_name = name;
    }

    /**
     * get_name
     * returns the name of the parameter constraint
     */
    const std::string& parameter_constraint::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns a token with parameter constraint source information
     */
    const token& parameter_constraint::get_token() const {
        return m_tok;
    }
}
