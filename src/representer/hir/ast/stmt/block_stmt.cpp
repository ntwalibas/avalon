#include <memory>
#include <vector>

#include "representer/hir/ast/stmt/block_stmt.hpp"
#include "representer/hir/ast/decl/decl.hpp"


namespace avalon {
    /**
     * the constructor expects nothing
     */
    block_stmt::block_stmt() {        
    }

    /**
     * copy constructor
     */
    block_stmt::block_stmt(const block_stmt& blc, std::shared_ptr<scope>& parent_scope) {
        const std::vector<std::shared_ptr<decl> >& declarations = blc.get_declarations();
        for(const auto& declaration : declarations)
            m_declarations.push_back(declaration -> copy(parent_scope));
    }

    /**
     * add_declaration
     * add a declaration to this block
     */
    void block_stmt::add_declaration(std::shared_ptr<decl>& declaration) {
        m_declarations.push_back(declaration);
    }

    /**
     * get_declarations
     * return a vector of declarations that make up this block
     */
    std::vector<std::shared_ptr<decl> >& block_stmt::get_declarations() {
        return m_declarations;
    }

    const std::vector<std::shared_ptr<decl> >& block_stmt::get_declarations() const {
        return m_declarations;
    }

    /**
     * is_empty
     * returns true if this block contains no declarations
     */
    bool block_stmt::is_empty() {
        return m_declarations.empty();
    }
}
