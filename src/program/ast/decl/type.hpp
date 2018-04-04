#ifndef AVALON_PROGRAM_AST_TYPE_HPP_
#define AVALON_PROGRAM_AST_TYPE_HPP_

#include <utility>
#include <memory>
#include <vector>
#include <map>
#include <new>

#include "lexer/token.hpp"
#include "program/ast/decl/decl.hpp"


namespace avalon {
    class type;
    class type_instance;
    class map_constructor;
    class list_constructor;
    class record_constructor;
    class default_constructor;

    /*
     * type checking data
     * valid    : the object is in a consistent state
     * invalid  : the object failed type checking
     * unknown  : the object has not been type checked yet
     */
    enum validation_state {
        VALID,
        INVALID,
        UNKNOWN
    };

    /*
     * types of type instances
     * USER     : user defined type instance
     * TUPLE    : built-in tuple type instance
     * LIST     : built-in list type instance
     * MAP      : built-in map type instance
     */
    enum type_instance_category {
        USER,
        TUPLE,
        LIST,
        MAP
    };


    class type: public decl {
    public:
        /**
         * the constructor expects:
         * - the token with type information
         * - the validation state
         */
        type(token& tok, validation_state is_valid);

        /**
         * set_name
         * updates the name of this type
         */
        void set_name(const std::string& name);

        /**
         * get_name
         * returns the name of the type
         */
        const std::string& get_name() const;

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * add_constructor
         * add a default constructor to this type
         */
        void add_constructor(default_constructor& def_constructor);

        /**
         * add_constructor
         * add a record constructor to this type
         */
        void add_constructor(record_constructor& rec_constructor);

        /**
         * add_constructor
         * add a list constructor to this type
         */
        void add_constructor(list_constructor& list_constructor);

        /**
         * add_constructor
         * add a map constructor to this type
         */
        void add_constructor(map_constructor& map_constructor);

        /**
         * replace_constructor
         * replace a default constructor found on this type
         */
        void replace_constructor(default_constructor& def_constructor);

        /**
         * replace_constructor
         * replace a record constructor found on this type
         */
        void replace_constructor(record_constructor& rec_constructor);

        /**
         * replace_constructor
         * replace a list constructor found on this type
         */
        void replace_constructor(list_constructor& list_constructor);

        /**
         * replace_constructor
         * replace a map constructor found on this type
         */
        void replace_constructor(map_constructor& map_constructor);

        /**
         * get_default_constructors
         * returns a vector of all default constructors that build this type
         */
        std::vector<default_constructor> get_default_constructors();

        /**
         * get_record_constructors
         * returns a vector of all record constructors that build this type
         */
        std::vector<record_constructor> get_record_constructors();

        /**
         * get_list_constructors
         * returns a vector of all list constructors that build this type
         */
        std::vector<list_constructor> get_list_constructors();

        /**
         * get_map_constructors
         * returns a vector of all map constructors that build this type
         */
        std::vector<map_constructor> get_map_constructors();

        /**
         * get_default_constructors
         * returns a vector of default constructors matching the name given
         */
        std::vector<default_constructor> get_default_constructors(const std::string& name);

        /**
         * get_record_constructors
         * returns a vector of record constructors matching the name given
         */
        std::vector<record_constructor> get_record_constructors(const std::string& name);

        /**
         * get_default_constructor
         * given a default constructor name and arity, find it there is one that builds this type
         */
        default_constructor& get_default_constructor(const std::string& name, std::size_t arity);

        /**
         * get_record_constructor
         * given a record constructor name and arity, find it there is one that builds this type
         */
        record_constructor& get_record_constructor(const std::string& name, std::size_t arity);

        /**
         * get_list_constructor
         * given a list constructor name, find it there is one that builds this type
         */
        list_constructor& get_list_constructor(const std::string& name);

        /**
         * get_map_constructor
         * given a map constructor name, find it there is one that builds this type
         */
        map_constructor& get_map_constructor(const std::string& name);

