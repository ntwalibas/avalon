#include <stdexcept>
#include <memory>

/* AST */
#include "representer/hir/ast/expr/assignment_expression.hpp"
#include "representer/hir/ast/expr/underscore_expression.hpp"
#include "representer/hir/ast/expr/identifier_expression.hpp"
#include "representer/hir/ast/expr/grouped_expression.hpp"
#include "representer/hir/ast/expr/literal_expression.hpp"
#include "representer/hir/ast/expr/binary_expression.hpp"
#include "representer/hir/ast/expr/unary_expression.hpp"
#include "representer/hir/ast/expr/match_expression.hpp"
#include "representer/hir/ast/expr/tuple_expression.hpp"
#include "representer/hir/ast/expr/list_expression.hpp"
#include "representer/hir/ast/expr/call_expression.hpp"
#include "representer/hir/ast/expr/cast_expression.hpp"
#include "representer/hir/ast/expr/map_expression.hpp"
#include "representer/hir/ast/expr/expr.hpp"


namespace avalon {
    /**
     * copy
     * creates a copy of an expression and returns it.
     * ideally, this should be the assignment copy operator but I suck at C++ so please educate me when you see this if you inclined to do so.
     */
    std::shared_ptr<expr> expr::copy() {
        if(this == nullptr)
            return nullptr;

        if(this -> is_underscore_expression()) {
            std::shared_ptr<underscore_expression> const & under_expr = std::static_pointer_cast<underscore_expression>(shared_from_this());
            std::shared_ptr<underscore_expression> l_under_expr = std::make_shared<underscore_expression>(under_expr);
            std::shared_ptr<expr> new_expression = l_under_expr;
            return new_expression;
        }
        else if(this -> is_literal_expression()) {
            std::shared_ptr<literal_expression> const & lit_expr = std::static_pointer_cast<literal_expression>(shared_from_this());
            std::shared_ptr<literal_expression> l_lit_expr = std::make_shared<literal_expression>(lit_expr);
            std::shared_ptr<expr> new_expression = l_lit_expr;
            return new_expression;
        }
        else if(this -> is_grouped_expression()) {
            std::shared_ptr<grouped_expression> const & gr_expr = std::static_pointer_cast<grouped_expression>(shared_from_this());
            std::shared_ptr<grouped_expression> l_gr_expr = std::make_shared<grouped_expression>(gr_expr);
            std::shared_ptr<expr> new_expression = l_gr_expr;
            return new_expression;
        }
        else if(this -> is_tuple_expression()) {
            std::shared_ptr<tuple_expression> const & tup_expr = std::static_pointer_cast<tuple_expression>(shared_from_this());
            std::shared_ptr<tuple_expression> l_tup_expr = std::make_shared<tuple_expression>(tup_expr);
            std::shared_ptr<expr> new_expression = l_tup_expr;
            return new_expression;
        }
        else if(this -> is_list_expression()) {
            std::shared_ptr<list_expression> const & list_expr = std::static_pointer_cast<list_expression>(shared_from_this());
            std::shared_ptr<list_expression> l_list_expr = std::make_shared<list_expression>(list_expr);
            std::shared_ptr<expr> new_expression = l_list_expr;
            return new_expression;
        }
        else if(this -> is_map_expression()) {
            std::shared_ptr<map_expression> const & map_expr = std::static_pointer_cast<map_expression>(shared_from_this());
            std::shared_ptr<map_expression> l_map_expr = std::make_shared<map_expression>(map_expr);
            std::shared_ptr<expr> new_expression = l_map_expr;
            return new_expression;
        }
        else if(this -> is_identifier_expression()) {
            std::shared_ptr<identifier_expression> const & id_expr = std::static_pointer_cast<identifier_expression>(shared_from_this());
            std::shared_ptr<identifier_expression> l_id_expr = std::make_shared<identifier_expression>(id_expr);
            std::shared_ptr<expr> new_expression = l_id_expr;
            return new_expression;
        }
        else if(this -> is_call_expression()) {
            std::shared_ptr<call_expression> const & call_expr = std::static_pointer_cast<call_expression>(shared_from_this());
            std::shared_ptr<call_expression> l_call_expr = std::make_shared<call_expression>(call_expr);
            std::shared_ptr<expr> new_expression = l_call_expr;
            return new_expression;
        }
        else if(this -> is_cast_expression()) {
            std::shared_ptr<cast_expression> const & cast_expr = std::static_pointer_cast<cast_expression>(shared_from_this());
            std::shared_ptr<cast_expression> l_cast_expr = std::make_shared<cast_expression>(cast_expr);
            std::shared_ptr<expr> new_expression = l_cast_expr;
            return new_expression;
        }
        else if(this -> is_unary_expression()) {
            std::shared_ptr<unary_expression> const & un_expr = std::static_pointer_cast<unary_expression>(shared_from_this());
            std::shared_ptr<unary_expression> l_un_expr = std::make_shared<unary_expression>(un_expr);
            std::shared_ptr<expr> new_expression = l_un_expr;
            return new_expression;
        }
        else if(this -> is_binary_expression()) {
            std::shared_ptr<binary_expression> const & bin_expr = std::static_pointer_cast<binary_expression>(shared_from_this());
            std::shared_ptr<binary_expression> l_bin_expr = std::make_shared<binary_expression>(bin_expr);
            std::shared_ptr<expr> new_expression = l_bin_expr;
            return new_expression;
        }
        else if(this -> is_match_expression()) {
            std::shared_ptr<match_expression> const & match_expr = std::static_pointer_cast<match_expression>(shared_from_this());
            std::shared_ptr<match_expression> l_match_expr = std::make_shared<match_expression>(match_expr);
            std::shared_ptr<expr> new_expression = l_match_expr;
            return new_expression;
        }
        else if(this -> is_assignment_expression()) {
            std::shared_ptr<assignment_expression> const & assign_expr = std::static_pointer_cast<assignment_expression>(shared_from_this());
            std::shared_ptr<assignment_expression> l_assign_expr = std::make_shared<assignment_expression>(assign_expr);
            std::shared_ptr<expr> new_expression = l_assign_expr;
            return new_expression;
        }
        else {
            throw std::runtime_error("[compiler error] unexpected expression.");
        }
    }
}
