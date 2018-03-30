#include <map>
#include <vector>
#include <cstddef>
#include <utility>
#include <iostream>
#include <stdexcept>

#include "lexer/token.hpp"
#include "program/ast/decl/type.hpp"
#include "program/ast/exceptions/type_error.hpp"
#include "program/ast/exceptions/constructor_error.hpp"


namespace avalon {

/*
 * type
 */
/**
 * the constructor expects:
 * - the token with type information
 * - the validation state
 */
type::type(token& tok, validation_state is_valid) : m_name(tok.get_lexeme()), m_tok(tok), m_is_valid(is_valid), m_is_public(true) {
}
    /**
     * set_name
     * updates the name of this type
     */
    void type::set_name(const std::string& name) {
        m_name = name;
    }

    /**
     * get_name
     * returns the name of the type
     */
    const std::string& type::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& type::get_token() const {
        return m_tok;
    }

    /**
     * add_constructor
     * add a default constructor to this type
     */
    void type::add_constructor(default_constructor& def_constructor) {        
        std::pair<std::string, std::size_t> cons_key(def_constructor.get_name(), def_constructor.get_params().size());
        if(m_def_constructors.count(cons_key) > 0)
            throw type_error("There already exists a default constructor with the given name and arity.");
        else
            m_def_constructors.emplace(cons_key, def_constructor);
    }

    /**
     * add_constructor
     * add a record constructor to this type
     */
    void type::add_constructor(record_constructor& rec_constructor) {
        std::pair<std::string, std::size_t> cons_key(rec_constructor.get_name(), rec_constructor.get_params().size());
        if(m_rec_constructors.count(cons_key) > 0)
            throw type_error("There already exists a record constructor with the given name and arity.");
        else
            m_rec_constructors.emplace(cons_key, rec_constructor);
    }

    /**
     * add_constructor
     * add a list constructor to this type
     */
    void type::add_constructor(list_constructor& list_constructor) {
        const std::string& cons_key = list_constructor.get_name();
        if(m_list_constructors.count(cons_key) > 0)
            throw type_error("There already exists a list constructor with the given name.");
        else
            m_list_constructors.emplace(cons_key, list_constructor);
    }

    /**
     * add_constructor
     * add a map constructor to this type
     */
    void type::add_constructor(map_constructor& map_constructor) {
        const std::string& cons_key = map_constructor.get_name();
        if(m_map_constructors.count(cons_key) > 0)
            throw type_error("There already exists a map constructor with the given name.");
        else
            m_map_constructors.emplace(cons_key, map_constructor);
    }

    /**
     * get_default_constructors
     * returns a vector of all default constructors that build this type
     */
    std::vector<default_constructor> type::get_default_constructors() {
        std::vector<default_constructor> def_constructors;
        for(auto& def_constructor : m_def_constructors)
            def_constructors.push_back(def_constructor.second);
        return def_constructors;
    }

    /**
     * get_record_constructors
     * returns a vector of all record constructors that build this type
     */
    std::vector<record_constructor> type::get_record_constructors() {
        std::vector<record_constructor> rec_constructors;
        for(auto& rec_constructor : m_rec_constructors)
            rec_constructors.push_back(rec_constructor.second);
        return rec_constructors;
    }

    /**
     * get_list_constructors
     * returns a vector of all list constructors that build this type
     */
    std::vector<list_constructor> type::get_list_constructors() {
        std::vector<list_constructor> list_constructors;
        for(auto& lst_constructor : m_list_constructors)
            list_constructors.push_back(lst_constructor.second);
        return list_constructors;
    }

    /**
     * get_map_constructors
     * returns a vector of all map constructors that build this type
     */
    std::vector<map_constructor> type::get_map_constructors() {
        std::vector<map_constructor> map_constructors;
        for(auto& mp_constructor : m_map_constructors)
            map_constructors.push_back(mp_constructor.second);
        return map_constructors;
    }

