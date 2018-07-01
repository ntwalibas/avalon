#include <memory>
#include <string>

#include "representer/hir/ast/stmt/block_stmt.hpp"
#include "checker/decl/function/body_checker.hpp"
#include "representer/hir/ast/decl/function.hpp"
#include "representer/hir/symtable/scope.hpp"
#include "checker/stmt/block_checker.hpp"

/* Exceptions */
#include "checker/exceptions/invalid_expression.hpp"
#include "checker/exceptions/invalid_statement.hpp"
#include "checker/exceptions/invalid_variable.hpp"
#include "checker/exceptions/invalid_block.hpp"


namespace avalon {
    /**
     * check_body
     * this function ensures that the function's body is valid
     */
    void body_checker::check_body(function& function_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        block_stmt& body = function_decl.get_body();
        type_instance& ret_instance = function_decl.get_return_type_instance();

        // checking the function is the same as checking the block statement that make the body
        block_checker checker(ret_instance);
        try {
            checker.check(body, l_scope, ns_name);
        } catch(invalid_variable err) {
            // if a variable declaration fails checking inside the block
            throw err;
        } catch(invalid_block err) {
            // if the block contains anything other that variable and statement declarations
            throw err;
        } catch(invalid_expression err) {
            // if any expression sucks
            throw err;
        } catch(invalid_statement err) {
            throw err;
        }
    }
}
