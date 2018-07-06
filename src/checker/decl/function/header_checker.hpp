#ifndef AVALON_CHECKER_DECL_FUNCTION_HEADER_CHECKER_HPP_
#define AVALON_CHECKER_DECL_FUNCTION_HEADER_CHECKER_HPP_

#include <memory>
#include <string>

#include "representer/hir/ast/decl/function.hpp"
#include "representer/hir/symtable/scope.hpp"


namespace avalon {
    class header_checker {
    public:
        /**
         * check_header
         * this function ensures that the function's header (parameters and return type) is of the correct form
         */
        static void check_header(function& function_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

        /**
         * prepare_header
         * this functions validates all type instances found in the function's header
         */
        static void prepare_header(function& function_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

    private:
        /**
         * we don't allow instatiation of this class
         */
        header_checker();
    };
}

#endif
