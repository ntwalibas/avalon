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
#include "checker/weak/decl/type/type_checker.hpp"
#include "checker/exceptions/invalid_type.hpp"


namespace avalon {
    /**
     * simple_check
     * given a type instance, the scope were it was found, the namespace of the object that holds it and a list of possible standins,
     * this function checks to see if that type instance exists in that scope in the given namespace or among the standins.
     */
    std::pair<bool,bool> type_instance_checker::simple_check(type_instance& instance, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::vector<token>& standins) {
        std::vector<type_instance>& instance_params = instance.get_params();
        std::shared_ptr<type> instance_type = nullptr;
        std::pair<bool,bool> ret(false, false);

        // we can only look for user defined type instances in the scope we have
        if(instance.get_category() == USER) {
            // we try to find if the type instance has an associated type that builds
            try {
                instance_type = l_scope -> get_type(ns_name, instance);
                // the type instance has a type that builds it,
                // we make sure the parameters it depends on are also valid
                for(auto& instance_param : instance_params) {
                    try {
                        std::pair<bool,bool> res = type_instance_checker::simple_check(instance_param, l_scope, ns_name, standins);
                        if(res.first == true || res.second == true) {
                            instance.is_parametrized(true);
                            ret.second = true;
                        }
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
                        ret.first = true;
                    }
                    else {
                        throw invalid_type(instance.get_token(), "This type instance has no type that builds it in the given scope and namespace.");
                    }
                }
            }
        }
        // if we have a built-in type, we check the type instances it depends on if any and set the type builder on it
        else {
            // we check dependent type instances, if any
            for(auto& instance_param : instance_params) {
                try {
                    std::pair<bool,bool> res = type_instance_checker::simple_check(instance_param, l_scope, ns_name, standins);
                    if(res.first == true || res.second == true) {
                        instance.is_parametrized(true);
                        ret.second = true;
                    }
                } catch(invalid_type err) {
                    throw err;
                }
            }

            // set the type builder
            token tok = instance.get_token();
            std::shared_ptr<type> ty = std::make_shared<type>(tok, VALID);
            instance.set_type(ty);
            if(instance.get_category() == TUPLE) {
                instance.set_category(TUPLE);
            }
            else if(instance.get_category() == LIST) {
                instance.set_category(LIST);
            }
            else if(instance.get_category() == MAP) {
                instance.set_category(MAP);
            }
        }        

        return ret;
    }

    /**
     * complex_check
     * given a type instance, the scope were it was found, the namespace of the object that holds it and a list of possible standins,
     * these functions check to see if that type instance has a type builder that live in that scope in one the possible namespaces:
     * - the holder object namespace
     * - the global namespace
     * - the type instance attached namespace
     */
    std::pair<bool,bool> type_instance_checker::complex_check(type_instance& instance, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::vector<token>& standins) {
        const std::string& l_ns_name = instance.get_namespace();
       std::pair<bool,bool> res;

        // if the type instance namespace is "*" then it means the type instance exists either:
        // - in the current namespace where the object that holds it is declared (this is given to us as a parameter)
        // - or in the global namespace
        if(l_ns_name == "*") {
            try {
                // first we search the type instance holder object namespace
                res = type_instance_checker::simple_check(instance, l_scope, ns_name, standins);
            } catch(invalid_type err) {
                // we could not find the type instance in the holder namespace, we try the global namespace
                try {
                    res = type_instance_checker::simple_check(instance, l_scope, l_ns_name, standins);
                } catch(invalid_type err) {
                    throw err;
                }
            }
        }
        else {
            try {
                // the type instance carries an identified namespace, we use that.
                res = type_instance_checker::simple_check(instance, l_scope, l_ns_name, standins);
                // if the type instance is abstract, we raise an error because abstract type instances
                // are not allowed to be constrainted to a namespace
                if(res.first) {
                    throw invalid_type(instance.get_token(), "An abstract type instance cannot be constrainted to a namespace.");
                }
            } catch(invalid_type err) {
                throw err;
            }
        }

        return res;
    }

