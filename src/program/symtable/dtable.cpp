#include <cstddef>
#include <cstdint>
#include <utility>
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "program/symtable/exceptions/symbol_already_declared.hpp"
#include "program/symtable/exceptions/symbol_can_collide.hpp"
#include "program/symtable/exceptions/symbol_not_found.hpp"
#include "program/symtable/exceptions/invalid_symbol.hpp"
#include "program/symtable/dtable.hpp"
#include "program/ast/decl/type.hpp"


namespace avalon {
/**
 * the constructor expects nothing
 */
dsymbols::dsymbols() {
}

    /**
     * insert_type
     * given a type declaration, this function inserts it into the symbol table
     */
    void dsymbols::insert_type(std::shared_ptr<type>& type_decl) {
        const std::string& name = type_decl -> get_name();

        // we make sure no other type of declaration has the same name as this type declaration
        try {
            declaration_type decl_type = m_declarations.at(name);
            if(decl_type != TYPE_DECL)
                throw symbol_can_collide("There already exists another declaration with the name given to this type.");
        } catch(std::out_of_range err){            
        }

        // no other declaration shares the given name, we leave to the type to handle the rest
        std::pair<std::string, std::size_t> type_key(name, type_decl -> get_params().size());
        if(m_types.count(type_key) > 0)
            throw symbol_already_declared("There already exist a locally defined type with the same name and arity.");
        else
            m_types.insert(std::make_pair(type_key, type_decl));
    }

    /**
     * get_types
     * given a type name, find all the types that match the give type irrespective of arity
     */
    std::vector<std::shared_ptr<type> > dsymbols::get_types(const std::string& name) {
        std::vector<std::shared_ptr<type> > type_decls;
        std::pair<std::string, std::size_t> lower_limit(name, 0), upper_limit(name, SIZE_MAX);

        auto lower_bound = m_types.lower_bound(lower_limit),
             upper_bound = m_types.lower_bound(upper_limit);
        for(auto it = lower_bound; it != upper_bound; ++it)
            type_decls.push_back(it -> second);
        
        return type_decls;
    }

    /**
     * get_type
     * given a type instance, we find the type that builds it
     */
    std::shared_ptr<type>& dsymbols::get_type(const type_instance& type_ins) {
        std::pair<std::string, std::size_t> type_key(type_ins.get_name(), type_ins.get_params().size());
        try {
            return m_types.at(type_key);
        } catch(std::out_of_range err) {
            throw symbol_not_found("There is no type with the given name and arity.");
        }
    }

    /**
     * get_type
     * given a type name and its arity, return the type that match them
     */
    std::shared_ptr<type>& dsymbols::get_type(const std::string& name, std::size_t arity) {
        std::pair<std::string, std::size_t> type_key(name, arity);
        try {
            return m_types.at(type_key);
        } catch(std::out_of_range err) {
            throw symbol_not_found("There is no type with the given name and arity.");
        }
    }

    /**
     * type_exists
     * given a type declaration, find if an existing type declaration matches it
     */
    bool dsymbols::type_exists(std::shared_ptr<type>& type_decl) {
        std::pair<std::string, std::size_t> type_key(type_decl -> get_name(), type_decl -> get_params().size());
        if(m_types.count(type_key) > 0)
            return true;
        else
            return false;
    }

    /**
     * type_exists
     * given a type instance, we find if there is a corresponding type in the symbol table.
     */
    bool dsymbols::type_exists(const type_instance& type_ins) {
        std::pair<std::string, std::size_t> type_key(type_ins.get_name(), type_ins.get_params().size());
        if(m_types.count(type_key) > 0)
            return true;
        else
            return false;
    }

    /**
     * type_exists
     * given a type name and its arity, find if an existing type declaration match the same
     */
    bool dsymbols::type_exists(const std::string& name, std::size_t arity) {
        std::pair<std::string, std::size_t> type_key(name, arity);
        if(m_types.count(type_key) > 0)
            return true;
        else
            return false;
    }


/**
 * the constructor expects nothing
 */
dtable::dtable() {    
}

    /**
     * insert_type
     * given a namespace and a type declaration, this function adds it into the namespace symbol table
     */
    void dtable::insert_type(const std::string& ns_name, std::shared_ptr<type>& type_decl) {
        // we try to find if there already exists a symbol table attached to the current namespace and use it
        try {
            dsymbols& decl_symbols = m_symbols.at(ns_name);
            try {
                decl_symbols.insert_type(type_decl);
            } catch(symbol_already_declared err) {
                throw symbol_already_declared("There already exists another type with the same name and arity declared in this namespace.");
            } catch(symbol_can_collide err) {
                throw symbol_can_collide("There already exist another declaration (function or variable) with the same name in this namespace.");
            }
        } catch(std::out_of_range err) {
            // if we don't have a symbol table attached to the given namespace, we create an entry for it
            dsymbols decl_symbols;
            decl_symbols.insert_type(type_decl);            
            m_symbols.insert(std::make_pair(ns_name, decl_symbols));
        }
    }

    /**
     * get_types
     * given a namespace and a type name, find all the types that match the give type name; in the namespace; irrespective of arity
     */
    std::vector<std::shared_ptr<type> > dtable::get_types(const std::string& ns_name, const std::string& type_name) {
        try {
            dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.get_types(type_name);
        } catch(std::out_of_range err) {
            std::vector<std::shared_ptr<type> > type_decls;
            return type_decls;
        }
    }

    /**
     * get_type
     * given a namespace and a type instance, we find the type that builds it from the namespace
     */
    std::shared_ptr<type>& dtable::get_type(const std::string& ns_name, const type_instance& type_ins) {
        try {
            dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.get_type(type_ins);
        } catch(std::out_of_range err) {
            throw symbol_not_found("There is no type corresponding to the given type instance in this namespace.");
        }
    }

    /**
     * get_type
     * given a namespace and a type name and its arity, return the type that matches both from the namespace
     */
    std::shared_ptr<type>& dtable::get_type(const std::string& ns_name, const std::string& type_name, std::size_t arity) {
        try {
            dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.get_type(type_name, arity);
        } catch(std::out_of_range err) {
            throw symbol_not_found("There is no type corresponding to the given name and arity in this namespace.");
        }
    }

    /**
     * type_exists
     * given a namespace and a type declaration, find if an existing type declaration matches it within the namespace
     */
    bool dtable::type_exists(const std::string& ns_name, std::shared_ptr<type>& type_decl) {
        try {
            dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.type_exists(type_decl);
        } catch(std::out_of_range err) {
            return false;
        }
    }

    /**
     * type_exists
     * given a namespace and a type instance, we find if there is a corresponding type in the namespace symbol table
     */
    bool dtable::type_exists(const std::string& ns_name, const type_instance& type_ins) {
        try {
            dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.type_exists(type_ins);
        } catch(std::out_of_range err) {
            return false;
        }
    }

    /**
     * type_exists
     * given a namespace and a type name and its arity, find if an existing type declaration matches them within the namespace
     */
    bool dtable::type_exists(const std::string& ns_name, const std::string& type_name, std::size_t arity) {
        try {
            dsymbols& decl_symbols = m_symbols.at(ns_name);
            return decl_symbols.type_exists(type_name, arity);
        } catch(std::out_of_range err) {
            return false;
        }
    }

    /**
     * type_exists
     * given a type name and its arity, find if an existing type declaration matches them within the namespace
     */
    bool dtable::type_exists(const std::string& type_name, std::size_t arity) {
        for(auto& decl_symbols : m_symbols) {
            if(decl_symbols.second.type_exists(type_name, arity))
                return true;
        }
        return false;
    }
}
