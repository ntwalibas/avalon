#include <unordered_map>
#include <memory>
#include <string>

#include "representer/exceptions/symbol_already_declared.hpp"
#include "representer/exceptions/symbol_not_found.hpp"
#include "representer/mir/ast/decl/mir_function.hpp"
#include "representer/mir/ast/decl/mir_variable.hpp"
#include "representer/mir/symtable/mir_dtable.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"


namespace avalon {
/**
 * the constructor expects nothing
 */
mir_dsymbols::mir_dsymbols() {

}

    /*** TYPE DECLARATIONS ***/

    /**
     * insert_type
     * given a type declaration, this function adds it into the symbol table
     */
    void mir_dsymbols::insert_type(std::shared_ptr<mir_type>& type_decl) {
        const std::string& name = type_decl -> get_name();
        if(m_types.count(name) > 0)
            throw symbol_already_declared("The given type declaration already exists.");
        else
            m_types.emplace(name, type_decl);
    }

    /**
     * get_type
     * given a type name, we find the corresponding type declaration
     */
    std::shared_ptr<mir_type>& mir_dsymbols::get_type(const std::string& name) {
        try {
            return m_types.at(name);
        } catch(std::out_of_range err) {
            throw symbol_not_found("No type declaration corresponds to the given type name.");
        }
    }

    /**
     * type_exists
     * given a type declaration, find if an existing type declaration matches it
     */
    bool mir_dsymbols::type_exists(std::shared_ptr<mir_type>& type_decl) {
        if(m_types.count(type_decl -> get_name()) > 0)
            return true;
        else
            return false;
    }

    /**
     * type_exists
     * given a type name, find if an existing type declaration match the same
     */
    bool mir_dsymbols::type_exists(const std::string& name) {
        if(m_types.count(name) > 0)
            return true;
        else
            return false;
    }

    
    /*** FUNCTION DECLARATIONS ***/

    /**
     * insert_function
     * given a function, insert it into the symbol table
     */
    void mir_dsymbols::insert_function(std::shared_ptr<mir_function>& function_decl) {
        const std::string& name = function_decl -> get_name();
        if(m_functions.count(name) > 0)
            throw symbol_already_declared("The given function declaration already exists.");
        else
            m_functions.emplace(name, function_decl);
    }

    /**
     * get_function
     * given a function name, return the corresponding function
     */
    std::shared_ptr<mir_function>& mir_dsymbols::get_function(const std::string& name) {
        try {
            return m_functions.at(name);
        } catch(std::out_of_range err) {
            throw symbol_not_found("No function declaration corresponds to the given function name.");
        }
    }

    /**
     * function_exists
     * given a function declaration, find if it already exists in the symbol table
     */
    bool mir_dsymbols::function_exists(std::shared_ptr<mir_function>& function_decl) {
        if(m_functions.count(function_decl -> get_name()) > 0)
            return true;
        else
            return false;
    }

    /**
     * function_exists
     * given a name, find if there is any function by that in name in the symbol table
     */
    bool mir_dsymbols::function_exists(const std::string& name) {
        if(m_functions.count(name) > 0)
            return true;
        else
            return false;
    }


    /*** VARIABLE DECLARATIONS ***/

    /**
     * insert_variable
     * add a new variable declaration to this table
     */
    void mir_dsymbols::insert_variable(std::shared_ptr<mir_variable>& variable_decl) {
        const std::string& name = variable_decl -> get_name();
        if(m_variables.count(name) > 0)
            throw symbol_already_declared("The given variable declaration already exists.");
        else
            m_variables.emplace(name, variable_decl);
    }

    /**
     * get_variable
     * given a variable name, return the corresponding variable declaration
     */
    std::shared_ptr<mir_variable>& mir_dsymbols::get_variable(const std::string& name) {
        try {
            return m_variables.at(name);
        } catch(std::out_of_range err) {
            throw symbol_not_found("No variable declaration corresponds to the given variable name.");
        }
    }

    /**
     * variable_exists
     * given a variable declaration, find if it already exists in this table
     */
    bool mir_dsymbols::variable_exists(std::shared_ptr<mir_variable>& variable_decl) {
        if(m_variables.count(variable_decl -> get_name()) > 0)
            return true;
        else
            return false;
    }

    /**
     * variable_exists
     * given a variable name, find if there is a variabl declaration for it in this table
     */
    bool mir_dsymbols::variable_exists(const std::string& name) {
        if(m_variables.count(name) > 0)
            return true;
        else
            return false;
    }


/**
 * the constructor expects nothing
 */
mir_dtable::mir_dtable() {

}

    /**
     * insert_type
     * given a namespace and a type declaration, this function adds it into the namespace symbol table
     */
    void mir_dtable::insert_type(const std::string& ns_name, std::shared_ptr<mir_type>& type_decl) {
        // we try to find if there already exists a symbol table attached to the current namespace and use it
        try {
            mir_dsymbols& decl_symbols = m_symbols.at(ns_name);
            try {
                decl_symbols.insert_type(type_decl);
            } catch(symbol_already_declared err) {
                throw symbol_already_declared("There already exists another type with the same name declared in this namespace.");
            }
        } catch(std::out_of_range err) {
            // if we don't have a symbol table attached to the given namespace, we create an entry for it
            mir_dsymbols decl_symbols;
            decl_symbols.insert_type(type_decl);            
            m_symbols.emplace(ns_name, decl_symbols);
        }
    }