    /**
     * get_default_constructors
     * returns a vector of default constructors matching the name given
     */
    std::vector<default_constructor> type::get_default_constructors(const std::string& name) {
        std::vector<default_constructor> def_constructors;
        for(auto& def_constructor : m_def_constructors)
            // they key of the map that holds constructors is a pair itself which first element is the constructor name
            if(def_constructor.first.first == name)
                def_constructors.push_back(def_constructor.second);
        return def_constructors;
    }

    /**
     * get_record_constructors
     * returns a vector of record constructors matching the name given
     */
    std::vector<record_constructor> type::get_record_constructors(const std::string& name) {
        std::vector<record_constructor> rec_constructors;
        for(auto& rec_constructor : m_rec_constructors)
            // they key of the map that holds constructors is a pair itself which first element is the constructor name
            if(rec_constructor.first.first == name)
                rec_constructors.push_back(rec_constructor.second);
        return rec_constructors;
    }

    /**
     * get_default_constructor
     * given a default constructor name and arity, find it there is one that builds this type
     */
    default_constructor& type::get_default_constructor(const std::string& name, std::size_t arity) {
        std::pair<std::string, std::size_t> cons_key(name, arity);
        try {
            return m_def_constructors.at(cons_key);
        } catch(std::out_of_range err) {
            throw type_error("This type has no default constructor with the given name and arity.");
        }
    }

    /**
     * get_record_constructor
     * given a record constructor name and arity, find it there is one that builds this type
     */
    record_constructor& type::get_record_constructor(const std::string& name, std::size_t arity) {
        std::pair<std::string, std::size_t> cons_key(name, arity);
        try {
            return m_rec_constructors.at(cons_key);
        } catch(std::out_of_range err) {
            throw type_error("This type has no record constructor with the given name and arity.");
        }
    }

    /**
     * get_list_constructor
     * given a list constructor name, find it there is one that builds this type
     */
    list_constructor& type::get_list_constructor(const std::string& name) {
        try {
            return m_list_constructors.at(name);
        } catch(std::out_of_range err) {
            throw type_error("This type has no list constructor with the given name.");
        }
    }

    /**
     * get_map_constructor
     * given a map constructor name, find it there is one that builds this type
     */
    map_constructor& type::get_map_constructor(const std::string& name) {
        try {
            return m_map_constructors.at(name);
        } catch(std::out_of_range err) {
            throw type_error("This type has no map constructor with the given name.");
        }
    }

    /**
     * add_type_param
     * if this is a parametric type, this function allows us to add type parameters
     */
    void type::add_param(token& type_param) {
        m_params.push_back(type_param);
    }

    /**
     * get_type_params
     * returns a vector with all type parameters if any.
     */
    const std::vector<token>& type::get_params() const {
        return m_params;
    }

    /**
     * set_is_valid
     * updates the type current validation state.
     */
    void type::set_is_valid(validation_state state) {
        m_is_valid = state;
    }

    /**
     * is_valid
     * given a validation state,
     * this function returns true if the type has the given validation state.
     */
    bool type::is_valid(validation_state state) {
        return m_is_valid == state;
    }

