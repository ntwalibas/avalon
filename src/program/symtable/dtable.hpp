#ifndef AVALON_PROGRAM_SYMTABLE_DTABLE_HPP_
#define AVALON_PROGRAM_SYMTABLE_DTABLE_HPP_

#include <unordered_map>
#include <cstddef>
#include <utility>
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "program/ast/decl/type.hpp"


namespace avalon {
    enum declaration_type {
        TYPE_DECL,
        FUNCTION_DECL,
        VARIABLE_DECL
    };


    class dsymbols {
    public:
        /**
         * the constructor expects nothing
         */
        dsymbols();

        /**
         * insert_type
         * given a type declaration, this function adds it into the symbol table
         */
        void insert_type(std::shared_ptr<type>& type_decl);

        /**
         * get_types
         * given a type name, find all the types that match the give type irrespective of arity
         */
        std::vector<std::shared_ptr<type> > get_types(const std::string& name);

        /**
         * get_type
         * given a type instance, we find the type that builds it
         */
        std::shared_ptr<type>& get_type(const type_instance& type_ins);

        /**
         * get_type
         * given a type name and its arity, return the type that match them
         */
        std::shared_ptr<type>& get_type(const std::string& name, std::size_t arity);

        /**
         * type_exists
         * given a type declaration, find if an existing type declaration matches it
         */
        bool type_exists(std::shared_ptr<type>& type_decl);

        /**
         * type_exists
         * given a type instance, we find if there is a corresponding type in the symbol table
         */
        bool type_exists(const type_instance& type_ins);

        /**
         * type_exists
         * given a type name and its arity, find if an existing type declaration match the same
         */
        bool type_exists(const std::string& name, std::size_t arity);

    private:
        /*
         * map of the names of all declarations in the current scope
         * this map is used to prevent types, functions and variables from sharing the same name
         * but also to find out where to get a specific symbol when imported
         */        
        std::map<std::string, declaration_type> m_declarations;

        /*
         * map of all the type declarations in the current scope
         */
        std::map<std::pair<std::string,std::size_t>, std::shared_ptr<type> > m_types;
    };


    class dtable {
    public:
        /**
         * the constructor expects nothing
         */
        dtable();

        /**
         * insert_type
         * given a namespace and a type declaration, this function adds it into the namespace symbol table
         */
        void insert_type(const std::string& ns_name, std::shared_ptr<type>& type_decl);

        /**
         * get_types
         * given a namespace and a type name, find all the types that match the give type name; in the namespace; irrespective of arity
         */
        std::vector<std::shared_ptr<type> > get_types(const std::string& ns_name, const std::string& type_name);

        /**
         * get_type
         * given a namespace and a type instance, we find the type that builds it from the namespace
         */
        std::shared_ptr<type>& get_type(const std::string& ns_name, const type_instance& type_ins);

        /**
         * get_type
         * given a namespace and a type name and its arity, return the type that matches both from the namespace
         */
        std::shared_ptr<type>& get_type(const std::string& ns_name, const std::string& type_name, std::size_t arity);

        /**
         * type_exists
         * given a namespace and a type declaration, find if an existing type declaration matches it within the namespace
         */
        bool type_exists(const std::string& ns_name, std::shared_ptr<type>& type_decl);

        /**
         * type_exists
         * given a namespace and a type instance, we find if there is a corresponding type in the namespace symbol table
         */
        bool type_exists(const std::string& ns_name, const type_instance& type_ins);

        /**
         * type_exists
         * given a namespace and a type name and its arity, find if an existing type declaration matches them within the namespace
         */
        bool type_exists(const std::string& ns_name, const std::string& type_name, std::size_t arity);

        /**
         * type_exists
         * given a type name and its arity, find if an existing type declaration matches them within the namespace
         */
        bool type_exists(const std::string& type_name, std::size_t arity);
    
    private:
        /*
         * a map between namespaces and their respective symbol tables
         */
        std::unordered_map<std::string, dsymbols> m_symbols;
    };
}

#endif
