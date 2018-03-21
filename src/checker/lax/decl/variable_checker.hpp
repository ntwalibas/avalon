#ifndef AVALON_CHECKER_VARIABLE_CHECKER_HPP_
#define AVALON_CHECKER_VARIABLE_CHECKER_HPP_

#include <memory>
#include <string>

#include "program/ast/decl/variable.hpp"
#include "program/symtable/scope.hpp"


namespace avalon {
    class variable_checker {
    public:
        /**
         * the constructor expects nothing
         */
        variable_checker();

        /**
         * check
         * validate the variable declaration
         */
        void check(const std::shared_ptr<variable>& variable_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name);
    };
}

#endif
