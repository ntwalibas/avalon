#include <algorithm>
#include <cstddef>
#include <utility>
#include <memory>
#include <vector>
#include <map>

#include "hir/ast/exceptions/type_error.hpp"
#include "hir/ast/stmt/block_stmt.hpp"
#include "hir/ast/decl/variable.hpp"
#include "hir/ast/decl/function.hpp"
#include "hir/symtable/scope.hpp"
#include "hir/ast/decl/decl.hpp"
#include "hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
static bool instances_collide(std::vector<type_instance>& this_instances, std::vector<type_instance>& that_instances) {
    if(this_instances.size() != that_instances.size())
        return false;

    auto this_it = this_instances.begin(), this_end = this_instances.end();
    auto that_it = that_instances.begin(), that_end = that_instances.end();

    for(; this_it != this_end && that_it != that_end; ++this_it, ++that_it) {
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

static bool params_collide(std::vector<std::pair<std::string, variable> >& this_params, std::vector<std::pair<std::string, variable> >& that_params) {
    std::vector<type_instance> this_instances;
    std::vector<type_instance> that_instances;

    auto this_it = this_params.begin(), this_end = this_params.end();
    auto that_it = that_params.begin(), that_end = that_params.end();
    for(; this_it != this_end && that_it != that_end; ++this_it, ++that_it) {
        this_instances.push_back(this_it -> second.get_type_instance());
        that_instances.push_back(that_it -> second.get_type_instance());
    }

    return instances_collide(this_instances, that_instances);
}

/**
 * the constructor expects the token with function information
 */
function::function(token& tok) : m_name(tok.get_lexeme()), m_tok(tok), m_terminates(false) {
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
     * set_fqn
     * sets the fqn where this declaration was found
     */
    void function::set_fqn(fqn& l_fqn) {
        m_fqn = l_fqn;
    }

    /**
     * get_fqn
     * returns the fqn where this declaration was found
     */
    fqn& function::get_fqn() {
        return m_fqn;
    }

    /**
     * set_namespace
     * set the namespace where this declaration was found
     */
    void function::set_namespace(const std::string& namespace_name) {
        m_namespace = namespace_name;
    }

    /**
     * get_namespace
     * returns the namespace where this declaration was found
     */
    const std::string& function::get_namespace() const {
        return m_namespace;
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
    void function::add_constraint(token& constraint) {
        m_constraints.push_back(constraint);
    }

    /**
     * get_constraints
     * returns the vector of all constraints
     */
    std::vector<token>& function::get_constraints() {
        return m_constraints;
    }

    /**
     * add_param
     * adds a parameter to the function
     */
    void function::add_param(variable& param) {
        m_params.emplace_back(std::make_pair(param.get_name(), param));
    }

    /**
     * get_params
     * returns a map of parameters indexed by their names
     */
    std::vector<std::pair<std::string, variable> >& function::get_params() {
        return m_params;
    }

    const std::vector<std::pair<std::string, variable> >& function::get_params() const {
        return m_params;
    }

    /**
     * set_return_type_instance
     * sets the function return type instance
     */
    void function::set_return_type_instance(type_instance& return_type_instance) {
        m_return_type_instance = return_type_instance;
    }

    /**
     * get_return_type_instance
     * return a function return type instance
     */
    type_instance& function::get_return_type_instance() {
        return m_return_type_instance;
    }

    const type_instance& function::get_return_type_instance() const {
        return m_return_type_instance;
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
     * collides_with
     * return true if this function can collide with the given function
     */
    bool function::collides_with(function& that) {
        std::vector<std::pair<std::string, variable> >& this_params = get_params();
        std::vector<std::pair<std::string, variable> >& that_params = that.get_params();
        if(this_params.size() != that_params.size())
            return false;

        if(params_collide(this_params, that_params)) {
            if(type_instance_strong_compare(m_return_type_instance, that.get_return_type_instance()))
                return true;
            return false;
        }
        
        return false;
    }
}
