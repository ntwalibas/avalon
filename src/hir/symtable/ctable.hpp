#ifndef AVALON_PROGRAM_SYMTABLE_CTABLE_HPP_
#define AVALON_PROGRAM_SYMTABLE_CTABLE_HPP_

#include <unordered_map>
#include <cstddef>
#include <utility>
#include <string>
#include <map>

#include "hir/ast/decl/type.hpp"


namespace avalon {
    enum constructor_type {
        DEFAULT_CONS,
        RECORD_CONS,
        LIST_CONS,
        MAP_CONS
    };



    class csymbols {
    public:
        /**
         * the constructor expects nothing
         */
        csymbols();

        /**
         * insert_default_constructor
         * insert a new default constructor into this symbol table
         */
        void insert_default_constructor(default_constructor& def_cons);

        /**
         * insert_record_constructor
         * insert a new record constructor into this symbol table
         */
        void insert_record_constructor(record_constructor& rec_cons);

        /**
         * insert_list_constructor
         * insert a new list constructor into this symbol table
         */
        void insert_list_constructor(list_constructor& list_cons);

        /**
         * insert_map_constructor
         * insert a new map constructor into this symbol table
         */
        void insert_map_constructor(map_constructor& map_cons);

        /**
         * get_default_constructor
         * given a constructor name and arity, return the matching default constructor if it exists
         */
        default_constructor& get_default_constructor(const std::string& name, std::size_t arity);

        /**
         * get_record_constructor
         * given a constructor name and arity, return the matching record constructor if it exists
         */
        record_constructor& get_record_constructor(const std::string& name, std::size_t arity);

        /**
         * get_list_constructor
         * given a constructor name, return the matching list constructor if it exists
         */
        list_constructor& get_list_constructor(const std::string& name);

        /**
         * get_map_constructor
         * given a constructor name, return the matching map constructor if it exists
         */
        map_constructor& get_map_constructor(const std::string& name);

        /**
         * default_constructor_exists
         * given a constructor name and its arity, find if an existing default constructor matches the same
         */
        bool default_constructor_exists(const std::string& name, std::size_t arity);

        /**
         * record_constructor_exists
         * given a constructor name and its arity, find if an existing record constructor matches the same
         */
        bool record_constructor_exists(const std::string& name, std::size_t arity);

        /**
         * default_constructor_exists
         * given a constructor name, find if an existing default constructor matches the same
         */
        bool default_constructor_exists(const std::string& name);

        /**
         * record_constructor_exists
         * given a constructor name, find if an existing record constructor matches the same
         */
        bool record_constructor_exists(const std::string& name);

        /**
         * list_constructor_exists
         * given a constructor name, find if an existing list constructor matches the same
         */
        bool list_constructor_exists(const std::string& name);

        /**
         * map_constructor_exists
         * given a constructor name, find if an existing map constructor matches the same
         */
        bool map_constructor_exists(const std::string& name);

    private:
        /*
         * map of all the constructors in the current scope
         */
        std::map<std::pair<std::string,std::size_t>, default_constructor> m_def_constructors;
        std::map<std::pair<std::string,std::size_t>, record_constructor> m_rec_constructors;
        std::map<std::string, list_constructor> m_list_constructors;
        std::map<std::string, map_constructor> m_map_constructors;
    };

    class ctable {
    public:
        /**
         * the constructor expects nothing
         */
        ctable();

        /**
         * insert_default_constructor
         * insert a new default constructor into this symbol table
         */
        void insert_default_constructor(const std::string& ns_name, default_constructor& def_cons);

        /**
         * insert_record_constructor
         * insert a new record constructor into this symbol table
         */
        void insert_record_constructor(const std::string& ns_name, record_constructor& rec_cons);

        /**
         * insert_list_constructor
         * insert a new list constructor into this symbol table
         */
        void insert_list_constructor(const std::string& ns_name, list_constructor& list_cons);

        /**
         * insert_map_constructor
         * insert a new map constructor into this symbol table
         */
        void insert_map_constructor(const std::string& ns_name, map_constructor& map_cons);

        /**
         * get_default_constructor
         * given a constructor name and arity, return the matching default constructor if it exists
         */
        default_constructor& get_default_constructor(const std::string& ns_name, const std::string& cons_name, std::size_t arity);

        /**
         * get_record_constructor
         * given a constructor name and arity, return the matching record constructor if it exists
         */
        record_constructor& get_record_constructor(const std::string& ns_name, const std::string& cons_name, std::size_t arity);

        /**
         * get_list_constructor
         * given a constructor name, return the matching list constructor if it exists
         */
        list_constructor& get_list_constructor(const std::string& ns_name, const std::string& cons_name);

        /**
         * get_map_constructor
         * given a constructor name, return the matching map constructor if it exists
         */
        map_constructor& get_map_constructor(const std::string& ns_name, const std::string& cons_name);

        /**
         * default_constructor_exists
         * given a constructor name and its arity, find if an existing default constructor matches the same
         */
        bool default_constructor_exists(const std::string& ns_name, const std::string& cons_name, std::size_t arity);

        /**
         * record_constructor_exists
         * given a constructor name and its arity, find if an existing record constructor matches the same
         */
        bool record_constructor_exists(const std::string& ns_name, const std::string& cons_name, std::size_t arity);

        /**
         * default_constructor_exists
         * given a constructor name, find if an existing default constructor matches the same
         */
        bool default_constructor_exists(const std::string& ns_name, const std::string& cons_name);

        /**
         * record_constructor_exists
         * given a constructor name, find if an existing record constructor matches the same
         */
        bool record_constructor_exists(const std::string& ns_name, const std::string& cons_name);

        /**
         * list_constructor_exists
         * given a constructor name, find if an existing list constructor matches the same
         */
        bool list_constructor_exists(const std::string& ns_name, const std::string& cons_name);

        /**
         * map_constructor_exists
         * given a constructor name, find if an existing map constructor matches the same
         */
        bool map_constructor_exists(const std::string& ns_name, const std::string& cons_name);

    private:
        /*
         * a map between namespaces and their respective symbol tables
         */
        std::unordered_map<std::string, csymbols> m_symbols;
    };
}

#endif
