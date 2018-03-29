#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>
#include <map>

/* AST */
#include "program/ast/decl/type.hpp"
#include "program/ast/program.hpp"
/* Symtable */
#include "program/symtable/exceptions/symbol_not_found.hpp"
#include "program/symtable/scope.hpp"

/* Lexer */
#include "lexer/token.hpp"

/* Checker */
#include "checker/exceptions/invalid_constructor.hpp"
#include "checker/lax/decl/type/type_checker.hpp"
#include "checker/exceptions/invalid_type.hpp"


namespace avalon {
    /**
     * simple_check
     * given a type instance, the scope were it was found, the namespace of the object that holds it and a list of possible standins,
     * this function checks to see if that type instance exists in that scope in the given namespace or among the standins.
     */
    bool type_instance_checker::simple_check(type_instance& instance, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::vector<token>& standins) {
        std::vector<type_instance>& instance_params = instance.get_params();
        std::shared_ptr<type> instance_type = nullptr;
        bool is_parametrized = false;

        // we can only look for user defined type instances in the scope we have
        if(instance.get_category() == USER) {
            // we try to find if the type instance has an associated type that builds
            try {
                instance_type = l_scope -> get_type(ns_name, instance);
                // the type instance has a type that builds it,
                // we make sure the parameters it depends on are also valid
                for(auto& instance_param : instance_params) {
                    try {
                        bool instance_param_is_parametrized = type_instance_checker::simple_check(instance_param, l_scope, ns_name, standins);
                        if(instance_param_is_parametrized == true)
                            instance.is_parametrized(true);
                    } catch(invalid_type err) {
                        throw err;
                    }
                }
                // so all parameters to this type instance have valid type builders, we set the type builder on this
                instance.set_type(instance_type);
            }
            catch(symbol_not_found err) {
                // the type was not found in the scope given the namespace,
                // we try to find if it might be among the stand in types
                // but we only bother checking if it is among the standins if it admit no parameters
                if(instance_params.size() > 0) {
                    throw invalid_type(instance.get_token(), "This type instance has no type that builds it in the given scope and namespace.");
                }
                else {
                    if(std::find(standins.begin(), standins.end(), instance.get_token()) != standins.end()) {
                        is_parametrized = true;
                    }
                    else {
                        throw invalid_type(instance.get_token(), "This type instance has no type that builds it in the given scope and namespace.");
                    }
                }
            }
        }
        // if we have a built-in type, we just check if it depends on a parametrized type instance
        else {
            for(auto& instance_param : instance_params) {
                try {
                    bool instance_param_in_abstract = type_instance_checker::simple_check(instance_param, l_scope, ns_name, standins);
                    if(instance_param_in_abstract == true)
                        instance.is_parametrized(true);
                } catch(invalid_type err) {
                    throw err;
                }
            }
        }        

        return is_parametrized;
    }

    /**
     * complex_check
     * given a type instance, the scope were it was found, the namespace of the object that holds it and a list of possible standins,
     * these functions check to see if that type instance has a type builder that live in that scope in one the possible namespaces:
     * - the holder object namespace
     * - the global namespace
     * - the type instance attached namespace
     */
    bool type_instance_checker::complex_check(type_instance& instance, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::vector<token>& standins) {
        const std::string& l_ns_name = instance.get_namespace();
        bool is_parametrized = false;

        // if the type instance namespace is "*" then it means the type instance exists either:
        // - in the current namespace where the object that holds it is declared (this is given to us as a parameter)
        // - or in the global namespace
        if(l_ns_name == "*") {
            try {
                // first we search the type instance holder object namespace
                is_parametrized = type_instance_checker::simple_check(instance, l_scope, ns_name, standins);
            } catch(invalid_type err) {
                // we could not find the type instance in the holder namespace, we try the global namespace
                try {
                    is_parametrized = type_instance_checker::simple_check(instance, l_scope, l_ns_name, standins);
                } catch(invalid_type err) {
                    throw err;
                }
            }
        }
        else {
            try {
                // the type instance carries an identified namespace, we use that.
                is_parametrized = type_instance_checker::simple_check(instance, l_scope, l_ns_name, standins);
                // if the type instance is abstract, we raise an error because abstract type instances
                // are not allowed to be constrainted to a namespace
                if(is_parametrized) {
                    throw invalid_type(instance.get_token(), "An abstract type instance cannot be constrainted to a namespace.");
                }
            } catch(invalid_type err) {
                throw err;
            }
        }

        return is_parametrized;
    }

    bool type_instance_checker::complex_check(type_instance& instance, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::vector<token> standins;
        return complex_check(instance, l_scope, ns_name, standins);
    }


