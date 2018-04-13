#ifndef AVALON_MIR_SYMTABLE_DTABLE_HPP_
#define AVALON_MIR_SYMTABLE_DTABLE_HPP_

#include <unordered_map>
#include <memory>
#include <string>


namespace avalon {
    /* forward declarations */
    class mir_type;
    class mir_function;
    class mir_variable;

    class mir_dsymbols {
    public:
        /**
         * the constructor expects nothing
         */
        mir_dsymbols();

        /*** TYPE DECLARATIONS ***/

        /**
         * insert_type
         * given a type declaration, this function adds it into the symbol table
         */
        void insert_type(std::shared_ptr<mir_type>& type_decl);

        /**
         * get_type
         * given a type name, we find the corresponding type declaration
         */
        std::shared_ptr<mir_type>& get_type(const std::string& name);

        /**
         * type_exists
         * given a type declaration, find if an existing type declaration matches it
         */
        bool type_exists(std::shared_ptr<mir_type>& type_decl);

        /**
         * type_exists
         * given a type name, find if an existing type declaration match the same
         */
        bool type_exists(const std::string& name);

        
        /*** FUNCTION DECLARATIONS ***/

        /**
         * insert_function
         * given a function, insert it into the symbol table
         */
        void insert_function(std::shared_ptr<mir_function>& function_decl);

        /**
         * get_function
         * given a function name, return the corresponding function
         */
        std::shared_ptr<mir_function>& get_function(const std::string& name);

        /**
         * function_exists
         * given a function declaration, find if it already exists in the symbol table
         */
        bool function_exists(std::shared_ptr<mir_function>& function_decl);

        /**
         * function_exists
         * given a name, find if there is any function by that in name in the symbol table
         */
        bool function_exists(const std::string& name);


        /*** VARIABLE DECLARATIONS ***/

        /**
         * insert_variable
         * add a new variable declaration to this table
         */
        void insert_variable(std::shared_ptr<mir_variable>& variable_decl);

        /**
         * get_variable
         * given a variable name, return the corresponding variable declaration
         */
        std::shared_ptr<mir_variable>& get_variable(const std::string& name);

        /**
         * variable_exists
         * given a variable declaration, find if it already exists in this table
         */
        bool variable_exists(std::shared_ptr<mir_variable>& variable_decl);

        /**
         * variable_exists
         * given a variable name, find if there is a variabl declaration for it in this table
         */
        bool variable_exists(const std::string& name);

    private:
        /*
         * map of all the type declarations in the current scope
         */
        std::unordered_map<std::string, std::shared_ptr<mir_type> > m_types;

        /*
         * a map of all functions declared in the program
         */        
        std::unordered_map<std::string, std::shared_ptr<mir_function> > m_functions;

        /*
         * map of variable names and variable declarations stored in this table
         */
        std::unordered_map<std::string, std::shared_ptr<mir_variable> > m_variables;
    };


    class mir_dtable {
    public:
        /**
         * the constructor expects nothing
         */
        mir_dtable();

        /**
         * insert_type
         * given a namespace and a type declaration, this function adds it into the namespace symbol table
         */
        void insert_type(const std::string& ns_name, std::shared_ptr<mir_type>& type_decl);

        /**
         * get_type
         * given a namespace and a type name, return the type that matches both from the namespace
         */
        std::shared_ptr<mir_type>& get_type(const std::string& ns_name, const std::string& type_name);

        /**
         * type_exists
         * given a namespace and a type declaration, find if an existing type declaration matches it within the namespace
         */
        bool type_exists(const std::string& ns_name, std::shared_ptr<mir_type>& type_decl);

        /**
         * type_exists
         * given a namespace and a type name, find if an existing type declaration matches them within the namespace
         */
        bool type_exists(const std::string& ns_name, const std::string& type_name);

        
        /*** FUNCTION DECLARATIONS ***/

        /**
         * insert_function
         * given a namespace and a function declaration, insert the last in the symbol table
         */
        void insert_function(const std::string& ns_name, std::shared_ptr<mir_function>& function_decl);

        /**
         * get_function
         * given a namespace and a function name, return all functions in that namespace with the same name and arity
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
         * insert_variable
         * given a namespace and a variable declaration, add the later to the former
         */
        void insert_variable(const std::string& ns_name, std::shared_ptr<mir_variable>& variable_decl);

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
    
    private:
        /*
         * a map between namespaces and their respective symbol tables
         */
        std::unordered_map<std::string, mir_dsymbols> m_symbols;
    };
}

#endif
