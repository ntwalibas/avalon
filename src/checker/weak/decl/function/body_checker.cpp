#include <memory>
#include <string>

#include "checker/weak/decl/function/body_checker.hpp"
#include "checker/weak/stmt/block_checker.hpp"
#include "program/ast/stmt/block_stmt.hpp"
#include "program/ast/decl/function.hpp"
#include "program/symtable/scope.hpp"
#include "program/ast/program.hpp"

/* Exceptions */
#include "program/symtable/exceptions/symbol_already_declared.hpp"
#include "checker/exceptions/invalid_expression.hpp"
#include "checker/exceptions/invalid_variable.hpp"
#include "checker/exceptions/invalid_block.hpp"


namespace avalon {
    /**
     * check_body
     * this function ensures that the function's body is valid
     */
    void body_checker::check_body(std::shared_ptr<function>& function_decl, program& prog, const std::string& ns_name) {
        block_stmt& body = function_decl -> get_body();
        std::shared_ptr<scope>& l_scope = prog.get_scope();
        type_instance& ret_instance = function_decl -> get_return_type_instance();


        // checking the function is the same as checking the block statement that make the body
        block_checker checker(ret_instance);
        try {
            checker.check(body, l_scope, ns_name);
        } catch(symbol_already_declared err) {
            // if variable is declared twice inside the block (or sub-blocks)
            throw err;
        } catch(invalid_variable err) {
            // if a variable declaration fails checking inside the block
            throw err;
        } catch(invalid_block err) {
            // if the block contains anything other that variable and statement declarations
            throw err;
        } catch(invalid_expression err) {
            // if any expression sucks
            throw err;
        }        
    }
}