    /**
     * check
     * given a default or record constructor,
     * this function checks whether they are valid by making sure
     * that all type instances they depend on are built from valid types.
     */
    void constructor_checker::check(default_constructor& def_constructor, std::shared_ptr<type>& type_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::vector<type_instance>& cons_params = def_constructor.get_params();
        const std::vector<token>& type_params = type_decl -> get_params();

        // make sure the new constructor is no duplicate of an existing constructor
        if(l_scope -> default_constructor_exists(ns_name, def_constructor.get_name(), cons_params.size()))
            throw invalid_constructor("There already exists a default constructor in this program with the same name and arity. Maybe brought in by an imported type?");

        // a constructor is judged to be valid if all the types it depends on are valid
        for(auto& cons_param : cons_params) {
            const std::string& l_ns_name = cons_param.get_namespace();
            std::shared_ptr<type> instance_type = nullptr;

            // validate the constructor parameter
            try {
                bool is_parametrized = type_instance_checker::complex_check(cons_param, l_scope, ns_name, type_params);
                if(is_parametrized == false)
                    instance_type = cons_param.get_type();
            } catch(invalid_type err) {
                throw invalid_constructor("This constructor depends on a type that does not exist either in the attached namespace or the local namespace or the global namespace.");
            }

            // we have the type instance type builder, we check if it is the same as that which this constructor builds
            // if they are the same, then we can judge this constructor to be temporarily valid
            if((ns_name == l_ns_name) & (instance_type != nullptr && * instance_type == * type_decl)) {
                continue;
            }
            else {
                // if the type instance type builder is invalid, so it is the constructor
                if(instance_type != nullptr && instance_type -> is_valid(INVALID))
                    throw invalid_constructor("Default constructor <" + def_constructor.get_name() + "> failed type checking because the type <" + instance_type -> get_name() + "> is not valid.");
            }
        }
    }

    void constructor_checker::check(record_constructor& rec_constructor, std::shared_ptr<type>& type_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::map<token, type_instance>& cons_params = rec_constructor.get_params();
        const std::vector<token>& type_params = type_decl -> get_params();

        // make sure the new constructor is no duplicate of an existing constructor
        if(l_scope -> record_constructor_exists(ns_name, rec_constructor.get_name(), cons_params.size()))
            throw invalid_constructor("There already exists a record constructor in this program with the same name and arity. Maybe brought in by an imported type?");

        // a constructor is judged to be valid if all the types it depends on are valid        
        for(auto& cons_param : cons_params) {
            const std::string& l_ns_name = cons_param.second.get_namespace();
            std::shared_ptr<type> instance_type = nullptr;

            // validate the constructor parameter
            try {
                bool is_parametrized = type_instance_checker::complex_check(cons_param.second, l_scope, ns_name, type_params);
                if(is_parametrized == false)
                    instance_type = cons_param.second.get_type();
            } catch(invalid_type err) {
                throw invalid_constructor("This constructor depends on a type that does not exist either in the attached namespace or the local namespace or the global namespace.");
            }

            // we have the type instance type builder, we check if it is the same as that which this constructor builds
            // if they are the same, then we can judge this constructor to be temporarily valid
            if((ns_name == l_ns_name) & (instance_type != nullptr && * instance_type == * type_decl)) {
                continue;
            }
            else {
                // if the type instance type builder is invalid, so it is the constructor
                if(instance_type != nullptr && instance_type -> is_valid(INVALID))
                    throw invalid_constructor("Record constructor <" + rec_constructor.get_name() + "> failed type checking because the type <" + instance_type -> get_name() + "> is not valid.");
            }
        }
    }


/**
 * the default constructor expects nothing
 */
type_checker::type_checker() {
}
    
    /**
     * check
     * - makes sure all type parameters are valid
     * - make sure all constructors are valid
     */
    void type_checker::check(std::shared_ptr<type>& type_decl, program& prog, const std::string& ns_name) {
        std::shared_ptr<scope>& l_scope = prog.get_scope();

        // check type parameters making sure no parameter appears twice
        // and no parameter correponds to any type in all the available namespaces
        const std::vector<token>& type_params = type_decl -> get_params();
        for(const auto& type_param : type_params) {
            // make sure no parameter is a duplicate of an existing parameter
            if(std::count(type_params.begin(), type_params.end(), type_param) > 1) {
                throw invalid_type(type_param, "The type parameter <" + type_param.get_lexeme() + "> cannot be used more than once inside type parameters.");
            }

            // make sure no parameter correponds to any concrete type available in the scope (all namespaces)
            if(l_scope -> type_exists(type_param.get_lexeme(), 0)) {
                throw invalid_type(type_param, "The type parameter <" + type_param.get_lexeme() + "> has the same name as a concrete type available in the current scope.");
            }
        }
        
        // make sure there are no other types with the same name and arity        
        if(l_scope -> type_exists(ns_name, type_decl)) {
            throw invalid_type(type_decl -> get_token(), "Another type with the same name and arity already exists within this program.");
        }

        // check default constructors
        std::vector<default_constructor> def_constructors = type_decl -> get_default_constructors();
        for(auto& def_constructor : def_constructors) {
            try {
                constructor_checker::check(def_constructor, type_decl, l_scope, ns_name);
            } catch(invalid_constructor err) {
                throw invalid_type(def_constructor.get_token(), err.what());
            }
        }

        // check record constructors
        std::vector<record_constructor> rec_constructors = type_decl -> get_record_constructors();
        for(auto& rec_constructor : rec_constructors) {
            try {
                constructor_checker::check(rec_constructor, type_decl, l_scope, ns_name);
            } catch(invalid_constructor err) {
                throw invalid_type(rec_constructor.get_token(), err.what());
            }
        }

        // all is well, the type declaration is valid
        type_decl -> set_is_valid(VALID);
    }
}