    /**
     * get_type
     * given a namespace and a type name, return the type that matches both from the namespace
     */
    std::shared_ptr<mir_type>& mir_dtable::get_type(const std::string& ns_name, const std::string& type_name) {
        try {
            mir_dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.get_type(type_name);
        } catch(std::out_of_range err) {
            throw symbol_not_found("No type declaration with the given name was found in the given namespace because the namespace doesn't exist.");
        }
    }

    /**
     * type_exists
     * given a namespace and a type declaration, find if an existing type declaration matches it within the namespace
     */
    bool mir_dtable::type_exists(const std::string& ns_name, std::shared_ptr<mir_type>& type_decl) {
        try {
            mir_dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.type_exists(type_decl);
        } catch(std::out_of_range err) {
            return false;
        }
    }

    /**
     * type_exists
     * given a namespace and a type name, find if an existing type declaration matches them within the namespace
     */
    bool mir_dtable::type_exists(const std::string& ns_name, const std::string& type_name) {
        try {
            mir_dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.type_exists(type_name);
        } catch(std::out_of_range err) {
            return false;
        }
    }

    
    /*** FUNCTION DECLARATIONS ***/

    /**
     * insert_function
     * given a namespace and a function declaration, insert the last in the symbol table
     */
    void mir_dtable::insert_function(const std::string& ns_name, std::shared_ptr<mir_function>& function_decl) {
        // we try to find if there already exists a symbol table attached to the current namespace and use it
        try {
            mir_dsymbols& decl_symbols = m_symbols.at(ns_name);
            try {
                decl_symbols.insert_function(function_decl);
            } catch(symbol_already_declared err) {
                throw symbol_already_declared("There already exist function with the same name in this namespace.");
            }
        } catch(std::out_of_range err) {
            // if we don't have a symbol table attached to the given namespace, we create an entry for it
            mir_dsymbols decl_symbols;
            decl_symbols.insert_function(function_decl);            
            m_symbols.emplace(ns_name, decl_symbols);
        }
    }

    /**
     * get_function
     * given a namespace and a function name, return all functions in that namespace with the same name and arity
     */
    std::shared_ptr<mir_function>& mir_dtable::get_function(const std::string& ns_name, const std::string& function_name) {
        try {
            mir_dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.get_function(function_name);
        } catch(std::out_of_range err) {
            throw symbol_not_found("No function declaration with the given name was found in the given namespace because the namespace doesn't exist.");
        }
    }

    /**
     * function_exists
     * given a namespace and a function declaration, find if that function declaration already exists in that namespace
     */
    bool mir_dtable::function_exists(const std::string& ns_name, std::shared_ptr<mir_function>& function_decl) {
        try {
            mir_dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.function_exists(function_decl);
        } catch(std::out_of_range err) {
            return false;
        }
    }

    /**
     * function_exists
     * given a namespace and a function name, find if the namespace contains a function with that name
     */
    bool mir_dtable::function_exists(const std::string& ns_name, const std::string& function_name) {
        try {
            mir_dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.function_exists(function_name);
        } catch(std::out_of_range err) {
            return false;
        }
    }


    /*** VARIABLE DECLARATIONS ***/

    /**
     * insert_variable
     * given a namespace and a variable declaration, add the later to the former
     */
    void mir_dtable::insert_variable(const std::string& ns_name, std::shared_ptr<mir_variable>& variable_decl) {
        // we try to find if there already exists a symbol table attached to the current namespace and use it
        try {
            mir_dsymbols& decl_symbols = m_symbols.at(ns_name);
            try {
                decl_symbols.insert_variable(variable_decl);
            } catch(symbol_already_declared err) {
                throw symbol_already_declared("There already exists another variable with the same name declared in this namespace.");
            }
        } catch(std::out_of_range err) {
            // if we don't have a symbol table attached to the given namespace, we create an entry for it
            mir_dsymbols decl_symbols;
            decl_symbols.insert_variable(variable_decl);
            m_symbols.emplace(ns_name, decl_symbols);
        }
    }

    /**
     * get_variable
     * given a namespace and a variable name, return the corresponding variable declaration if it exists in that namespace
     */
    std::shared_ptr<mir_variable>& mir_dtable::get_variable(const std::string& ns_name, const std::string& variable_name) {
        try {
            mir_dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.get_variable(variable_name);
        } catch(std::out_of_range err) {
            throw symbol_not_found("No variable declaration with the given name was found in the given namespace because the namespace doesn't exist.");
        }
    }

    /**
     * variable_exists
     * given a namespace and a variable declaration, find if it already exists in this namespace
     */
    bool mir_dtable::variable_exists(const std::string& ns_name, std::shared_ptr<mir_variable>& variable_decl) {
        try {
            mir_dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.variable_exists(variable_decl);
        } catch(std::out_of_range err) {
            return false;
        }
    }

    /**
     * variable_exists
     * given a namespace and a variable name, find if there is a variabl declaration for it in this namespace
     */
    bool mir_dtable::variable_exists(const std::string& ns_name, const std::string& variable_name) {
        try {
            mir_dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.variable_exists(variable_name);
        } catch(std::out_of_range err) {
            return false;
        }
    }
}
