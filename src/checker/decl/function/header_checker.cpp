#include <utility>
#include <memory>
#include <vector>
#include <string>

#include "checker/decl/function/header_checker.hpp"
#include "checker/exceptions/invalid_function.hpp"
#include "representer/hir/ast/decl/function.hpp"
#include "checker/exceptions/invalid_type.hpp"
#include "checker/decl/type/type_checker.hpp"
#include "representer/hir/symtable/scope.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * check_header
     * this function ensures that the function's header (parameters and return type) is of the correct form
     */
    void header_checker::check_header(function& function_decl, std::shared_ptr<scope>& l_scope, const std::string& ns_name) {
        std::vector<std::pair<std::string, variable> >& params = function_decl.get_params();
        std::vector<token>& constraints = function_decl.get_constraints();

        // we make sure all the function's parameters type instances are valid
        for(auto& param : params) {
            type_instance param_type_instance = param.second.get_type_instance();            
            try {
                std::shared_ptr<type> param_instance_type = nullptr;
                std::pair<bool,bool> res = type_instance_checker::complex_check(param_type_instance, l_scope, ns_name, constraints);
                if(res.first == false)
                    param_instance_type = param_type_instance.get_type();

                // we make sure that if the function is public then all the type instances it depends are built by public types
                if(param_instance_type != nullptr && (param_instance_type -> is_public() == false && function_decl.is_public() == true)) {
                    throw invalid_function(function_decl.get_token(), "This function is public but has parameters that depend on private types. All parameters of a public function must be public as well.");
                }

                // if the type instance checked out, we replace the one on the parameter with the updated on
                param.second.set_type_instance(param_type_instance);
            } catch(invalid_type err) {
                throw err;
            }
        }

        // we make sure the function's return type instance is valid
        type_instance& return_type_instance = function_decl.get_return_type_instance();
        try {
            std::shared_ptr<type> return_instance_type = nullptr;
            std::pair<bool,bool> res = type_instance_checker::complex_check(return_type_instance, l_scope, ns_name, constraints);
            if(res.first == false)
                return_instance_type = return_type_instance.get_type();

            // we make sure that if the function is public then the return type instance must be public as well
            if(return_instance_type != nullptr && (return_instance_type -> is_public() == false && function_decl.is_public() == true)) {
                throw invalid_function(function_decl.get_token(), "This function is public but its return type depends on a private type. The return type must be public as well or the function must be private.");
            }

            // if the type instance checked out, we replace the previous return type instance
            function_decl.set_return_type_instance(return_type_instance);
        } catch(invalid_type err) {
            throw err;
        }
    }
}
