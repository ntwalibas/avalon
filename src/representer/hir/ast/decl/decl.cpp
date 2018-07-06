#include <stdexcept>
#include <memory>

/* AST */
#include "representer/hir/ast/decl/statement.hpp"
#include "representer/hir/ast/decl/variable.hpp"
#include "representer/hir/ast/decl/decl.hpp"

/* Symtable */
#include "representer/hir/symtable/scope.hpp"


namespace avalon {
    /**
     * copy
     * copies a declaration and returns the copied one
     */
    std::shared_ptr<decl> decl::copy(std::shared_ptr<scope>& parent_scope) {
        if(this == nullptr)
            return nullptr;

        if(this -> is_variable()) {
            std::shared_ptr<variable> const & var_decl = std::static_pointer_cast<variable>(shared_from_this());
            std::shared_ptr<variable> l_var_decl = std::make_shared<variable>(var_decl);
            std::shared_ptr<decl> new_declaration = l_var_decl;
            return new_declaration;
        }
        else if(this -> is_statement()) {
            std::shared_ptr<statement> const & stmt_decl = std::static_pointer_cast<statement>(shared_from_this());
            std::shared_ptr<statement> l_stmt_decl = std::make_shared<statement>(stmt_decl, parent_scope);
            std::shared_ptr<decl> new_declaration = l_stmt_decl;
            return new_declaration;
        }
        else {
            throw std::runtime_error("[compiler error] attempting to copy a declaration that's neither variable or a statement declaration.");
        }
    }
}
