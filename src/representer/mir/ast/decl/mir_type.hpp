#ifndef AVALON_MIR_AST_TYPE_HPP_
#define AVALON_MIR_AST_TYPE_HPP_

#include <utility>
#include <cstddef>
#include <vector>
#include <string>
#include <map>

#include "representer/mir/ast/decl/mir_decl.hpp"
#include "representer/hir/symtable/fqn.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * forward declarations
     */
    class mir_record_constructor;
    class mir_default_constructor;


    class mir_type : public mir_decl {
    public:
        /**
         * the constructor expects:
         * - the name of the type
         * - the token with source code information
         */
        mir_type(const std::string& name, const token& tok);

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
         * set_fqn
         * sets the fqn where this declaration was found
         */
        void set_fqn(fqn& l_fqn);

        /**
         * get_fqn
         * returns the fqn where this declaration was found
         */
        fqn& get_fqn();

        /**
         * set_namespace
         * set the namespace where this declaration was found
         */
        void set_namespace(const std::string& l_namespace);

        /**
         * get_namespace
         * returns the namespace where this declaration was found
         */
        const std::string& get_namespace() const;

        /**
         * add_constructor
         * add a default constructor to the type
         */
        void add_constructor(mir_default_constructor& def_cons);

        /**
         * add_constructor
         * add a record constructor to the type
         */
        void add_constructor(mir_record_constructor& rec_cons);

        /**
         * get_default_constructor
         * find the default constructor that has the given name and arity
         */
        mir_default_constructor& get_default_constructor(const std::string& name, std::size_t arity);

        /**
         * get_record_constructor
         * find the record constructor that has the given name and arity
         */
        mir_record_constructor& get_record_constructor(const std::string& name, std::size_t arity);

        /**
         * get_default_constructors
         * returns a vector of all the default constructors that can create this type
         */
        std::vector<mir_default_constructor> get_default_constructors();

        /**
         * get_record_constructors
         * returns a vector of all the record constructors that can create this type
         */
        std::vector<mir_record_constructor> get_record_constructors();

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
         * sets and returns a boolean indicating whether this declaration will be executed
         */
        virtual bool is_reachable() {
            return true;
        }
        virtual void is_reachable(bool reachable) {
        }

        /**
         * terminates
         * sets and returns a boolean indicating whether this declaration will cause a function to return
         */
        virtual bool terminates() {
            return false;
        }
        virtual void terminates(bool terminates) {
        }

        /**
         * passes
         * sets and returns a boolean indicating whether this declaration allows the next declaration to be exeuted
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
         * token with source code information
         */
        token m_tok;

        /*
         * the FQN where this type exists
         */
        fqn m_fqn;

        /*
         * the namespace where this type exists
         */
        std::string m_namespace;

        /*
         * the map of default constructors that build this type
         */
        std::map<std::pair<std::string, std::size_t>, mir_default_constructor> m_def_cons;

        /*
         * the map of record constructors that build this type
         */
        std::map<std::pair<std::string, std::size_t>, mir_record_constructor> m_rec_cons;

        /*
         * whether this type declaration is available for import
         */
        bool m_is_public;
    };

    /**
     * operator==
     * compares two types for equality
     */
    inline bool operator==(const mir_type& lhs, const mir_type& rhs) {
        if (lhs.get_name() == rhs.get_name())
            return true;
        else
            return false;
    }
    
    /**
     * operator!=
     * compares two types for inequality
     */
    inline bool operator!=(const mir_type& lhs, const mir_type& rhs) {
        return !(lhs == rhs);
    }


    class mir_default_constructor {
    public:
        /**
         * the constructor expects:
         * - the token with the constructor name
         * - the type that this constructor contructs
         */
        mir_default_constructor(const std::string& name, token& tok, mir_type& l_type);

        /**
         * get_name
         * returns a string with the name of the constructor
         */
        const std::string& get_name() const;

        /**
         * get_token
         * returns the token that contains the name of this constructor
         */
        const token& get_token() const;

        /**
         * get_type
         * returns the type that this constructor creates
         */
        mir_type& get_type();

        /**
         * add_param
         * if the contructor is not an nullary constructor,
         * this function allows us to save types that the constructor relies on
         */
        void add_param(mir_type& param);

        /**
         * get_params
         * returns a vector of types that make up the constructor parameters
         */
        std::vector<mir_type>& get_params();

    private:
        /*
         * name of the constructor
         */
        std::string m_name;

        /*
         * token with source code information
         */
        token m_tok;

        /*
         * the type that this contructor creates
         */
        mir_type m_type;

        /*
         * a vector of types for nary constructors
         */
        std::vector<mir_type> m_params;
    };


    class mir_record_constructor {
    public:
        /**
         * the constructor expects:
         * - the token with the constructor name
         * - the type that this constructor contructs
         */
        mir_record_constructor(const std::string& name, token& tok, mir_type& l_type);

        /**
         * get_name
         * returns a string with the name of the constructor
         */
        const std::string& get_name() const;

        /**
         * get_token
         * returns the token that contains the name of this constructor
         */
        const token& get_token() const;

        /**
         * get_type
         * returns the type that this constructor creates
         */
        mir_type& get_type();

        /**
         * add_param
         * this function allows us to save types that the constructor relies on
         */
        void add_param(token& param_tok, mir_type& param_type);

        /**
         * get_params
         * returns a map of types that make up the constructor parameters
         */
        std::map<token, mir_type>& get_params();

    private:
        /*
         * name of the constructor
         */
        std::string m_name;

        /*
         * token with source code information
         */
        token m_tok;

        /*
         * the type that this contructor creates
         */
        mir_type m_type;

        /*
         * a map of parameters names and their types
         */
        std::map<token, mir_type> m_params;
    };
}

#endif
