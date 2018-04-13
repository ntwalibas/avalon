#ifndef AVALON_HIR_AST_DECL_NAMESPACE_HPP_
#define AVALON_HIR_AST_DECL_NAMESPACE_HPP_

#include <memory>
#include <vector>

#include "representer/hir/ast/decl/decl.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class ns: public decl {
    public:
        /**
         * the constructor expects the token with source code information
         */
        ns(token tok);

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
         * add_declaration
         * a namespace can contain the following declarations:
         * - type declarations
         * - function declarations
         * - variable declarations
         */
        void add_declaration(std::shared_ptr<decl>& declaration);

        /**
         * get_declarations
         * for futher processing, this function is called to get all the declarations that can be found in the namespace
         */
        std::vector<std::shared_ptr<decl> >& get_declarations();

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
         * a vector of declarations found in this namespace
         */
        std::vector<std::shared_ptr<decl> > m_declarations;
    };
}

#endif
