#include <cstddef>
#include <memory>
#include <string>
#include <vector>

/* AST */
#include "program/ast/decl/variable.hpp"
#include "program/ast/decl/function.hpp"
#include "program/ast/decl/type.hpp"
/* Symbol table */
#include "program/symtable/exceptions/symbol_already_declared.hpp"
#include "program/symtable/exceptions/symbol_can_collide.hpp"
#include "program/symtable/exceptions/symbol_not_found.hpp"
#include "program/symtable/dtable.hpp"
#include "program/symtable/ctable.hpp"
#include "program/symtable/scope.hpp"


namespace avalon {
/**
 * the constructor expects nothing
 */
scope::scope() : m_parent(nullptr), m_start_line(0), m_end_line(0) {
}
    
    /**
     * add_namespace
     */
    void scope::add_namespace(const std::string& namespace_name) {
        m_namespaces.emplace(namespace_name, namespace_name);
    }

    /**
     * get_dtable
     * returns the symbol table of declarations found in this scope
     */
    dtable& scope::get_dtable() {
        return m_dtable;
    }

    /**
     * get_ctable
     * returns the symbol table of type constructors found in this scope
     */
    ctable& scope::get_ctable() {
        return m_ctable;
    }

    /**
     * add_type
     * add a new type declaration into this scope
     */
    void scope::add_type(const std::string& ns_name, std::shared_ptr<type>& type_decl) {
        m_dtable.insert_type(ns_name, type_decl);

        // we proceed to add constructors to the this scope as well
        // we import default constructors
        std::vector<default_constructor> def_constructors = type_decl -> get_default_constructors();
        for(auto& def_constructor : def_constructors)
            add_default_constructor(ns_name, def_constructor);

        // we import record constructors
        std::vector<record_constructor> rec_constructors = type_decl -> get_record_constructors();
        for(auto& rec_constructor : rec_constructors)
            add_record_constructor(ns_name, rec_constructor);
    }

    /**
     * get_types
     * given a type name, find all the types that match the give type irrespective of arity
     */
    std::vector<std::shared_ptr<type> > scope::get_types(const std::string& ns_name, const std::string& type_name) {
        return m_dtable.get_types(ns_name, type_name);
    }

    /**
     * get_type
     * given a type instance, we find the type that builds it
     */
    std::shared_ptr<type>& scope::get_type(const std::string& ns_name, const type_instance& type_ins) {
        return m_dtable.get_type(ns_name, type_ins);
    }

    /**
     * get_type
     * given a type name and its arity, return the type that match them
     */
    std::shared_ptr<type>& scope::get_type(const std::string& ns_name, const std::string& type_name, std::size_t arity) {
        return m_dtable.get_type(ns_name, type_name, arity);
    }

    /**
     * type_exists
     * given a type declaration, find if an existing type declaration matches it
     */
    bool scope::type_exists(const std::string& ns_name, std::shared_ptr<type>& type_decl) {
        return m_dtable.type_exists(ns_name, type_decl);
    }

    /**
     * type_exists
     * given a type instance, we find if there is a corresponding type in the symbol table
     */
    bool scope::type_exists(const std::string& ns_name, const type_instance& type_ins) {
        return m_dtable.type_exists(ns_name, type_ins);
    }

    /**
     * type_exists
     * given a type name and its arity, find if an existing type declaration match the same
     */
    bool scope::type_exists(const std::string& ns_name, const std::string& type_name, std::size_t arity) {
        return m_dtable.type_exists(ns_name, type_name, arity);
    }

    /**
     * type_exists
     * given a type name and its arity, find if an existing type declaration match the same in all registered namespaces
     */
    bool scope::type_exists(const std::string& type_name, std::size_t arity) {
        return m_dtable.type_exists(type_name, arity);
    }

    /**
     * add_default_constructor
     * add a new default constructor into this symbol table
     */
    void scope::add_default_constructor(const std::string& ns_name, default_constructor& def_cons) {
        // make sure there are no namespaces that share the same with this constructor
        if(m_namespaces.count(def_cons.get_name()) > 0)
            throw symbol_can_collide("A constructor cannot have share the same name as a namespace available in this scope.");

        // make sure there are no function declarations with the same name as this constructor
        if(m_dtable.function_exists(ns_name, def_cons.get_name()))
            throw symbol_can_collide("A constructor cannot have share the same name as a function already declared in this scope.");

        // make sure there are no variable declarations with the same name as this constructor
        if(m_dtable.variable_exists(ns_name, def_cons.get_name()))
            throw symbol_can_collide("A constructor cannot have share the same name as a variable already declared in this scope.");

        m_ctable.insert_default_constructor(ns_name, def_cons);
    }

    /**
     * add_record_constructor
     * add a new record constructor into this symbol table
     */
    void scope::add_record_constructor(const std::string& ns_name, record_constructor& rec_cons) {
        // make sure there are no namespaces that share the same with this constructor
        if(m_namespaces.count(rec_cons.get_name()) > 0)
            throw symbol_can_collide("A constructor cannot have share the same name as a namespace available in this scope.");

        // make sure there are no function declarations with the same name as this constructor
        if(m_dtable.function_exists(ns_name, rec_cons.get_name()))
            throw symbol_can_collide("A constructor cannot have share the same name as a function already declared in this scope.");

        // make sure there are no variable declarations with the same name as this constructor
        if(m_dtable.variable_exists(ns_name, rec_cons.get_name()))
            throw symbol_can_collide("A constructor cannot have share the same name as a variable already declared in this scope.");

        m_ctable.insert_record_constructor(ns_name, rec_cons);
    }

