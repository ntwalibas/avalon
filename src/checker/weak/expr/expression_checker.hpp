#ifndef AVALON_CHECKER_LAX_EXPR_EXPRESSION_CHECKER_HPP_
#define AVALON_CHECKER_LAX_EXPR_EXPRESSION_CHECKER_HPP_

#include <memory>
#include <string>

#include "program/ast/stmt/expression_stmt.hpp"
#include "program/symtable/scope.hpp"
#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"


namespace avalon {
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
        type_instance check(std::shared_ptr<expression_stmt>& l_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

    private:
        /**
         * check
         * determines the kind of expression we have then calls the appropriate checker
         */
        type_instance check(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

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
        type_instance check_default_constructor(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_record_constructor
         * we validate the expressions that occur within the constructor.
         */
        type_instance check_record_constructor(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_function_call
         * we validate the expressions that were passed as argument to the function
         * we also make sure that all arguments were named or none was
         */
        type_instance check_function_call(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_list_constructor
         * we validate the expressions that make up the list
         */
        type_instance check_list_constructor(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_map_constructor
         * we validate the expressions that make up the map
         */
        type_instance check_map_constructor(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_identifier
         * this function determines the kind of identifier expression this is (default constructor or variable)
         * then dispatches the checking to the appropriate checker.
         */
        type_instance check_identifier(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_variable
         * we make sure that the variable declaration associated with this variable expression has a type instance set.
         */
        type_instance check_variable(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

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
         * we make sure that there is a function that corresponds to the binary expression
         */
        type_instance check_binary(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * check_assignment
         * we make sure that the expression on the right side is valid
         * and make sure that it's type is compatible with that of the variable on the left side
         */
        type_instance check_assignment(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
    };
}

#endif
