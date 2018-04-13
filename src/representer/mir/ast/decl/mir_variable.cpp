#include <cstddef>
#include <memory>
#include <string>

#include "representer/mir/ast/decl/mir_variable.hpp"
#include "representer/mir/ast/decl/mir_decl.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/hir/symtable/fqn.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects:
     * - the token with source code information, including the variable name
     * - whether the variable is mutable
     */
    mir_variable::mir_variable(const std::string& name, token& tok, mir_type& l_type, bool is_mutable) : m_name(name), m_tok(tok), m_type(l_type), m_is_mutable(is_mutable), m_namespace("*"), m_value(nullptr) {        
    }

    /**
     * get_name
     * returns the name of the variable
     */
    const std::string& mir_variable::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns a token with the variable source information
     */
    const token& mir_variable::get_token() const {
        return m_tok;
    }

    /**
     * get_type
     * returns the type of this variable
     */
    const mir_type& mir_variable::get_type() const {
        return m_type;
    }

    /**
     * is_mutable
     * returns true if the variable is mutable, false otherwise
     */
    const bool mir_variable::is_mutable() const {
        return m_is_mutable;
    }

    /**
     * set_fqn
     * sets the fqn where this declaration was found
     */
    void mir_variable::set_fqn(fqn& l_fqn) {
        m_fqn = l_fqn;
    }

    /**
     * get_fqn
     * returns the fqn where this declaration was found
     */
    fqn& mir_variable::get_fqn() {
        return m_fqn;
    }

    /**
     * set_namespace
     * set the namespace where this declaration was found
     */
    void mir_variable::set_namespace(const std::string& l_namespace) {
        m_namespace = l_namespace;
    }

    /**
     * get_namespace
     * returns the namespace where this declaration was found
     */
    const std::string& mir_variable::get_namespace() const {
        return m_namespace;
    }

    /**
     * set_value
     * called to set the value of the variable
     */
    void mir_variable::set_value(std::shared_ptr<mir_expr>& value) {
        m_value = value;
    }

    /**
     * get_value
     * returns the value contained within the variable
     */
    std::shared_ptr<mir_expr>& mir_variable::get_value() {
        return m_value;
    }
}
