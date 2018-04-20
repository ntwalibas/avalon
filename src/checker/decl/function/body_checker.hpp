#ifndef AVALON_CHECKER_DECL_FUNCTION_BODY_CHECKER_HPP_
#define AVALON_CHECKER_DECL_FUNCTION_BODY_CHECKER_HPP_

#include <memory>
#include <string>

#include "representer/hir/ast/decl/function.hpp"
#include "representer/hir/symtable/scope.hpp"


namespace avalon {
    class body_checker {
    public:
        /**
         * check_body
         * this function ensures that the function's body is valid
         */
        static void check_body(function& function_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

    private:
        /**
         * we don't allow instatiation of this class
         */
        body_checker();
    };
}

#endif
