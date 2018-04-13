#include <utility>
#include <cstddef>
#include <vector>
#include <string>
#include <map>

#include "representer/mir/ast/decl/mir_decl.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "representer/hir/symtable/fqn.hpp"
#include "lexer/token.hpp"


namespace avalon {
/**
 * the constructor expects:
 * - the name of the type
 * - the token with source code information
 */
mir_type::mir_type(const std::string& name, const token& tok) : m_name(name), m_tok(tok), m_namespace("*"), m_is_public(true) {
}

    /**
     * get_name
     * returns the name of the type
     */
    const std::string& mir_type::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_type::get_token() const {
        return m_tok;
    }

    /**
     * set_fqn
     * sets the fqn where this declaration was found
     */
    void mir_type::set_fqn(fqn& l_fqn) {
        m_fqn = l_fqn;
    }

    /**
     * get_fqn
     * returns the fqn where this declaration was found
     */
    fqn& mir_type::get_fqn() {
        return m_fqn;
    }

    /**
     * set_namespace
     * set the namespace where this declaration was found
     */
    void mir_type::set_namespace(const std::string& l_namespace) {
        m_namespace = l_namespace;
    }

    /**
     * get_namespace
     * returns the namespace where this declaration was found
     */
    const std::string& mir_type::get_namespace() const {
        return m_namespace;
    }

    /**
     * add_constructor
     * add a default constructor to the type
     */
    void mir_type::add_constructor(mir_default_constructor& def_cons) {
        std::pair<std::string, std::size_t> cons_key(def_cons.get_name(), def_cons.get_params().size());
        m_def_cons.emplace(cons_key, def_cons);
    }

    /**
     * add_constructor
     * add a record constructor to the type
     */
    void mir_type::add_constructor(mir_record_constructor& rec_cons) {
        std::pair<std::string, std::size_t> cons_key(rec_cons.get_name(), rec_cons.get_params().size());
        m_rec_cons.emplace(cons_key, rec_cons);
    }

    /**
     * get_mir_default_constructor
     * find the default constructor that has the given name and arity
     */
    mir_default_constructor& mir_type::get_default_constructor(const std::string& name, std::size_t arity) {
        return m_def_cons.at(std::make_pair(name, arity));
    }

    /**
     * get_mir_record_constructor
     * find the record constructor that has the given name and arity
     */
    mir_record_constructor& mir_type::get_record_constructor(const std::string& name, std::size_t arity) {
        return m_rec_cons.at(std::make_pair(name, arity));
    }

    /**
     * get_mir_default_constructors
     * returns a vector of all the default constructors that can create this type
     */
    std::vector<mir_default_constructor> mir_type::get_default_constructors() {
        std::vector<mir_default_constructor> def_conss;
        for(auto& def_cons : m_def_cons)
            def_conss.push_back(def_cons.second);
        return def_conss;
    }

    /**
     * get_mir_record_constructors
     * returns a vector of all the record constructors that can create this type
     */
    std::vector<mir_record_constructor> mir_type::get_record_constructors() {
        std::vector<mir_record_constructor> rec_conss;
        for(auto& rec_cons : m_rec_cons)
            rec_conss.push_back(rec_cons.second);
        return rec_conss;
    }


/**
 * the constructor expects:
 * - the token with the constructor name
 * - the type that this constructor contructs
 */
mir_default_constructor::mir_default_constructor(const std::string& name, token& tok, mir_type& l_type) : m_name(name), m_tok(tok), m_type(l_type) {
}

    /**
     * get_name
     * returns a string with the name of the constructor
     */
    const std::string& mir_default_constructor::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns the token that contains the name of this constructor
     */
    const token& mir_default_constructor::get_token() const {
        return m_tok;
    }

    /**
     * get_type
     * returns the type that this constructor creates
     */
    mir_type& mir_default_constructor::get_type() {
        return m_type;
    }

    /**
     * add_param
     * if the contructor is not an nullary constructor,
     * this function allows us to save types that the constructor relies on
     */
    void mir_default_constructor::add_param(mir_type& param) {
        m_params.push_back(param);
    }

    /**
     * get_params
     * returns a vector of types that make up the constructor parameters
     */
    std::vector<mir_type>& mir_default_constructor::get_params() {
        return m_params;
    }

/**
 * the constructor expects:
 * - the token with the constructor name
 * - the type that this constructor contructs
 */
mir_record_constructor::mir_record_constructor(const std::string& name, token& tok, mir_type& l_type) : m_name(name), m_tok(tok), m_type(l_type) {
}

    /**
     * get_name
     * returns a string with the name of the constructor
     */
    const std::string& mir_record_constructor::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns the token that contains the name of this constructor
     */
    const token& mir_record_constructor::get_token() const {
        return m_tok;
    }

    /**
     * get_type
     * returns the type that this constructor creates
     */
    mir_type& mir_record_constructor::get_type() {
        return m_type;
    }

    /**
     * add_param
     * this function allows us to save types that the constructor relies on
     */
    void mir_record_constructor::add_param(token& param_tok, mir_type& param_type) {
        m_params.emplace(param_tok, param_type);
    }

    /**
     * get_params
     * returns a map of types that make up the constructor parameters
     */
    std::map<token, mir_type>& mir_record_constructor::get_params() {
        return m_params;
    }
}
