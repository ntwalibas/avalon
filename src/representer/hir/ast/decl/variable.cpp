#include <cstddef>
#include <utility>
#include <memory>
#include <string>

#include "representer/exceptions/type_error.hpp"
#include "representer/hir/ast/decl/variable.hpp"
#include "representer/hir/ast/decl/decl.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects:
     * - the token with source code information, including the variable name
     * - whether the variable is mutable
     */
    variable::variable(token& tok, bool is_mutable) : m_name(tok.get_lexeme()), m_tok(tok), m_is_mutable(is_mutable), m_namespace("*"), m_type_instance(star_instance), m_value(nullptr), m_is_valid(UNKNOWN), m_check_initializer(true), m_is_public(true), m_is_used(false), m_reachable(false), m_terminates(false) {
    }

    /**
     * the constructor expects:
     * - the token with source code information, including the variable name
     * - whether the variable is mutable
     * - the validation state of the variable
     */
    variable::variable(token& tok, bool is_mutable, validation_state is_valid) : m_name(tok.get_lexeme()), m_tok(tok), m_is_mutable(is_mutable), m_namespace("*"), m_type_instance(star_instance), m_value(nullptr), m_is_valid(is_valid), m_is_public(true), m_is_used(false), m_reachable(false), m_terminates(false) {        
    }

    /**
     * set_name
     * updates the name of this variable
     */
    void variable::set_name(const std::string& name) {
        m_name = name;
    }

    /**
     * get_name
     * returns the name of the variable
     */
    const std::string& variable::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns a token with the variable source information
     */
    const token& variable::get_token() const {
        return m_tok;
    }

    /**
     * is_mutable
     * returns true if the variable is mutable, false otherwise
     */
    const bool variable::is_mutable() const {
        return m_is_mutable;
    }

    /**
     * set_fqn
     * sets the fqn where this declaration was found
     */
    void variable::set_fqn(fqn& l_fqn) {
        m_fqn = l_fqn;
    }

    /**
     * get_fqn
     * returns the fqn where this declaration was found
     */
    fqn& variable::get_fqn() {
        return m_fqn;
    }

    /**
     * set_namespace
     * set the namespace where this declaration was found
     */
    void variable::set_namespace(const std::string& namespace_name) {
        m_namespace = namespace_name;
    }

    /**
     * get_namespace
     * returns the namespace where this declaration was found
     */
    const std::string& variable::get_namespace() const {
        return m_namespace;
    }

    /**
     * set_type_instance
     * sets the type instance for this variable.
     * this function is called either directly while parsing
     * or when the variable is initialized
     */
    void variable::set_type_instance(type_instance& instance) {
        m_type_instance = instance;
    }

    /**
     * get_type_instance
     * returns the type instance of this variable
     * throws a "type_error" exception if no type instance has been set.
     */
    type_instance& variable::get_type_instance() {
        return m_type_instance;
    }

    const type_instance& variable::get_type_instance() const {
        return m_type_instance;
    }

    /**
     * has_type_instance
     * returns true if the variable declaration has a type instance, false otherwise
     */
    bool variable::has_type_instance() {
        if(m_type_instance.is_star())
            return false;
        else
            return true;
    }

    /**
     * set_value
     * called to set the value of the variable
     * throws a "mutabity_error" exception if the variable is not mutable
     * and its value has already been set.
     */
    void variable::set_value(std::shared_ptr<expr>& value) {
        m_value = value;
    }

    /**
     * get_value
     * returns the value contained within the variable
     */
    std::shared_ptr<expr>& variable::get_value() {
        return m_value;
    }

    /**
     * check_initializer
     * returns true if we can skip the variable initializer checking
     */
    void variable::check_initializer(bool check_initializer_) {
        m_check_initializer = check_initializer_;
    }

    bool variable::check_initializer() {
        return m_check_initializer;
    }

    /**
     * set_is_valid
     * updates the variable validation state.
     */
    void variable::set_is_valid(validation_state state) {
        m_is_valid = state;
    }

    /**
     * is_valid
     * given a validation state,
     * this function returns true if the variable has the given validation state.
     */
    bool variable::is_valid(validation_state state) {
        return m_is_valid;
    }

    /**
     * is_global
     * sets and returns a boolean indicating whether this variable is a global variable
     */
    bool variable::is_global() {
        return m_is_public;
    }
    void variable::is_global(bool is_global) {
        m_is_global = is_global;
    }

    /**
     * is_used
     * sets and returns a boolean indicating whether this declaration was used anywhere
     * this is useful during code generation to emit messages about unused declarations and to avoid generating dead code
     */
    void variable::is_used(bool used) {
        m_is_used = used;
    }

    bool variable::is_used() const {
        return m_is_used;
    }

}
