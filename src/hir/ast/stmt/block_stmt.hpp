#ifndef AVALON_HIR_AST_STMT_BLOCK_HPP_
#define AVALON_HIR_AST_STMT_BLOCK_HPP_

#include <memory>
#include <vector>

#include "hir/ast/stmt/stmt.hpp"
#include "hir/ast/decl/decl.hpp"


namespace avalon {
    class block_stmt : public stmt {
    public:
        /**
         * the constructor expects nothing
         */
        block_stmt();

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

        /**
         * is_empty
         * returns true if this block contains no declarations
         */
        bool is_empty();

    private:
        /**
         * vector of declarations that form the block
         */
        std::vector<std::shared_ptr<decl> > m_declarations;
    };
}

#endif
