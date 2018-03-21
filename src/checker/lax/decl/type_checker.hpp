#ifndef AVALON_CHECKER_DECL_TYPE_CHECKER_HPP_
#define AVALON_CHECKER_DECL_TYPE_CHECKER_HPP_

#include <memory>
#include <string>

#include "program/symtable/scope.hpp"
#include "program/ast/decl/type.hpp"
#include "program/ast/program.hpp"


namespace avalon {
    class type_instance_checker {
    public:
        /**
         * simple_check
         * given a type instance, the scope were it was found, the namespace of the object that holds it and a list of possible standins,
         * this function checks to see if that type instance exists in that scope in the given namespace or among the standins.
         */
        static bool simple_check(type_instance& instance, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::vector<token>& standins);

        /**
         * complex_check
         * given a type instance, the scope were it was found, the namespace of the object that holds it and a list of possible standins,
         * these functions check to see if that type instance has a type builder that live in that scope in one the possible namespaces:
         * - the holder object namespace
         * - the global namespace
         * - the type instance attached namespace
         */
        static bool complex_check(type_instance& instance, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::vector<token>& standins);
        static bool complex_check(type_instance& instance, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
    };


    class constructor_checker {
    public:
        /**
         * check
         * given a default or record constructor,
         * this function checks whether they are valid by making sure
         * that all type instances they depend on are built from valid types.
         */
        static void check(default_constructor& def_constructor, std::shared_ptr<type>& type_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
        static void check(record_constructor& rec_constructor, std::shared_ptr<type>& type_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
    
    private:
        /**
         * a default constructor expects nothing
         */
        constructor_checker();        
    };


    class type_checker {
    public:
        /**
         * the default constructor expects nothing
         */
        type_checker();

        /**
         * check
         * - makes sure all type parameters are valid
         * - make sure all constructors are valid
         */
        void check(std::shared_ptr<type>& type_decl, program& prog, const std::string& ns_name);
    };
}

#endif