        /**
         * add_type_param
         * if this is a parametric type, this function allows us to add type parameters
         */
        void add_param(token& type_param);

        /**
         * get_params
         * returns a vector with all type parameters if any.
         */
        const std::vector<token>& get_params() const;

        /**
         * set_is_valid
         * updates the type current validation state.
         */
        void set_is_valid(validation_state state);

        /**
         * is_valid
         * given a validation state,
         * this function returns true if the type has the given validation state.
         */
        bool is_valid(validation_state state);

        /*
         * is_type
         * returns true as this declaration is indeed a type
         */
        virtual bool is_type() {
            return true;
        }

        /**
         * is_public
         * sets and returns a boolean indicating whether this declaration can be imported
         */
        virtual bool is_public() {
            return m_is_public;
        }
        virtual void is_public(bool public_) {
            m_is_public = public_;
        }

        /**
         * is_reachable
         * sets and returns a boolean indicating whether this type declaration will be executed
         */
        virtual bool is_reachable() {
            return true;
        }
        virtual void is_reachable(bool reachable) {
        }
        
        /**
         * sets and returns a boolean that indicates whether this type declaration will cause a function to return
         */
        virtual bool terminates() {
            return false;
        }
        virtual void terminates(bool terminates) {
        }

        /**
         * passes
         * sets and returns a boolean indicating whether this type declaration allows the next declaration to be exeuted
         */
        virtual bool passes() {
            return true;
        }
        virtual void passes(bool passes) {            
        }

    private:
        /*
         * the name of the type
         */
        std::string m_name;

        /*
         * the token with source file information about the type
         */
        token m_tok;

        /*
         * map of all default constructors that build this type
         */
        std::map<std::pair<std::string,std::size_t>, default_constructor> m_def_constructors;

        /*
         * map of all record constructors that build this type
         */
        std::map<std::pair<std::string,std::size_t>, record_constructor> m_rec_constructors;

        /*
         * map of all list constructors that build this type
         */
        std::map<std::string, list_constructor> m_list_constructors;

        /*
         * map of all map constructors that build this type
         */
        std::map<std::string, map_constructor> m_map_constructors;

        /*
         * vector of type parameters if this type was parametrized
         */
        std::vector<token> m_params;

        /*
         * the current validation state of the type
         */
        validation_state m_is_valid;

        /*
         * whether this type is available for public use outside of the namespace where it was declared
         * true if it is, false otherwise
         */
        bool m_is_public;
    };
    
    /**
     * operator==
     * compares two types for equality
     */
    inline bool operator==(const type& lhs, const type& rhs) {
        if (lhs.get_name() == rhs.get_name() && lhs.get_params().size() == rhs.get_params().size())
            return true;
        else
            return false;
    }
    
    /**
     * operator!=
     * compares two types for inequality
     */
    inline bool operator!=(const type& lhs, const type& rhs) {
        return !(lhs == rhs);
    }

    /**
     * mangle_type
     * give a type name and the number of type parameters accepts,
     * we create a string that represents the true type name as
     * the new name takes into account the parameters the type accepts.
     */
    std::string mangle_type(const std::string& name, int params_count);


    class type_instance {
    public:
        /**
         * the default constructor expects nothing
         */
        type_instance();
        
        /**
         * this constructor expects the token with source code information and the namespace where to find the type
         */
        type_instance(token& tok, const std::string& namespace_name);
        
        /**
         * this constructor expects the token with source code information and the type that buils this instance
         */
        type_instance(token& tok, std::shared_ptr<type>& ty, const std::string& namespace_name);

        /**
         * set_name
         * updates the name of this type
         */
        void set_name(const std::string& name);

        /**
         * get_name
         * returns the name of the type instance
         */
        const std::string& get_name() const;

        /**
         * set_token
         * changes the token and the name of this type instance
         */
        void set_token(token tok);

