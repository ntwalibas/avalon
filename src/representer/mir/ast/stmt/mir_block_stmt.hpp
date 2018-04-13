#ifndef AVALON_MIR_AST_STMT_BLOCK_HPP_
#define AVALON_MIR_AST_STMT_BLOCK_HPP_

#include <memory>
#include <vector>

#include "representer/mir/ast/stmt/mir_stmt.hpp"
#include "representer/mir/ast/decl/mir_decl.hpp"


namespace avalon {
    class mir_block_stmt : public mir_stmt {
    public:
        /**
         * the constructor expects nothing
         */
        mir_block_stmt();

        /**
         * add_declaration
         * add a declaration to this block
         */
        void add_declaration(std::shared_ptr<mir_decl>& declaration);

        /**
         * get_declarations
         * return a vector of declarations that make up this block
         */
        std::vector<std::shared_ptr<mir_decl> >& get_declarations();

        /**
         * is_empty
         * returns true if this block contains no declarations
         */
        bool is_empty();

    private:
        /**
         * vector of declarations that form the block
         */
        std::vector<std::shared_ptr<mir_decl> > m_declarations;
    };
}

#endif
