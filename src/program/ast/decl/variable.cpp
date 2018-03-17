#include <cstddef>
#include <utility>
#include <memory>
#include <string>

#include "program/ast/exceptions/type_error.hpp"
#include "program/ast/decl/variable.hpp"
#include "program/ast/decl/decl.hpp"
#include "program/ast/decl/type.hpp"
#include "program/ast/expr/expr.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects:
     * - the token with source code information, including the variable name
     * - whether the variable is mutable
     */
    variable::variable(token& tok, bool is_mutable) : m_name(tok.get_lexeme()), m_tok(tok), m_is_mutable(is_mutable), m_type_instance(nullptr), m_value(nullptr), m_reachable(false), m_terminates(false) {
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
     * set_type_instance
     * sets the type instance for this variable.
     * this function is called either directly while parsing
     * or when the variable is initialized
     */
    void variable::set_type_instance(type_instance& instance) {
        m_type_instance = std::make_shared<type_instance>(instance);
    }

    /**
     * get_type_instance
     * returns the type instance of this variable
     * throws a "type_error" exception if no type instance has been set.
     */
    type_instance variable::get_type_instance() const {
        if(m_type_instance == nullptr)
            throw type_error("Variable has no type instance set.");
        else
            return * m_type_instance;
    }

    /**
     * has_type_instance
     * returns true if the variable declaration has a type instance, false otherwise
     */
    bool variable::has_type_instance() const {
        if(m_type_instance == nullptr)
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
}
