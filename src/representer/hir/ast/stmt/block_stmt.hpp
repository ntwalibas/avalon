#ifndef AVALON_HIR_AST_STMT_BLOCK_HPP_
#define AVALON_HIR_AST_STMT_BLOCK_HPP_

#include <memory>
#include <vector>

#include "representer/hir/ast/stmt/stmt.hpp"
#include "representer/hir/ast/decl/decl.hpp"
#include "lexer/token.hpp"


namespace avalon {
    class block_stmt : public stmt {
    public:
        /**
         * the constructor expects nothing
         */
        block_stmt();

        /**
         * copy constructor
         */
        block_stmt(const block_stmt& blc, std::shared_ptr<scope>& parent_scope);

        /**
         * add_declaration
         * add a declaration to this block
         */
        void add_declaration(std::shared_ptr<decl>& declaration);

        /**
         * get_declarations
         * return a vector of declarations that make up this block
         */
        std::vector<std::shared_ptr<decl> >& get_declarations();
        const std::vector<std::shared_ptr<decl> >& get_declarations() const;

        /**
         * is_empty
         * returns true if this block contains no declarations
         */
        bool is_empty();

        /**
         * token
         * returns this statement token
         */
        virtual const token& stmt_token() const {
            return star_tok;
        }

    private:
        /**
         * vector of declarations that form the block
         */
        std::vector<std::shared_ptr<decl> > m_declarations;
    };
}

#endif