        /**
         * get_token
         * returns a token with type source information
         */
        const token& get_token() const;

        /**
         * set_old_token
         * When an instance is updated from an abstract one to a concrete one
         * this function is called to keep a record of the old token
         */
        void set_old_token(token tok);

        /**
         * get_old_token
         * returns the old token
         */
        const token& get_old_token() const;

        /**
         * set_category
         * set the category to which this type instance belongs
         */
        void set_category(type_instance_category category);

        /**
         * get_category
         * get the category to which this type instance belongs
         */
        const type_instance_category& get_category() const;

        /**
         * get_namespace
         * returns the namespace where to find the type declaration that builds this type instance
         */
        const std::string& get_namespace() const;

        /**
         * set_type
         * sets the type that constructs this type instance
         */
        void set_type(std::shared_ptr<type>& ty);

        /**
         * get_type
         * returns the type that corresponds to this type instance
         */
        std::shared_ptr<type>& get_type();

        /**
         * is_builtby
         * given a type declaration, return true if that type builds this type instance
         */
        bool is_builtby(std::shared_ptr<type>& ty);

        /**
         * add_param
         * a type instance depends on other type instances if
         * the type constructor that builds it is parametric.
         */
        void add_param(type_instance& param);

        /**
         * get_params
         * returns a vector of type instances this depends on
         */
        std::vector<type_instance>& get_params();
        const std::vector<type_instance>& get_params() const;

        /**
         * set_return_type_instance
         * if this is a functional type instance, this function allows us to set its return type instance
         */
        void set_return_type_instance(type_instance return_type_instance);

        /**
         * get_return_type_instance
         * return the return type instance if this type instance is a functional type instance
         * if it is not a functional type instance, this function throws a "type_error" exception
         */
        type_instance get_return_type_instance();
        type_instance get_return_type_instance() const;

        /**
         * is_parametrized
         * if the type instance was declared within a type that depends on parametric types,
         * this function marks it as such.
         */
        void is_parametrized(bool is_parametrized);
        bool is_parametrized() const;
        bool is_parametrized();

        /**
         * is_abstract
         * returns true if this type instance is a parametric type, false otherwise
         */
        void is_abstract(bool is_abstract);
        bool is_abstract();
        bool is_abstract() const;

    private:
        /*
         * the name of the type
         */
        std::string m_name;

        /*
         * the token with source file information about the type
         */
        token m_tok;

        /*
         * the old token when this type instance was an abstract instance
         */
        token m_old_tok;

        /*
         * the category to which this type instance belongs
         */
        type_instance_category m_category;

        /*
         * the name of the namespace where to find the type builder of this type instance
         */
        std::string m_namespace;

        /*
         * the type the constructs this type instance
         */
        std::shared_ptr<type> m_type;

        /*
         * type instances this type instance depends on
         */
        std::vector<type_instance> m_params;

        /*
         * if this type instance is a functional type instance, it will admit a return type instance
         */
        std::shared_ptr<type_instance> m_return_type_instance;
        
        /*
         * flag indicating whether this type instance depends on an abstract type instance
         */
        bool m_is_parametrized;

    /**
     * a friend function that helps us std::cout type instances
     */
    friend std::ostream& operator<<(std::ostream& os, const type_instance& instance);
    };

    /**
     * mangle_type_constructor
     * returns a string representation of a type instance
     */
    std::string mangle_type_instance(const type_instance& instance);

    /**
     * operator<<
     * print the type instance to the current stream
     */
    std::ostream& operator<<(std::ostream& os, const type_instance& instance);

    /**
     * strong_compare
     * does a strong comparison of type instances
     * this means that a concrete type instance cannot be equal to an abstract type instance
     */
    bool type_instance_strong_compare(type_instance& this_instance, type_instance& that_instance);
    bool type_instance_strong_compare(const type_instance& this_instance, const type_instance& that_instance);

