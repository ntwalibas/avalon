#ifndef AVALON_CHECKER_EXPR_EXPRESSION_CHECKER_HPP_
#define AVALON_CHECKER_EXPR_EXPRESSION_CHECKER_HPP_

#include <utility>
#include <memory>
#include <string>

#include "representer/hir/ast/expr/identifier_expression.hpp"
#include "representer/hir/ast/expr/binary_expression.hpp"
#include "representer/hir/ast/stmt/expression_stmt.hpp"
#include "representer/hir/ast/expr/call_expression.hpp"
#include "representer/hir/symtable/scope.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"


namespace avalon {
    enum last_binary_expression_type {
        BIN_NONE,
        BIN_NAMESPACE,
        BIN_VARIABLE,
        BIN_ATTRIBUTE,
        BIN_SUBSCRIPT,
        BIN_FUNCTION,
        BIN_CONSTRUCTOR,
    };

    class expression_checker {
    public:
        /**
         * the default constructor expects nothing.
         */
        expression_checker();

        /**
         * check
         * given an expression, this functions checks that it is a valid expression
         * and as a side effect it deduces the expression type instance and returns it.
         */
        type_instance check(std::shared_ptr<expression_stmt> const & l_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check
         * determines the kind of expression we have then calls the appropriate checker
         */
        type_instance check(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
        type_instance check(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name, const std::string& sub_ns_name);

    private:
        /**
         * check_underscore
         * returns an abstract type instance since underscore expression can never have a concrete type
         */
        type_instance check_underscore(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_literal
         * literals are built-in data (constructors)
         * this function simply returns the type instance for each type of literal
         */
        type_instance check_literal(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_tuple
         * we validate all the expressions that occur within the tuple
         */
        type_instance check_tuple(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_list
         * we validate all the expressions that occur within the list
         * in addition, it makes sure that those expressions are all of the same type instance
         */
        type_instance check_list(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_map
         * we validate all the expressions that occur within the map
         * in addition, it makes sure that those expressions are all of the same type instance
         */
        type_instance check_map(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_call
         * this function determines the kind of expression the call expression is (default constructor, record constructor or function call)
         * then dispatches the checking to the actual checker.
         */
        type_instance check_call(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_default_constructor
         * we validate the expressions that occur within the constructor.
         */
        type_instance check_default_constructor(std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_record_constructor
         * we validate the expressions that occur within the constructor.
         */
        type_instance check_record_constructor(std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_function_call
         * we validate the expressions that were passed as argument to the function
         * we also make sure that all arguments were named or none was
         */
        type_instance check_function_call(std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_identifier
         * this function determines the kind of identifier expression this is (default constructor or variable)
         * then dispatches the checking to the appropriate checker.
         */
        type_instance check_identifier(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_variable
         * we make sure that the variable declaration associated with this variable expression is valid
         */
        type_instance check_variable(std::shared_ptr<identifier_expression> const & an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_constructor
         * we make sure that the default constructor associated with this identifier expression is valid
         */
        type_instance check_constructor(std::shared_ptr<identifier_expression> const & an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_grouping
         * we make sure that the expression in the grouping is valid
         */
        type_instance check_grouping(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_cast
         * we make sure there is a function in the will allow the cast given the type instance to cast to and the type instance of the expression
         * it also makes sure the destination type instance has a type builder and the origin expression is valid
         */
        type_instance check_cast(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_unary
         * we make sure that there is a function that corresponds to the unary expression
         */
        type_instance check_unary(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_binary
         * we make sure that there is a function that corresponds to the binary expression where applicable
         */
        type_instance check_binary(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_functional_binary
         * we make sure that the binary expression has a function that corresponds to it
         */
        type_instance check_functional_binary(binary_expression_type expr_type, std::shared_ptr<binary_expression> const & binary_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_dot_binary
         * we make sure that the dot expression lval refers to either a namespace or a variable declaration
         */
        type_instance check_dot_binary(binary_expression_type expr_type, std::shared_ptr<binary_expression> const & binary_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_subscript_binary
         * we make sure that the lval is a variable declaration and that its content is accessible for reading
         */
        type_instance check_subscript_binary(binary_expression_type expr_type, std::shared_ptr<binary_expression> const & binary_expr, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
                
        type_instance check_namespace_binary(const std::string& sub_ns_name, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
        type_instance check_variable_binary(std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
        type_instance check_variable_attribute(std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
        type_instance check_tuple_attribute(std::shared_ptr<expr>& lval_val, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
        type_instance check_custom_attribute(std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
        type_instance check_variable_subscript(std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
        type_instance check_tuple_subscript(std::shared_ptr<expr>& lval_val, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
        type_instance check_list_subscript(std::shared_ptr<expr>& lval_val, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
        type_instance check_map_subscript(std::shared_ptr<expr>& lval_val, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
        type_instance check_custom_subscript(std::shared_ptr<expr>& lval, std::shared_ptr<expr>& rval, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
        
        /**
         * check_match
         * we make sure that the rval of the match expression is a literal or a constructor expression
         */
        type_instance check_match(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_assignment
         * we make sure that the expression on the right side is valid
         * and make sure that it's type is compatible with that of the variable on the left side
         */
        type_instance check_assignment(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);


        /*
         * if we are checking a match expression, it is possible that a variable expression will be provided
         * in that case, we keep this flag to inform the variable expression checker that this new variable expression is also a variable declaration.
         */
        bool m_inside_match;
    };
}

#endif