    std::pair<bool,bool> type_instance_checker::complex_check(type_instance& instance, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
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
                std::pair<bool,bool> res = type_instance_checker::complex_check(cons_param, l_scope, ns_name, type_params);
                if(res.first == false)
                    instance_type = cons_param.get_type();
            } catch(invalid_type err) {
                // we check if the type instance in question is not the current type this constructor builds
                if(cons_param.is_builtby(type_decl)) {
                    instance_type = type_decl;
                    cons_param.set_type(type_decl);
                }
                else {
                    throw invalid_constructor("This constructor depends on a type instance that does not exist either in the attached namespace or the local namespace or the global namespace.");
                }
            }

            // if the type that builds the parameters this constructor depends on is private and the type this constructor is public
            // we issue an error as this constructor cannot be used
            if(instance_type != nullptr && (instance_type -> is_public() == false && type_decl -> is_public() == true)) {
                throw invalid_constructor("This constructor depends on a type instance that's private while the type it builds is public. Both must be public or both must be private.");
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
                std::pair<bool,bool> res = type_instance_checker::complex_check(cons_param.second, l_scope, ns_name, type_params);
                if(res.first == false)
                    instance_type = cons_param.second.get_type();
            } catch(invalid_type err) {
                // we check if the type instance in question is not the current type this constructor builds
                if(cons_param.second.is_builtby(type_decl)) {
                    instance_type = type_decl;
                    cons_param.second.set_type(type_decl);
                }
                else {
                    throw invalid_constructor("This constructor depends on a type instance that does not exist either in the attached namespace or the local namespace or the global namespace.");
                }
            }