    /**
     * mangle_type
     * give a type name and the number of type parameters accepts,
     * we create a string that represents the true type name as
     * the new name takes into account the parameters the type accepts.
     */
    std::string mangle_type(const std::string& name, int params_count) {
        std::string mangled_name = std::string("__");
        mangled_name += name;    

        for(auto i = 0; i < params_count; i++) {
            if(i == 0)
                mangled_name += std::string("_");

            mangled_name += "*";

            if(i == params_count - 1)
                mangled_name += std::string("_");
        }

        mangled_name += std::string("__");
        return mangled_name;
    }

/**
 * the default constructor expects nothing
 */
type_instance::type_instance() : m_name(star_tok.get_lexeme()), m_tok(star_tok), m_old_tok(star_tok), m_category(USER), m_namespace("*"), m_type(nullptr), m_return_type_instance(nullptr), m_is_parametrized(false) {    
}

/*
 * type instance
 */
type_instance::type_instance(token& tok, const std::string& namespace_name) : m_name(tok.get_lexeme()), m_tok(tok), m_old_tok(tok), m_category(USER), m_namespace(namespace_name), m_type(nullptr), m_return_type_instance(nullptr), m_is_parametrized(false) {
}

/**
 * this constructor expects the token with source code information, the type that buils this instance and the namespace where to find that type
 */
type_instance::type_instance(token& tok, std::shared_ptr<type>& ty, const std::string& namespace_name) : m_name(tok.get_lexeme()), m_tok(tok), m_old_tok(tok), m_category(USER), m_namespace(namespace_name), m_type(ty), m_return_type_instance(nullptr), m_is_parametrized(false)  {
}
    
    /**
     * set_name
     * updates the name of this type
     */
    void type_instance::set_name(const std::string& name) {
        m_name = name;
    }

    /**
     * get_name
     * returns the name of the type
     */
    const std::string& type_instance::get_name() const {
        return m_name;
    }

