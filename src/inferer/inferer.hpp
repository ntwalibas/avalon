#ifndef AVALON_INFERER_HPP_
#define AVALON_INFERER_HPP_

#include <memory>
#include <string>

#include "program/symtable/scope.hpp"
#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"


namespace avalon {
    class inferer {
    public:
        /**
         * infer
         * given an expression, this function infers the type instance of said expression and returns it
         */
        static type_instance infer(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name);

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
        static type_instance infer_literal(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope);

        /**
         * infer_tuple
         * infers the type instance of a tuple
         */
        static type_instance infer_tuple(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name);
    };
}

#endif
