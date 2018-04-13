#ifndef AVALON_MIR_SYMTABLE_CTABLE_HPP_
#define AVALON_MIR_SYMTABLE_CTABLE_HPP_

#include <unordered_map>
#include <cstddef>
#include <utility>
#include <string>
#include <map>

#include "representer/mir/ast/decl/mir_type.hpp"


/**
 * NOTE: This file is exactly the same as "representer/hir/symtable/ctable.hpp".
 * In future version, both must be replaced with one file that uses templates.
 * For now, I'm just concerned with getting things to work and have no desire to complicate my life.
 */


namespace avalon {
    class mir_csymbols {
    public:
        /**
         * the constructor expects nothing
         */
        mir_csymbols();

        /**
         * insert_default_constructor
         * insert a new default constructor into this symbol table
         */
        void insert_default_constructor(mir_default_constructor& def_cons);

        /**
         * insert_record_constructor
         * insert a new record constructor into this symbol table
         */
        void insert_record_constructor(mir_record_constructor& rec_cons);

        /**
         * get_default_constructor
         * given a constructor name and arity, return the matching default constructor if it exists
         */
        mir_default_constructor& get_default_constructor(const std::string& name, std::size_t arity);

        /**
         * get_record_constructor
         * given a constructor name and arity, return the matching record constructor if it exists
         */
        mir_record_constructor& get_record_constructor(const std::string& name, std::size_t arity);

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

    private:
        /*
         * map of all the constructors in the current scope
         */
        std::map<std::pair<std::string,std::size_t>, mir_default_constructor> m_def_constructors;
        std::map<std::pair<std::string,std::size_t>, mir_record_constructor> m_rec_constructors;
    };

    class mir_ctable {
    public:
        /**
         * the constructor expects nothing
         */
        mir_ctable();

        /**
         * insert_default_constructor
         * insert a new default constructor into this symbol table
         */
        void insert_default_constructor(const std::string& ns_name, mir_default_constructor& def_cons);

        /**
         * insert_record_constructor
         * insert a new record constructor into this symbol table
         */
        void insert_record_constructor(const std::string& ns_name, mir_record_constructor& rec_cons);

        /**
         * get_default_constructor
         * given a constructor name and arity, return the matching default constructor if it exists
         */
        mir_default_constructor& get_default_constructor(const std::string& ns_name, const std::string& cons_name, std::size_t arity);

        /**
         * get_record_constructor
         * given a constructor name and arity, return the matching record constructor if it exists
         */
        mir_record_constructor& get_record_constructor(const std::string& ns_name, const std::string& cons_name, std::size_t arity);

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

    private:
        /*
         * a map between namespaces and their respective symbol tables
         */
        std::unordered_map<std::string, mir_csymbols> m_symbols;
    };
}

#endif