    /**
     * set_token
     * changes the token and the name of this type instance
     */
    void type_instance::set_token(token tok) {
        m_tok = tok;
        m_name = tok.get_lexeme();
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& type_instance::get_token() const {
        return m_tok;
    }

    /**
     * set_old_token
     * When an instance is updated from an abstract one to a concrete one
     * this function is called to keep a record of the old token
     */
    void type_instance::set_old_token(token tok) {
        m_old_tok = tok;
    }

    /**
     * get_old_token
     * returns the old token
     */
    const token& type_instance::get_old_token() const {
        return m_old_tok;
    }

    /**
     * set_category
     * set the category to which this type instance belongs
     */
    void type_instance::set_category(type_instance_category category) {
        m_category = category;
    }

    /**
     * get_category
     * get the category to which this type instance belongs
     */
    const type_instance_category& type_instance::get_category() const {
        return m_category;
    }

    /**
     * get_namespace
     * returns the namespace where to find the type declaration that builds this type instance
     */
    const std::string& type_instance::get_namespace() const {
        return m_namespace;
    }

    /**
     * set_type
     * sets the type that constructs this type instance
     */
    void type_instance::set_type(std::shared_ptr<type>& ty) {
        m_type = ty;
    }

    /**
     * get_type
     * returns the type that corresponds to this type instance
     */
    std::shared_ptr<type>& type_instance::get_type() {
        return m_type;
    }

    /**
     * is_builtby
     * given a type declaration, return true if that type builds this type instance
     */
    bool type_instance::is_builtby(std::shared_ptr<type>& ty) {
        if(ty -> get_name() == m_name && ty -> get_params().size() == m_params.size())
            return true;
        else
            return false;
    }

    /**
     * add_param
     * a type instance depends on other type instances if
     * the type constructor that builds it is parametric.
     */
    void type_instance::add_param(type_instance& param) {
        m_params.push_back(param);
    }

    /**
     * get_params
     * returns a vector of type instances this depends on
     */
    std::vector<type_instance>& type_instance::get_params() {
        return m_params;
    }

    const std::vector<type_instance>& type_instance::get_params() const {
        return m_params;
    }

    /**
     * set_return_type_instance
     * if this is a functional type instance, this function allows us to set its return type instance
     */
    void type_instance::set_return_type_instance(type_instance return_type_instance) {
        m_return_type_instance = std::make_shared<type_instance>(return_type_instance);
    }

    /**
     * get_return_type_instance
     * return the return type instance if this type instance is a functional type instance
     * if it is not a functional type instance, this function throws a "type_error" exception
     */
    type_instance type_instance::get_return_type_instance() {
        if(m_return_type_instance == nullptr)
            throw type_error("This type instance is does not have a return type instance.");
        else
            return * m_return_type_instance;
    }

    type_instance type_instance::get_return_type_instance() const {
        if(m_return_type_instance == nullptr)
            throw type_error("This type instance is does not have a return type instance.");
        else
            return * m_return_type_instance;
    }

    /**
     * set_is_parametrized
     * if the type instance was declared within a type that depends on parametric types,
     * this function marks it as such.
     */
    void type_instance::is_parametrized(bool is_parametrized) {
        m_is_parametrized = is_parametrized;
    }

    bool type_instance::is_parametrized() const {
        return m_is_parametrized;
    }

    bool type_instance::is_parametrized() {
        return m_is_parametrized;
    }

    /**
     * is_abstract
     * returns true if this type instance is a parametric type, false otherwise
     */
    bool type_instance::is_abstract() {
        if(m_type == nullptr)
            return false;
        else
            return true;
    }

    bool type_instance::is_abstract() const {
        if(m_type == nullptr)
            return false;
        else
            return true;
    }

    /**
     * mangle_type_constructor
     * returns a string representation of a type instance
     */
    std::string mangle_type_instance(const type_instance& instance) {
        std::vector<type_instance> params = instance.get_params();
        std::string mangled_name = "";

        if(instance.is_abstract()) {
            mangled_name += "*";
        }
        else {
            if(instance.get_category() == USER) {
                mangled_name += instance.get_name();                
                if(!params.empty()) {
                    mangled_name += std::string("(");
                    for(auto it = params.begin(); it != params.end(); ++it) {
                        mangled_name += mangle_type_instance(* it);

                        if((it != params.end()) && (it + 1 != params.end()))
                            mangled_name += std::string(",");
                    }
                    mangled_name += std::string(")");
                }
            }
            else if(instance.get_category() == TUPLE) {
                mangled_name += "(";
                if(!params.empty()) {
                    for(auto it = params.begin(); it != params.end(); ++it) {
                        mangled_name += mangle_type_instance(* it);

                        if((it != params.end()) && (it + 1 != params.end()))
                            mangled_name += std::string(",");
                    }
                }
                mangled_name += std::string(")");
            }
            else if(instance.get_category() == LIST) {
                mangled_name += "[";
                mangled_name += mangle_type_instance(params[0]);
                mangled_name += std::string("]");
            }
            else if(instance.get_category() == MAP) {
                mangled_name += "{";
                mangled_name += mangle_type_instance(params[0]);
                mangled_name += ":";
                mangled_name += mangle_type_instance(params[1]);
                mangled_name += std::string("}");
            }
            
        }

        return mangled_name;
    }

    /**
     * operator<<
     * print the type instance to the current stream
     */
    std::ostream& operator<<(std::ostream& os, const type_instance& instance) {
        std::string mangled_name = mangle_type_instance(instance);
        os << mangled_name;
        return os;
    }

    /**
     * type_instance_strong_compare
     * does a strong comparison of type instances
     * this means that a concrete type instance cannot be equal to an abstract type instance
     */
    bool type_instance_strong_compare(type_instance& this_instance, type_instance& that_instance) {
        const std::vector<type_instance>& this_params = this_instance.get_params();
        const std::vector<type_instance>& that_params = that_instance.get_params();
        
        // get this type instance type and parameters
        std::shared_ptr<type> this_type = nullptr;
        if(this_instance.is_abstract() == false)
            this_type = this_instance.get_type();

        // get the other type instance type and parameters
        std::shared_ptr<type> that_type = nullptr;
        if(that_instance.is_abstract() == false)
            that_type = that_instance.get_type();
        
        // if both type instances are parametric types, then they are equal
        if(this_type == nullptr && that_type == nullptr)
            return true;

        // if this type instance has no type (it is parametric) but the other has, they are not equal
        else if(this_type == nullptr && that_type != nullptr)
            return false;

        // if this type instance has a type but the other doesn't (it is parametric), they are not equal
        else if(this_type != nullptr && that_type == nullptr)
            return false;

        // if both have types, we look at parameters, if any
        else {
            // if both don't admit parameters, we compare types for equality and return that as a result
            if(this_params.size() == 0 && that_params.size() == 0)
                return (* this_type) == (* that_type);

            // if they don't admit they same number of parameters,
            // they were not built from the same type even though they may share the same name
            if(this_params.size() != that_params.size())
                return false;

            // if they both admit they same number of parameters, we compare their types and their parameters
            // if they don't have the same type, they are not equal
            if((* this_type) != (* that_type)) {
                return false;
            }
            // if both type instances are built from the same type, we make sure their parameters are also equal
            else {
                for(auto this_it = this_params.begin(), that_it = that_params.begin(); this_it != this_params.end() && that_it != that_params.end(); ++this_it, ++that_it)
                    // if any of the parameters they depend on are not equal, then both type instances are not equal
                    if(type_instance_strong_compare(const_cast<type_instance&>(* this_it), const_cast<type_instance&>(* that_it)) == false)
                        return false;

                // if we are here, then all parameters are equal
                return true;
            }
        }
    }

    bool type_instance_strong_compare(const type_instance& this_instance, const type_instance& that_instance) {
        return type_instance_strong_compare(const_cast<type_instance&>(this_instance), const_cast<type_instance&>(that_instance));
    }

    /**
     * type_instance_weak_compare
     * does a weak comparison of type instances
     * this means that a concrete type instance cann be equal to an abstract type instance
     */
    bool type_instance_weak_compare(type_instance& this_instance, type_instance& that_instance) {
        const std::vector<type_instance>& this_params = this_instance.get_params();
        const std::vector<type_instance>& that_params = that_instance.get_params();
        
        // get this type instance type and parameters
        std::shared_ptr<type> this_type = nullptr;
        if(this_instance.is_abstract() == false)
            this_type = this_instance.get_type();

        // get the other type instance type and parameters
        std::shared_ptr<type> that_type = nullptr;
        if(that_instance.is_abstract() == false)
            that_type = that_instance.get_type();
        
        // if one of the type instances if abstract, then they are equal
        if(this_type == nullptr || that_type == nullptr)
            return true;

        // if both have types, we look at parameters, if any
        else {
            // if both don't admit parameters, we compare types for equality and return that as a result
            if(this_params.size() == 0 && that_params.size() == 0)
                return (* this_type) == (* that_type);

            // if they don't admit they same number of parameters,
            // they were not built from the same type even though they may share the same name
            if(this_params.size() != that_params.size())
                return false;

            // if they both admit they same number of parameters, we compare their types and their parameters
            // if they don't have the same type, they are not equal
            if((* this_type) != (* that_type)) {
                return false;
            }
            // if both type instances are built from the same type, we make sure their parameters are also equal
            else {
                for(auto this_it = this_params.begin(), that_it = that_params.begin(); this_it != this_params.end() && that_it != that_params.end(); ++this_it, ++that_it)
                    // if any of the parameters they depend on are not equal, then both type instances are not equal
                    if(type_instance_weak_compare(const_cast<type_instance&>(* this_it), const_cast<type_instance&>(* that_it)) == false)
                        return false;

                // if we are here, then all parameters are equal
                return true;
            }
        }
    }

    bool type_instance_weak_compare(const type_instance& this_instance, const type_instance& that_instance) {
        return type_instance_weak_compare(const_cast<type_instance&>(this_instance), const_cast<type_instance&>(that_instance));
    }


/**
 * the constructor expects:
 * - the token with the constructor name
 * - the type that this constructor contructs
 */
default_constructor::default_constructor(token& tok, std::shared_ptr<type>& ty) : m_name(tok.get_lexeme()), m_tok(tok), m_type(ty), m_is_parametrized(false) {
}

    /**
     * set_type
     * changes the name of the constructor.
     */
    void default_constructor::set_name(const std::string& name) {
        m_name = name;
    }

    /**
     * get_name
     * returns a string with the name of the constructor
     */
    const std::string& default_constructor::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns the token that contains the name of this constructor.
     */
    const token& default_constructor::get_token() const {
        return m_tok;
    }

    /**
     * get_type
     * returns the type that this constructor creates
     */
    std::shared_ptr<type>& default_constructor::get_type() {
        return m_type;
    }

    /**
     * add_param
     * if the contructor is not an nullary constructor,
     * this function allows us to save type instances that the constructor relies on.
     */
    void default_constructor::add_param(type_instance& param) {
        m_params.push_back(param);
    }

    /**
     * get_params
     * returns a vector of type instances that make up the constructor parameters.
     */
    std::vector<type_instance>& default_constructor::get_params() {
        return m_params;
    }

    /**
     * set_is_parametrized
     * if the constructor relies on an unknown type,
     * we use this function to set this flag on.
     */
    void default_constructor::set_is_parametrized(bool is_parametrized) {
        m_is_parametrized = is_parametrized;
    }

    /**
     * is_parametrized
     * returns true if the constructor relies on an abstract type.
     */
    bool default_constructor::is_parametrized() {
        return m_is_parametrized;
    }


/**
 * the constructor expects:
 * - the token with the constructor name
 * - the type that this constructor contructs
 */
record_constructor::record_constructor(token& tok, std::shared_ptr<type>& ty) : m_name(tok.get_lexeme()), m_tok(tok), m_type(ty), m_is_parametrized(false) {
}

    /**
     * set_type
     * changes the name of the constructor.
     */
    void record_constructor::set_name(const std::string& name) {
        m_name = name;
    }

    /**
     * get_name
     * returns a string with the name of the constructor
     */
    const std::string& record_constructor::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns the token that contains the name of this constructor.
     */
    const token& record_constructor::get_token() const {
        return m_tok;
    }

    /**
     * get_type
     * returns the type that this constructor creates
     */
    std::shared_ptr<type>& record_constructor::get_type() {
        return m_type;
    }

    /**
     * add_param
     * given the parameter name (as a token) and a type instance,
     * add both to the map of parameters.
     * we use the token as parameter name instead of a string for error reporting purposes.
     * when showing error messages, we need the information the token holds.
     */
    void record_constructor::add_param(token param_name, type_instance& param_type) {
        std::pair<token, type_instance> data(param_name, param_type);
        m_params.insert(data);
    }

    /**
     * get_params
     * returns a map of parameter names and type instances.
     */
    std::map<token,type_instance>& record_constructor::get_params() {
        return m_params;
    }

    /**
     * get_params_as_vector
     * returns a vector of type instances,
     * essentially transforming a record constructor into a default constructor
     */
    std::vector<type_instance>& record_constructor::get_params_as_vector() {
        if(m_params_vector.empty()) {
            for(auto it = m_params.begin(); it != m_params.end(); ++it) {
                m_params_vector.push_back(it -> second);
            } 
        }

        return m_params_vector;
    }

    /**
     * set_is_parametrized
     * if the constructor relies on an unknown type,
     * we use this function to set this flag on.
     */
    void record_constructor::set_is_parametrized(bool is_parametrized) {
        m_is_parametrized = is_parametrized;
    }

    /**
     * is_parametrized
     * returns true if the constructor relies on an abstract type.
     */
    bool record_constructor::is_parametrized() {
        return m_is_parametrized;
    }


/**
 * the constructor expects:
 * - the token with the constructor name
 * - the type that this constructor contructs
 * - the type instance that this constructor depends on
 */
list_constructor::list_constructor(token& tok, std::shared_ptr<type>& ty, type_instance& param) : m_name(tok.get_lexeme()), m_tok(tok), m_type(ty), m_param(param), m_is_parametrized(false) {
}

    /**
     * set_type
     * changes the name of the constructor.
     */
    void list_constructor::set_name(const std::string& name) {
        m_name = name;
    }

    /**
     * get_name
     * returns a string with the name of the constructor
     */
    const std::string& list_constructor::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns the token that contains the name of this constructor.
     */
    const token& list_constructor::get_token() const {
        return m_tok;
    }

    /**
     * get_type
     * returns the type that this constructor creates
     */
    std::shared_ptr<type>& list_constructor::get_type() {
        return m_type;
    }

    /**
     * add_param
     * set type instance this constructor depends on
     */
    void list_constructor::add_param(type_instance& param) {
        m_param = param;
    }

    /**
     * get_param
     * return the type instance this list constructor depends on
     */
    type_instance& list_constructor::get_param() {
        return m_param;
    }

    /**
     * set_is_parametrized
     * if the constructor relies on an unknown type,
     * we use this function to set this flag on.
     */
    void list_constructor::set_is_parametrized(bool is_parametrized) {
        m_is_parametrized = is_parametrized;
    }

    /**
     * is_parametrized
     * returns true if the constructor relies on an abstract type.
     */
    bool list_constructor::is_parametrized() {
        return m_is_parametrized;
    }


/**
 * the constructor expects:
 * - the token with the constructor name
 * - the type that this constructor contructs
 * - the type instance of the constructor key
 * - the type instance of the constructor value
 */
map_constructor::map_constructor(token& tok, std::shared_ptr<type>& ty, type_instance& param_key, type_instance& param_value) : m_name(tok.get_lexeme()), m_tok(tok), m_type(ty), m_param_key(param_key), m_param_value(param_value), m_is_parametrized(false) {
}

    /**
     * set_type
     * changes the name of the constructor.
     */
    void map_constructor::set_name(const std::string& name) {
        m_name = name;
    }

    /**
     * get_name
     * returns a string with the name of the constructor
     */
    const std::string& map_constructor::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns the token that contains the name of this constructor.
     */
    const token& map_constructor::get_token() const {
        return m_tok;
    }

    /**
     * get_type
     * returns the type that this constructor creates
     */
    std::shared_ptr<type>& map_constructor::get_type() {
        return m_type;
    }

    /**
     * add_param_key
     * set type instance this map constructor key
     */
    void map_constructor::add_param_key(type_instance& param) {
        m_param_key = param;
    }

    /**
     * get_param_key
     * return the type instance this map constructor key
     */
    type_instance& map_constructor::get_param_key() {
        return m_param_key;
    }
    
    /**
     * add_param_value
     * set type instance this map constructor value
     */
    void map_constructor::add_param_value(type_instance& param) {
        m_param_value = param;
    }

    /**
     * get_param_value
     * return the type instance this map constructor value
     */
    type_instance& map_constructor::get_param_value() {
        return m_param_value;
    }

    /**
     * set_is_parametrized
     * if the constructor relies on an unknown type,
     * we use this function to set this flag on.
     */
    void map_constructor::set_is_parametrized(bool is_parametrized) {
        m_is_parametrized = is_parametrized;
    }

    /**
     * is_parametrized
     * returns true if the constructor relies on an abstract type.
     */
    bool map_constructor::is_parametrized() {
        return m_is_parametrized;
    }


/**
 * mangle_constructor
 * produces a string version of a constructor for use within maps, usually
 */
std::string mangle_constructor(const std::string& name, std::vector<type_instance>& params) {
    std::string mangled_name = std::string("__");
    mangled_name += name;

    for(auto it = params.begin(); it != params.end(); ++it) {
        if(it == params.begin())
            mangled_name += std::string("_");

        mangled_name += mangle_type_instance(* it);

        if((it != params.end()) && (it + 1 != params.end()))
            mangled_name += std::string("_");
    }

    mangled_name += std::string("__");
    return mangled_name;
}
}
