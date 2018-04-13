#include <cstddef>
#include <memory>
#include <vector>
#include <string>

#include "representer/mir/ast/stmt/mir_block_stmt.hpp"
#include "representer/mir/ast/decl/mir_function.hpp"
#include "representer/mir/ast/decl/mir_variable.hpp"
#include "representer/mir/symtable/mir_scope.hpp"
#include "representer/mir/ast/decl/mir_decl.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "representer/hir/symtable/fqn.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with function information
     */
    mir_function::mir_function(const std::string& name, token& tok, mir_type& return_type) : m_name(name), m_tok(tok), m_namespace("*"), m_scope(nullptr), m_return_type(return_type), m_is_public(true), m_terminates(false) {
    }

    /**
     * get_name
     * returns the name of the function
     */
    const std::string& mir_function::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns a token with function source information
     */
    const token& mir_function::get_token() const {
        return m_tok;
    }

    /**
     * set_fqn
     * sets the fqn where this declaration was found
     */
    void mir_function::set_fqn(fqn& l_fqn) {
        m_fqn = l_fqn;
    }

    /**
     * get_fqn
     * returns the fqn where this declaration was found
     */
    fqn& mir_function::get_fqn() {
        return m_fqn;
    }

    /**
     * set_namespace
     * set the namespace where this declaration was found
     */
    void mir_function::set_namespace(const std::string& l_namespace) {
        m_namespace = l_namespace;
    }

    /**
     * get_namespace
     * returns the namespace where this declaration was found
     */
    const std::string& mir_function::get_namespace() const {
        return m_namespace;
    }

    /**
     * set_scope
     * specify the scope introduced by this function
     */
    void mir_function::set_scope(std::shared_ptr<mir_scope>& l_scope) {
        m_scope = l_scope;
    }

    /**
     * get_scope
     * return the scope introduced by this function
     */
    std::shared_ptr<mir_scope>& mir_function::get_scope() {
        return m_scope;
    }

    /**
     * add_param
     * adds a parameter to the function
     */
    void mir_function::add_param(mir_variable& param) {
        m_params.emplace_back(param.get_name(), param);
    }

    /**
     * get_params
     * returns a vector of parameters
     */
    const std::vector<std::pair<std::string, mir_variable> >& mir_function::get_params() const {
        return m_params;
    }

    /**
     * get_return_type
     * return a function return type
     */
    const mir_type& mir_function::get_return_type() const {
        return m_return_type;
    }

    /**
     * set_body
     * set the body of the function
     */
    void mir_function::set_body(mir_block_stmt& body) {
        m_body = body;
    }

    /**
     * get_body
     * return the body of the function
     */
    mir_block_stmt& mir_function::get_body() {
        return m_body;
    }
}
