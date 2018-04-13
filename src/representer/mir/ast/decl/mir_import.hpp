#ifndef AVALON_MIR_AST_IMPORT_HPP_
#define AVALON_MIR_AST_IMPORT_HPP_

#include <string>

#include "representer/mir/ast/decl/mir_decl.hpp"
#include "representer/hir/symtable/fqn.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class mir_import : public mir_decl {
    public:
        /**
         * the constructor expects the import token and the fqn to import declaration from
         */
        mir_import(token& tok, const std::string& fqn_name);

        /**
         * get_token
         * returns a token with import source information
         */
        const token& get_token() const;

        /**
         * get_name
         * returns the fully qualified name of the module to import
         */
        const std::string& get_fqn_name();

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

        /*
         * is_import
         * returns true as this declaration is indeed an import
         */
        virtual bool is_import() {
            return true;
        }

        /**
         * is_public
         * sets and returns a boolean indicating whether this declaration can be imported
         */
        virtual bool is_public() {
            return false;
        }
        virtual void is_public(bool public_) {
        }

        /**
         * is_reachable
         * sets and returns a boolean indicating whether this import declaration will be executed
         */
        virtual bool is_reachable() {
            return true;
        }
        virtual void is_reachable(bool reachable) {
        }
        
        /**
         * sets and returns a boolean that indicates whether this import declaration will cause a function to return
         */
        virtual bool terminates() {
            return false;
        }
        virtual void terminates(bool terminates) {
        }

        /**
         * passes
         * sets and returns a boolean indicating whether this import declaration allows the next declaration to be exeuted
         */
        virtual bool passes() {
            return true;
        }
        virtual void passes(bool passes) {            
        }

    private:
        /*
         * the token with source file information about the import
         */
        token m_tok;

        /*
         * the string representation of the fqn to import declarations from
         */
        std::string m_fqn_name;

        /*
         * the fqn of the compilation unit where this declaration was found
         */
        fqn m_fqn;
    };
}

#endif
