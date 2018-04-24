#ifndef AVALON_INFERER_HPP_
#define AVALON_INFERER_HPP_

#include <memory>
#include <string>

#include "representer/hir/ast/expr/identifier_expression.hpp"
#include "representer/hir/ast/expr/call_expression.hpp"
#include "representer/hir/ast/expr/cast_expression.hpp"
#include "representer/hir/ast/decl/function.hpp"
#include "representer/hir/symtable/scope.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"


namespace avalon {
    class inferer {
    public:
        /**
         * infer
         * given an expression, this function infers the type instance of said expression and returns it
         */
        static type_instance infer(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name);

        /**
         * infer_default_constructor
         * infers the type instance of a default constructor expression
         */
        static type_instance infer_default_constructor(std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name);

        /**
         * infer_record_constructor
         * infers the type instance of a record constructor expression
         */
        static type_instance infer_record_constructor(std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name);

        /**
         * infer_function_call
         * infers the type instance of a function call expression
         */
        static type_instance infer_function_call(function& new_fun, std::shared_ptr<call_expression> const & call_expr, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name);

        /**
         * infer_variable
         * infers the type instance of a variable expression
         */
        static type_instance infer_variable(std::shared_ptr<identifier_expression> const & id_expr, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name);

        /**
         * infer_constructor
         * infers the type instance of an identifier constructor expression
         */
        static type_instance infer_constructor(std::shared_ptr<identifier_expression> const & id_expr, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name);

        /**
         * infer_cast
         * infers the type instance of a cast expression
         */
        static type_instance infer_cast(function& cast_fun, std::shared_ptr<cast_expression> const & cast_expr, std::shared_ptr<scope> l_scope, const std::string& ns_name);

    private:
        /**
         * the default constructor expects nothing
         */
        inferer();

        /**
         * infer_underscore
         * infers the type instance of an underscopre expression, which is just a type instance without any type
         */
        static type_instance infer_underscore(std::shared_ptr<expr>& an_expression);

        /**
         * infer_literal
         * infers the type instance of a literal
         */
        static type_instance infer_literal(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name);

        /**
         * infer_tuple
         * infers the type instance of a tuple
         */
        static type_instance infer_tuple(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name);

        /**
         * infer_list
         * infers the type instance of a list
         */
        static type_instance infer_list(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name);

        /**
         * infer_map
         * infers the type instance of a map
         */
        static type_instance infer_map(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name);

        /**
         * infer_call
         * infers the type instance of a call expression
         */
        static type_instance infer_call(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name);

        /**
         * infer_identifier
         * infers the type instance of an identifier expression
         */
        static type_instance infer_identifier(std::shared_ptr<expr> & an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name, const std::string& sub_ns_name);

        /**
         * infer_grouping
         * infers the type instance of a grouped expression
         */
        static type_instance infer_grouping(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name);
    };
}

#endif
