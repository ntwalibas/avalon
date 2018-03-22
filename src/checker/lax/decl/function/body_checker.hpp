#ifndef AVALON_CHECKER_LAX_DECL_FUNCTION_BODY_CHECKER_HPP_
#define AVALON_CHECKER_LAX_DECL_FUNCTION_BODY_CHECKER_HPP_

#include <memory>
#include <string>

#include "program/ast/decl/function.hpp"
#include "program/ast/program.hpp"


namespace avalon {
    class body_checker {
    public:
        /**
         * check_body
         * this function ensures that the function's body is valid
         */
        static void check_body(std::shared_ptr<function>& function_decl, program& prog, const std::string& ns_name);

    private:
        /**
         * we don't allow instatiation of this class
         */
        body_checker();
    };
}

#endif