    /**
     * weak_compare
     * does a strong comparison of type instances
     * this means that a concrete type instance cann be equal to an abstract type instance
     */
    bool type_instance_weak_compare(type_instance& this_instance, type_instance& that_instance);
    bool type_instance_weak_compare(const type_instance& this_instance, const type_instance& that_instance);


    class default_constructor {
    public:
        /**
         * the constructor expects:
         * - the token with the constructor name
         * - the type that this constructor contructs
         */
        default_constructor(token& tok, std::shared_ptr<type>& ty);

        /**
         * set_type
         * changes the name of the constructor.
         */
        void set_name(const std::string& name);

        /**
         * get_name
         * returns a string with the name of the constructor
         */
        const std::string& get_name() const;

        /**
         * get_token
         * returns the token that contains the name of this constructor.
         */
        const token& get_token() const;

        /**
         * get_type
         * returns the type that this constructor creates
         */
        std::shared_ptr<type>& get_type();

        /**
         * add_param
         * if the contructor is not an nullary constructor,
         * this function allows us to save type instances that the constructor relies on.
         */
        void add_param(type_instance& param);

        /**
         * get_params
         * returns a vector of type instances that make up the constructor parameters.
         */
        std::vector<type_instance>& get_params();

        /**
         * set_is_parametrized
         * if the constructor relies on an unknown type,
         * we use this function to set this flag on.
         */
        void set_is_parametrized(bool is_parametrized);

        /**
         * is_parametrized
         * returns true if the constructor relies on an abstract type.
         */
        bool is_parametrized();

    private:
        /*
         * name of the constructor
         */
        std::string m_name;

        /*
         * token that contains the following relevant information
         */
        token m_tok;

        /*
         * a vector of type instances for nary constructors
         */
        std::vector<type_instance> m_params;

        /*
         * the type that this contructor creates
         */
        std::shared_ptr<type> m_type;

        /*
         * flag set to true if the constructor depends on an abstract type
         */
        bool m_is_parametrized;
    };


    class record_constructor {
    public:
        /**
         * the constructor expects:
         * - the token with the constructor name
         * - the type that this constructor contructs
         */
        record_constructor(token& tok, std::shared_ptr<type>& ty);

        /**
         * set_type
         * changes the name of the constructor.
         */
        void set_name(const std::string& name);

        /**
         * get_name
         * returns a string with the name of the constructor
         */
        const std::string& get_name() const;

        /**
         * get_token
         * returns the token that contains the name of this constructor.
         */
        const token& get_token() const;

        /**
         * get_type
         * returns the type that this constructor creates
         */
        std::shared_ptr<type>& get_type();
        
        /**
         * add_param
         * given the parameter name (as a token) and a type instance,
         * add both to the map of parameters.
         * we use the token as parameter name instead of a string for error reporting purposes.
         * when showing error messages, we need the information the token holds.
         */
        void add_param(token param_name, type_instance& param_type);

        /**
         * get_params
         * returns a map of parameter names and type instances.
         */
        std::map<token,type_instance>& get_params();

        /**
         * get_params_as_vector
         * returns a vector of type instances,
         * essentially transforming a record constructor into a default constructor
         */
        std::vector<type_instance>& get_params_as_vector();

        /**
         * set_is_parametrized
         * if the constructor relies on an unknown type,
         * we use this function to set this flag on.
         */
        void set_is_parametrized(bool is_parametrized);

        /**
         * is_parametrized
         * returns true if the constructor relies on an abstract type.
         */
        bool is_parametrized();

    private:
        /*
         * name of the constructor
         */
        std::string m_name;

        /*
         * token that contains the following relevant information
         */
        token m_tok;

        /*
         * a map between parameter names and their type[s] (instances)
         */
        std::map<token, type_instance> m_params;

        /*
         * a vector of types
         */
        std::vector<type_instance> m_params_vector;

        /*
         * the type that this contructor creates
         */
        std::shared_ptr<type> m_type;

