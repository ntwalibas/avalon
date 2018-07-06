#include <stdexcept>
#include <memory>

/* AST */
#include "representer/hir/ast/stmt/expression_stmt.hpp"
#include "representer/hir/ast/stmt/continue_stmt.hpp"
#include "representer/hir/ast/stmt/return_stmt.hpp"
#include "representer/hir/ast/stmt/block_stmt.hpp"
#include "representer/hir/ast/stmt/while_stmt.hpp"
#include "representer/hir/ast/stmt/break_stmt.hpp"
#include "representer/hir/ast/stmt/pass_stmt.hpp"
#include "representer/hir/ast/stmt/if_stmt.hpp"
#include "representer/hir/ast/stmt/stmt.hpp"

/* Symtable */
#include "representer/hir/symtable/scope.hpp"


namespace avalon {
    /**
     * copy
     * copies a declaration and returns the copied one
     */
    std::shared_ptr<stmt> stmt::copy(std::shared_ptr<scope>& parent_scope) {
        if(this == nullptr)
            return nullptr;

        if(this -> is_while()) {
            std::shared_ptr<while_stmt> const & wh_stmt = std::static_pointer_cast<while_stmt>(shared_from_this());
            std::shared_ptr<while_stmt> l_wh_stmt = std::make_shared<while_stmt>(wh_stmt, parent_scope);
            std::shared_ptr<stmt> new_statement = l_wh_stmt;
            return new_statement;
        }
        else if(this -> is_if()) {
            std::shared_ptr<if_stmt> const & f_stmt = std::static_pointer_cast<if_stmt>(shared_from_this());
            std::shared_ptr<if_stmt> l_f_stmt = std::make_shared<if_stmt>(f_stmt, parent_scope);
            std::shared_ptr<stmt> new_statement = l_f_stmt;
            return new_statement;
        }
        else if(this -> is_break()) {
            std::shared_ptr<break_stmt> const & br_stmt = std::static_pointer_cast<break_stmt>(shared_from_this());
            std::shared_ptr<break_stmt> l_br_stmt = std::make_shared<break_stmt>(br_stmt);
            std::shared_ptr<stmt> new_statement = l_br_stmt;
            return new_statement;
        }
        else if(this -> is_continue()) {
            std::shared_ptr<continue_stmt> const & cont_stmt = std::static_pointer_cast<continue_stmt>(shared_from_this());
            std::shared_ptr<continue_stmt> l_cont_stmt = std::make_shared<continue_stmt>(cont_stmt);
            std::shared_ptr<stmt> new_statement = l_cont_stmt;
            return new_statement;
        }
        else if(this -> is_pass()) {
            std::shared_ptr<pass_stmt> const & pas_stmt = std::static_pointer_cast<pass_stmt>(shared_from_this());
            std::shared_ptr<pass_stmt> l_pas_stmt = std::make_shared<pass_stmt>(pas_stmt);
            std::shared_ptr<stmt> new_statement = l_pas_stmt;
            return new_statement;
        }
        else if(this -> is_return()) {
            std::shared_ptr<return_stmt> const & ret_stmt = std::static_pointer_cast<return_stmt>(shared_from_this());
            std::shared_ptr<return_stmt> l_ret_stmt = std::make_shared<return_stmt>(ret_stmt);
            std::shared_ptr<stmt> new_statement = l_ret_stmt;
            return new_statement;
        }
        else if(this -> is_expression()) {
            std::shared_ptr<expression_stmt> const & expr_stmt = std::static_pointer_cast<expression_stmt>(shared_from_this());
            std::shared_ptr<expression_stmt> l_expr_stmt = std::make_shared<expression_stmt>(expr_stmt);
            std::shared_ptr<stmt> new_statement = l_expr_stmt;
            return new_statement;
        }
        else {
            throw std::runtime_error("[compiler error] attempting to copy an unsupported statement type.");
        }
    }
}
