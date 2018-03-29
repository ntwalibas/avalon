#ifndef AVALON_CHECKER_LAX_DECL_FUNCTION_HEADER_CHECKER_HPP_
#define AVALON_CHECKER_LAX_DECL_FUNCTION_HEADER_CHECKER_HPP_

#include <memory>
#include <string>

#include "program/ast/decl/function.hpp"
#include "program/ast/program.hpp"


namespace avalon {
    class header_checker {
    public:
        /**
         * check_header
         * this function ensures that the function's header (parameters and return type) is of the correct form
         */
        static void check_header(std::shared_ptr<function>& function_decl, program& prog, const std::string& ns_name);

    private:
        /**
         * we don't allow instatiation of this class
         */
        header_checker();
    };
}

#endif