        /*
         * flag set to true if the constructor depends on an abstract type
         */
        bool m_is_parametrized;
    };


    class list_constructor {
    public:
        /**
         * the constructor expects:
         * - the token with the constructor name
         * - the type that this constructor contructs
         * - the type instance that this constructor depends on
         */
        list_constructor(token& tok, std::shared_ptr<type>& ty, type_instance& param);

        /**
         * set_type
         * changes the name of the constructor.
         */
        void set_name(const std::string& name);

        /**
         * get_name
         * returns a string with the name of the constructor
         */
        const std::string& get_name() const;

        /**
         * get_token
         * returns the token that contains the name of this constructor.
         */
        const token& get_token() const;

        /**
         * get_type
         * returns the type that this constructor creates
         */
        std::shared_ptr<type>& get_type();
        
        /**
         * add_param
         * set type instance this constructor depends on
         */
        void add_param(type_instance& param);

        /**
         * get_param
         * return the type instance this list constructor depends on
         */
        type_instance& get_param();

        /**
         * set_is_parametrized
         * if the constructor relies on an unknown type,
         * we use this function to set this flag on.
         */
        void set_is_parametrized(bool is_parametrized);

        /**
         * is_parametrized
         * returns true if the constructor relies on an abstract type.
         */
        bool is_parametrized();

    private:
        /*
         * name of the constructor
         */
        std::string m_name;

        /*
         * token that contains the following relevant information
         */
        token m_tok;

        /*
         * the type that this contructor creates
         */
        std::shared_ptr<type> m_type;

        /*
         * the type instance this list constructor depends on
         */
        type_instance m_param;

        /*
         * flag set to true if the constructor depends on an abstract type
         */
        bool m_is_parametrized;
    };


    class map_constructor {
    public:
        /**
         * the constructor expects:
         * - the token with the constructor name
         * - the type that this constructor contructs
         * - the type instance of the constructor key
         * - the type instance of the constructor value
         */
        map_constructor(token& tok, std::shared_ptr<type>& ty, type_instance& param_key, type_instance& param_value);

        /**
         * set_type
         * changes the name of the constructor.
         */
        void set_name(const std::string& name);

        /**
         * get_name
         * returns a string with the name of the constructor
         */
        const std::string& get_name() const;

        /**
         * get_token
         * returns the token that contains the name of this constructor.
         */
        const token& get_token() const;

        /**
         * get_type
         * returns the type that this constructor creates
         */
        std::shared_ptr<type>& get_type();
        
        /**
         * add_param_key
         * set type instance this map constructor key
         */
        void add_param_key(type_instance& param);

        /**
         * get_param_key
         * return the type instance this map constructor key
         */
        type_instance& get_param_key();
        
        /**
         * add_param_value
         * set type instance this map constructor value
         */
        void add_param_value(type_instance& param);

        /**
         * get_param_value
         * return the type instance this map constructor value
         */
        type_instance& get_param_value();

        /**
         * set_is_parametrized
         * if the constructor relies on an unknown type,
         * we use this function to set this flag on.
         */
        void set_is_parametrized(bool is_parametrized);

        /**
         * is_parametrized
         * returns true if the constructor relies on an abstract type.
         */
        bool is_parametrized();

    private:
        /*
         * name of the constructor
         */
        std::string m_name;

        /*
         * token that contains the following relevant information
         */
        token m_tok;

        /*
         * the type that this contructor creates
         */
        std::shared_ptr<type> m_type;

        /*
         * the type instance of the map key
         */
        type_instance m_param_key;

        /*
         * the type instance of map value
         */
        type_instance m_param_value;

        /*
         * flag set to true if the constructor depends on an abstract type
         */
        bool m_is_parametrized;
    };

    /**
     * mangle_constructor
     * produces a string version of a constructor for use within maps, usually
     */
    std::string mangle_constructor(const std::string& name, std::vector<type_instance>& params);
}

#endif
