#include <memory>
#include <vector>

#include "representer/mir/ast/stmt/mir_block_stmt.hpp"
#include "representer/mir/ast/stmt/mir_stmt.hpp"
#include "representer/mir/ast/decl/mir_decl.hpp"


namespace avalon {
    /**
     * the constructor expects nothing
     */
    mir_block_stmt::mir_block_stmt() {        
    }

    /**
     * add_declaration
     * add a declaration to this block
     */
    void mir_block_stmt::add_declaration(std::shared_ptr<mir_decl>& declaration) {
        m_declarations.push_back(declaration);
    }

    /**
     * get_declarations
     * return a vector of declarations that make up this block
     */
    std::vector<std::shared_ptr<mir_decl> >& mir_block_stmt::get_declarations() {
        return m_declarations;
    }

    /**
     * is_empty
     * returns true if this block contains no declarations
     */
    bool mir_block_stmt::is_empty() {
        return m_declarations.empty();
    }
}