            // if the type that builds the parameters this constructor depends on is private and the type this constructor is public
            // we issue an error as this constructor cannot be used
            if(instance_type != nullptr && (instance_type -> is_public() == false && type_decl -> is_public() == true)) {
                throw invalid_constructor("This constructor depends on a type instance that's private while the type it builds is public. Both must be public or both must be private.");
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

    void constructor_checker::check(list_constructor& lst_constructor, std::shared_ptr<type>& type_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        type_instance& cons_param = lst_constructor.get_param();
        const std::vector<token>& type_params = type_decl -> get_params();
        const std::string& l_ns_name = cons_param.get_namespace();
        std::shared_ptr<type> instance_type = nullptr;

        // validate the constructor parameter
        try {
            std::pair<bool,bool> res = type_instance_checker::complex_check(cons_param, l_scope, ns_name, type_params);
            if(res.first == false)
                instance_type = cons_param.get_type();
        } catch(invalid_type err) {
            // we check if the type instance in question is not the current type this constructor builds
            if(cons_param.is_builtby(type_decl)) {
                instance_type = type_decl;
                cons_param.set_type(type_decl);
            }
            else {
                throw invalid_constructor("This constructor depends on a type instance that does not exist either in the attached namespace or the local namespace or the global namespace.");
            }
        }

        // if the type that builds the parameters this constructor depends on is private and the type this constructor is public
        // we issue an error as this constructor cannot be used
        if(instance_type != nullptr && (instance_type -> is_public() == false && type_decl -> is_public() == true)) {
            throw invalid_constructor("This constructor depends on a type instance that's private while the type it builds is public. Both must be public or both must be private.");
        }

        // we have the type instance type builder, we check if it is the same as that which this constructor builds
        // if they are the same, then we can judge this constructor to be temporarily valid
        if((ns_name == l_ns_name) & (instance_type != nullptr && * instance_type == * type_decl)) {
            ;
        }
        else {
            // if the type instance type builder is invalid, so it is the constructor
            if(instance_type != nullptr && instance_type -> is_valid(INVALID))
                throw invalid_constructor("List constructor <" + lst_constructor.get_name() + "> failed type checking because the type <" + instance_type -> get_name() + "> is not valid.");
        }
    }

    void constructor_checker::check(map_constructor& mp_constructor, std::shared_ptr<type>& type_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        const std::vector<token>& type_params = type_decl -> get_params();
        
        /* we do checking for the key parameter */
        type_instance& cons_param_key = mp_constructor.get_param_key();
        const std::string& key_ns_name = cons_param_key.get_namespace();
        std::shared_ptr<type> key_instance_type = nullptr;
        // validate the constructor parameter key
        try {
            std::pair<bool,bool> res = type_instance_checker::complex_check(cons_param_key, l_scope, ns_name, type_params);
            if(res.first == false)
                key_instance_type = cons_param_key.get_type();
        } catch(invalid_type err) {
            // we check if the type instance in question is not the current type this constructor builds
            if(cons_param_key.is_builtby(type_decl)) {
                key_instance_type = type_decl;
                cons_param_key.set_type(type_decl);
            }
            else {
                throw invalid_constructor("This constructor key parameter depends on a type instance that does not exist either in the attached namespace or the local namespace or the global namespace.");
            }
        }
        // if the type that builds the parameters this constructor depends on is private and the type this constructor is public
        // we issue an error as this constructor cannot be used
        if(key_instance_type != nullptr && (key_instance_type -> is_public() == false && type_decl -> is_public() == true)) {
            throw invalid_constructor("This constructor key parameter depends on a type instance that's private while the type it builds is public. Both must be public or both must be private.");
        }
        // we have the type instance type builder, we check if it is the same as that which this constructor builds
        // if they are the same, then we can judge this constructor to be temporarily valid
        if((ns_name == key_ns_name) & (key_instance_type != nullptr && * key_instance_type == * type_decl)) {
            ;
        }
        else {
            // if the type instance type builder is invalid, so it is the constructor
            if(key_instance_type != nullptr && key_instance_type -> is_valid(INVALID))
                throw invalid_constructor("Map constructor <" + mp_constructor.get_name() + "> failed type checking because the type <" + key_instance_type -> get_name() + "> is not valid.");
        }
        

        /* we do checking for the value parameter */
        type_instance& cons_param_value = mp_constructor.get_param_value();
        const std::string& value_ns_name = cons_param_key.get_namespace();
        std::shared_ptr<type> value_instance_type = nullptr;
        // validate the constructor parameter value
        try {
            std::pair<bool,bool> res = type_instance_checker::complex_check(cons_param_value, l_scope, ns_name, type_params);
            if(res.first == false)
                value_instance_type = cons_param_key.get_type();
        } catch(invalid_type err) {
            // we check if the type instance in question is not the current type this constructor builds
            if(cons_param_value.is_builtby(type_decl)) {
                value_instance_type = type_decl;
                cons_param_value.set_type(type_decl);
            }
            else {
                throw invalid_constructor("This constructor value parameter depends on a type instance that does not exist either in the attached namespace or the local namespace or the global namespace.");
            }
        }
        // if the type that builds the parameters this constructor depends on is private and the type this constructor is public
        // we issue an error as this constructor cannot be used
        if(value_instance_type != nullptr && (value_instance_type -> is_public() == false && type_decl -> is_public() == true)) {
            throw invalid_constructor("This constructor value parameter depends on a type instance that's private while the type it builds is public. Both must be public or both must be private.");
        }
        // we have the type instance type builder, we check if it is the same as that which this constructor builds
        // if they are the same, then we can judge this constructor to be temporarily valid
        if((ns_name == value_ns_name) & (value_instance_type != nullptr && * value_instance_type == * type_decl)) {
            ;
        }
        else {
            // if the type instance type builder is invalid, so it is the constructor
            if(value_instance_type != nullptr && value_instance_type -> is_valid(INVALID))
                throw invalid_constructor("Map constructor <" + mp_constructor.get_name() + "> failed type checking because the type <" + value_instance_type -> get_name() + "> is not valid.");
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

        // check list constructors
        std::vector<list_constructor> list_constructors = type_decl -> get_list_constructors();
        for(auto& lst_constructor : list_constructors) {
            try {
                constructor_checker::check(lst_constructor, type_decl, l_scope, ns_name);
            } catch(invalid_constructor err) {
                throw invalid_type(lst_constructor.get_token(), err.what());
            }
        }

        // check map constructors
        std::vector<map_constructor> map_constructors = type_decl -> get_map_constructors();
        for(auto& mp_constructor : map_constructors) {
            try {
                constructor_checker::check(mp_constructor, type_decl, l_scope, ns_name);
            } catch(invalid_constructor err) {
                throw invalid_type(mp_constructor.get_token(), err.what());
            }
        }

        // all is well, the type declaration is valid
        type_decl -> set_is_valid(VALID);
    }
}