    /**
     * get_default_constructor
     * given a constructor name and arity, return the matching default constructor if it exists
     */
    default_constructor& scope::get_default_constructor(const std::string& ns_name, const std::string& cons_name, std::size_t arity) {
        return m_ctable.get_default_constructor(ns_name, cons_name, arity);
    }

    /**
     * get_record_constructor
     * given a constructor name and arity, return the matching record constructor if it exists
     */
    record_constructor& scope::get_record_constructor(const std::string& ns_name, const std::string& cons_name, std::size_t arity) {
        return m_ctable.get_record_constructor(ns_name, cons_name, arity);
    }

    /**
     * default_constructor_exists
     * given a type name and its arity, find if an existing default constructor matches the same
     */
    bool scope::default_constructor_exists(const std::string& ns_name, const std::string& cons_name, std::size_t arity) {
        return m_ctable.default_constructor_exists(ns_name, cons_name, arity);
    }

    /**
     * record_constructor_exists
     * given a type name and its arity, find if an existing record constructor matches the same
     */
    bool scope::record_constructor_exists(const std::string& ns_name, const std::string& cons_name, std::size_t arity) {
        return m_ctable.record_constructor_exists(ns_name, cons_name, arity);
    }

    /**
     * add_function
     * given a namespace and a function declaration, add the declaration to the scope
     */
    void scope::add_function(const std::string& ns_name, std::shared_ptr<function>& function_decl) {
        m_dtable.insert_function(ns_name, function_decl);
    }

    /**
     * get_functions
     * given a namespace, a function name and its arity, return all functions in that namespace with the same name and arity
     */
    std::vector<std::shared_ptr<function> > scope::get_functions(const std::string& ns_name, const std::string& function_name, size_t arity) {
        return m_dtable.get_functions(ns_name, function_name, arity);
    }

    /**
     * function_exists
     * given a namespace and a function declaration, find if that function declaration already exists in that namespace
     */
    bool scope::function_exists(const std::string& ns_name, std::shared_ptr<function>& function_decl) {
        return m_dtable.function_exists(ns_name, function_decl);
    }

    /**
     * function_exists
     * given a namespace, a function name and its arity, find if we have a function with that name and arity in the current namespace
     */
    bool scope::function_exists(const std::string& ns_name, const std::string& function_name, size_t arity) {
        return m_dtable.function_exists(ns_name, function_name, arity);
    }

    /**
     * function_exists
     * given a namespace and a function name, find if the namespace contains a function with that name
     */
    bool scope::function_exists(const std::string& ns_name, const std::string& function_name) {
        return m_dtable.function_exists(ns_name, function_name);
    }

    /**
     * add_variable
     * given a namespace and a variable declaration, add the later to the former
     */
    void scope::add_variable(const std::string& ns_name, std::shared_ptr<variable>& variable_decl) {
        m_dtable.insert_variable(ns_name, variable_decl);
    }

    /**
     * get_variable
     * given a namespace and a variable name, return the corresponding variable declaration if it exists in that namespace
     */
    std::shared_ptr<variable>& scope::get_variable(const std::string& ns_name, const std::string& variable_name) {
        return m_dtable.get_variable(ns_name, variable_name);
    }

    /**
     * variable_exists
     * given a namespace and a variable declaration, find if it already exists in this namespace
     */
    bool scope::variable_exists(const std::string& ns_name, std::shared_ptr<variable>& variable_decl) {
        return m_dtable.variable_exists(ns_name, variable_decl);
    }

    /**
     * variable_exists
     * given a namespace and a variable name, find if there is a variabl declaration for it in this namespace
     */
    bool scope::variable_exists(const std::string& ns_name, const std::string& variable_name) {
        return m_dtable.variable_exists(ns_name, variable_name);
    }

    /**
     * set_parent
     * sets the parent of this scope
     */
    void scope::set_parent(std::shared_ptr<scope>& parent) {
        m_parent = parent;
    }

    /**
     * get_parent
     * returns the parent of this scope
     */
    std::shared_ptr<scope>& scope::get_parent() {
        return m_parent;
    }

    /**
     * set_start_line
     * sets the line where the scope starts
     */
    void scope::set_start_line(std::size_t start_line) {
        m_start_line = start_line;
    }

    /**
     * get_start_line
     * returns the line where the scope starts
     */
    std::size_t scope::get_start_line() {
        return m_start_line;
    }

    /**
     * set_end_line
     * sets the line where the scope ends
     */
    void scope::set_end_line(std::size_t end_line) {
        m_end_line = end_line;
    }

    /**
     * get_end_line
     * returns the line where the scope ends
     */
    std::size_t scope::get_end_line() {
        return m_end_line;
    }
}
