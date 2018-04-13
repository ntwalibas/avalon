#ifndef AVALON_MIR_SYMTABLE_SCOPE_HPP_
#define AVALON_MIR_SYMTABLE_SCOPE_HPP_

#include <memory>

#include "representer/mir/symtable/mir_dtable.hpp"
#include "representer/mir/symtable/mir_ctable.hpp"


namespace avalon {
    /* forward declarations */
    class mir_type;
    class mir_function;
    class mir_variable;

    class mir_scope {
    public:
        /**
         * the constructor expects nothing
         */
        mir_scope();

        /**
         * add_namespace
         */
        void add_namespace(const std::string& namespace_name);

        /**
         * get_dtable
         * returns the symbol table of declarations found in this scope
         */
        mir_dtable& get_dtable();

        /**
         * get_ctable
         * returns the symbol table of type constructors found in this scope
         */
        mir_ctable& get_ctable();

        /*** TYPE DECLARATIONS ***/

        /**
         * add_type
         * add a new type declaration into this scope
         */
        void add_type(const std::string& ns_name, std::shared_ptr<mir_type>& type_decl);

        /**
         * get_type
         * given a type name, return the type that match them
         */
        std::shared_ptr<mir_type>& get_type(const std::string& ns_name, const std::string& type_name);

        /**
         * type_exists
         * given a type declaration, find if an existing type declaration matches it
         */
        bool type_exists(const std::string& ns_name, std::shared_ptr<mir_type>& type_decl);

        /**
         * type_exists
         * given a type name, find if an existing type declaration match the same
         */
        bool type_exists(const std::string& ns_name, const std::string& type_name);

        /**
         * add_default_constructor
         * add a new default constructor into this symbol table
         */
        void add_default_constructor(const std::string& ns_name, mir_default_constructor& def_cons);

        /**
         * add_record_constructor
         * add a new record constructor into this symbol table
         */
        void add_record_constructor(const std::string& ns_name, mir_record_constructor& rec_cons);

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

        
        /*** FUNCTION DECLARATIONS ***/

        /**
         * add_function
         * given a namespace and a function declaration, add the declaration to the scope
         */
        void add_function(const std::string& ns_name, std::shared_ptr<mir_function>& function_decl);

        /**
         * get_function
         * given a namespace and a function name, return the matching function in that namespace with the same name
         */
        std::shared_ptr<mir_function>& get_function(const std::string& ns_name, const std::string& function_name);

        /**
         * function_exists
         * given a namespace and a function declaration, find if that function declaration already exists in that namespace
         */
        bool function_exists(const std::string& ns_name, std::shared_ptr<mir_function>& function_decl);

        /**
         * function_exists
         * given a namespace and a function name, find if the namespace contains a function with that name
         */
        bool function_exists(const std::string& ns_name, const std::string& function_name);


        /*** VARIABLE DECLARATIONS ***/

        /**
         * add_variable
         * given a namespace and a variable declaration, add the later to the former
         */
        void add_variable(const std::string& ns_name, std::shared_ptr<mir_variable>& variable_decl);

        /**
         * get_variable
         * given a namespace and a variable name, return the corresponding variable declaration if it exists in that namespace
         */
        std::shared_ptr<mir_variable>& get_variable(const std::string& ns_name, const std::string& variable_name);

        /**
         * variable_exists
         * given a namespace and a variable declaration, find if it already exists in this namespace
         */
        bool variable_exists(const std::string& ns_name, std::shared_ptr<mir_variable>& variable_decl);

        /**
         * variable_exists
         * given a namespace and a variable name, find if there is a variabl declaration for it in this namespace
         */
        bool variable_exists(const std::string& ns_name, const std::string& variable_name);

        /**
         * set_parent
         * sets the parent of this scope
         */
        void set_parent(std::shared_ptr<mir_scope>& parent);

        /**
         * get_parent
         * returns the parent of this scope
         */
        std::shared_ptr<mir_scope>& get_parent();

        /**
         * has_parent
         * returns true if this scope has a parent scope
         */
        bool has_parent();

    private:
        /*
         * a map of namespaces available to this scope
         */
        std::map<std::string, std::string> m_namespaces;

        /*
         * declarations declared within this scope.
         */
        mir_dtable m_dtable;

        /*
         * constructors declared within this scope
         */
        mir_ctable m_ctable;

        /*
         * parent scope
         */
        std::shared_ptr<mir_scope> m_parent;
    };
}

#endif
