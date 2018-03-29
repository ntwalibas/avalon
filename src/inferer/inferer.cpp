#include <stdexcept>
#include <memory>
#include <string>

#include "program/symtable/scope.hpp"
#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "inferer/inferer.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * infer
     * given an expression, this function infers the type instance of said expression and returns it
     */
    type_instance inferer::infer(std::shared_ptr<expr>& an_expression, std::shared_ptr<scope> l_scope, const std::string& ns_name) {
        if(an_expression -> is_underscore_expression())
            return inferer::infer_underscore(an_expression);

        else
            throw std::runtime_error("[compiler error] unexpected expression type in inference engine.");
    }

    /**
     * infer_underscore
     * infers the type instance of an underscopre expression, which is just a type instance without any type
     */
    type_instance inferer::infer_underscore(std::shared_ptr<expr>& an_expression) {
        type_instance instance(star_tok, "*");
        return instance;
    }
}
