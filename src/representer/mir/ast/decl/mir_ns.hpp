#ifndef AVALON_MIR_AST_DECL_NAMESPACE_HPP_
#define AVALON_MIR_AST_DECL_NAMESPACE_HPP_

#include <utility>
#include <memory>
#include <vector>
#include <string>

#include "representer/mir/ast/decl/mir_decl.hpp"
#include "representer/hir/symtable/fqn.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_ns: public mir_decl {
    public:
        /**
         * the constructor expects the token with source code information
         */
        mir_ns(const std::string& name, token& tok);

        /**
         * get_name
         * returns the name of the namespace
         */
        const std::string& get_name() const;

        /**
         * get_token
         * returns a token with the namespace source information
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
         * add_hir_declaration
         * given an HIR declaration name, create the corresponding entry in the HIR vector
         */
        void add_hir_declaration(const std::string& decl_name);

        /**
         * add_mir_declaration
         * given a MIR declaration name, add it to the HIR-MIR vector
         */
        void add_mir_declaration(const std::string& decl_name, std::shared_ptr<mir_decl>& declaration);

        /**
         * get_declarations
         * for futher processing, this function is called to get all the declarations that can be found in the namespace
         */
        std::vector<std::shared_ptr<mir_decl> >& get_declarations();

        /**
         * flatten
         * transforms the MIR-MIR map into the MIR vector
         */
        void flatten();

        /*
         * is_namespace
         * returns true as this declaration is indeed a namespace
         */
        virtual bool is_namespace() {
            return true;
        }

        /**
         * is_public
         * sets and returns a boolean indicating whether this declaration can be imported
         */
        virtual bool is_public() {
            return true;
        }
        virtual void is_public(bool public_) {
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
         * the name of the namespace
         */
        std::string m_name;

        /*
         * the token with source file information about the type
         */
        token m_tok;

        /*
         * the fqn of the compilation unit where this declaration was found
         */
        fqn m_fqn;

        /*
         * a vector with declaration names as they appear in the HIR and a vector of their MIR equivalent
         * this is important because we want to preserve the order of declarations in the MIR as they appear in the HIR
         */
        std::vector<std::pair<std::string, std::vector<std::shared_ptr<mir_decl> > > > m_hir_declarations;

        /*
         * a vector of declarations found in this namespace
         */
        std::vector<std::shared_ptr<mir_decl> > m_mir_declarations;

        /*
         * a boolean that is true if HIR declarations have been flattened
         * in case it is false and the client requests MIR declarations, we flatten HIR declarations first
         * if a new HIR declaration is added, we set this flag to false even if it was true before
         */
        bool m_is_flat;
    };
}

#endif
