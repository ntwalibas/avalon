#include <memory>
#include <vector>
#include <string>

#include "checker/lax/decl/function/body_checker.hpp"
#include "checker/exceptions/invalid_function.hpp"
#include "program/ast/stmt/block_stmt.hpp"
#include "program/ast/decl/function.hpp"
#include "program/ast/decl/decl.hpp"
#include "program/ast/program.hpp"


namespace avalon {
    /**
     * check_body
     * this function ensures that the function's body is valid
     */
    void body_checker::check_body(std::shared_ptr<function>& function_decl, program& prog, const std::string& ns_name) {
        block_stmt& body = function_decl -> get_body();
        std::vector<std::shared_ptr<decl> >& declarations = body.get_declarations();

        // make sure we only have variable and statement declarations in the body
        for(auto& declaration : declarations) {
            if(declaration -> is_variable() == false and declaration -> is_statement() == false)
                throw invalid_function(function_decl -> get_token(), "A function can only contain variable declarations and statement declarations.");
        }

        // make sure the function's body is valid


        // make sure each return statement returns an object whose type instance is the same as the function's return type instance
        
    }
}
