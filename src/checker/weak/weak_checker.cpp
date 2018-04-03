#include <stdexcept>
#include <memory>
#include <string>
#include <vector>
#include <queue>

/* Error display */
#include "checker/check_error.hpp"

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "program/ast/decl/variable.hpp"
#include "program/ast/decl/function.hpp"
#include "program/ast/decl/import.hpp"
#include "program/ast/decl/decl.hpp"
#include "program/ast/decl/type.hpp"
#include "program/ast/decl/ns.hpp"
#include "program/ast/program.hpp"
/* Symbol table */
#include "program/symtable/exceptions/symbol_already_declared.hpp"
#include "program/symtable/gtable.hpp"
#include "program/symtable/scope.hpp"

/* Checker */
#include "checker/weak/decl/function/function_checker.hpp"
#include "checker/weak/decl/variable/variable_checker.hpp"
#include "checker/weak/decl/import/import_checker.hpp"
#include "checker/exceptions/invalid_variable.hpp"
#include "checker/exceptions/invalid_function.hpp"
#include "checker/weak/decl/type/type_checker.hpp"
#include "checker/exceptions/invalid_import.hpp"
#include "checker/exceptions/invalid_type.hpp"
#include "checker/weak/weak_checker.hpp"


namespace avalon {
    /**
     * the constructor expects:
     * - the global symbol table with programs to check
     * - the path to the source containing the program we are validating
     * - the error handler to use in order to display errors
     */
    weak_checker::weak_checker(gtable& gtab, const std::string& source_path, error& error_handler) : m_source_path(source_path), m_error_handler(error_handler), m_gtable(gtab) {
        create_declarations();        
    }

    /**
     * check
     * the checker entry point
     * it validates that all programs found in the gtable are correct.
     * returns the program that may contan the __main__ function.
     */
    program weak_checker::check() {
        std::queue<std::string> checking_order = m_gtable.get_checking_order();
        while(!checking_order.empty()) {
            const std::string& fqn_name = checking_order.front();
            program& prog = m_gtable.get_program(fqn_name);
            check_program(prog);
            checking_order.pop();
            // if the queue is empty, then we got the program that sits on top with the last front()
            if(checking_order.empty())
                m_prog = prog;
        }

        return m_prog;
    }

    /**
     * check_program
     * given a program, check that all the declarations within it are correct
     */
    void weak_checker::check_program(program& prog) {
        std::vector<std::shared_ptr<decl> >& declarations = prog.get_declarations();
        for(auto& declaration : declarations)
            check_declaration(declaration, prog);
    }

    /**
     * check_declaration
     * given a declaration, determine which type it is and dispatch the checking to the appropriate function
     */
    void weak_checker::check_declaration(std::shared_ptr<decl>& declaration, program& prog) {
        if(declaration -> is_import()) {
            check_import(declaration, prog);
        }
        else if(declaration -> is_namespace()) {
            check_namespace(declaration, prog);
        }
        else {
            throw std::runtime_error("[compiler error] Unexpected declaration while checking program. A program must contain only import and namespace declarations. All other declarations go into namespace declarations.");
        }
    }

    /**
     * check_import
     * given a declaration, check if it is a valid import declaration
     */
    void weak_checker::check_import(std::shared_ptr<decl>& declaration, program& prog) {
        const std::shared_ptr<import>& import_decl = std::static_pointer_cast<import>(declaration);
        import_checker i_checker;
        
        // check that all imported declarations exist
        try {
            i_checker.check(import_decl, m_gtable);
        } catch(invalid_import err) {
            throw checking_error(true, err.get_token(), err.what());
        }

        // run side effects: we import declarations from the import fqn into the current program we are checking
        program& imported_prog = m_gtable.get_program(import_decl -> get_fqn_name());
        import_declarations(imported_prog, prog);
    }

    /**
     * check_namespace
     * given a declaration, check if it is a valid namespace
     */
    void weak_checker::check_namespace(std::shared_ptr<decl>& declaration, program& prog) {
        std::shared_ptr<ns> namespace_decl = std::static_pointer_cast<ns>(declaration);
        std::shared_ptr<scope>& l_scope = prog.get_scope();

        // first, we "import" built-in types in the current program
        for(auto& bit : m_bits) {
            import_type(bit, l_scope, "*");
        }

        // then we "import" built-in functions in the current program
        for(auto& bif : m_bifs) {
            import_function(bif, l_scope, "*");
        }

        // we add the namespace to the scope
        l_scope -> add_namespace(namespace_decl -> get_name());

        // we validate all declarations inside the namespace
        std::vector<std::shared_ptr<decl> >& declarations = namespace_decl -> get_declarations();
        for(auto& declaration : declarations) {            
            if(declaration -> is_statement() == false) {
                check_top_declaration(declaration, prog, namespace_decl -> get_name());
            }
            // we don't allow statement declarations directly inside namespaces
            else {
                throw checking_error(false, namespace_decl -> get_token(), "Statement declarations are not allowed inside namespace declarations.");
            }
        }
    }

    /**
     * check_top_declaration
     * given a top declaration (type, variable, function or statement), find which is it and dispatch the checking to the appropriate function
     */
    void weak_checker::check_top_declaration(std::shared_ptr<decl>& declaration, program& prog, const std::string& namespace_name) {
        if(declaration -> is_type()) {
            check_type(declaration, prog, namespace_name);
        }
        else if(declaration -> is_function()) {
            check_function(declaration, prog, namespace_name);
        }
        else if(declaration -> is_variable()) {
            check_variable(declaration, prog, namespace_name);
        }
    }

    /**
     * check_type
     * given a declaration, check if it is a valid type
     */
    void weak_checker::check_type(std::shared_ptr<decl>& declaration, program& prog, const std::string& namespace_name) {
        std::shared_ptr<type> type_decl = std::static_pointer_cast<type>(declaration);
        type_checker t_checker;

        // check the type
        try {
            t_checker.check(type_decl, prog, namespace_name);
        } catch(invalid_type err) {
            throw checking_error(true, err.get_token(), err.what());
        }

        // run side effects: add the type declaration to the program scope
        std::shared_ptr<scope>& l_scope = prog.get_scope();
        import_type(type_decl, l_scope, namespace_name);
    }

    /**
     * check_function
     * given a declaration, check if it is a valid function
     */
    void weak_checker::check_function(std::shared_ptr<decl>& declaration, program& prog, const std::string& namespace_name) {
        std::shared_ptr<function> function_decl = std::static_pointer_cast<function>(declaration);
        function_checker f_checker;

        // check the function
        try {
            f_checker.check(function_decl, prog, namespace_name);
        } catch(invalid_function err) {
            throw checking_error(true, err.get_token(), err.what());
        }

        // run side effects: add the function declaration to the program scope
        std::shared_ptr<scope>& l_scope = prog.get_scope();
        import_function(function_decl, l_scope, namespace_name);
    }

    /**
     * check_variable
     * given a declaration, check if it is a valid variable
     */
    void weak_checker::check_variable(std::shared_ptr<decl>& declaration, program& prog, const std::string& namespace_name) {
        std::shared_ptr<variable> variable_decl = std::static_pointer_cast<variable>(declaration);
        std::shared_ptr<scope>& l_scope = prog.get_scope();
        variable_checker v_checker;

        // check the variable
        try {
            v_checker.check(variable_decl, l_scope, namespace_name);
        } catch(invalid_variable err) {
            throw checking_error(true, err.get_token(), err.what());
        }

        // run side effects: add the variable declaration to the program scope
        import_variable(variable_decl, l_scope, namespace_name);
    }

    /**
     * importer
     * Given two programs, import all the declarations in "from" scope into "to" scope
     */
    void weak_checker::import_declarations(program& from, program& to) {
        // declarations defined in from with hope of find namespaces with declarations in them
        std::vector<std::shared_ptr<decl> >& declarations = from.get_declarations();
        std::vector<std::shared_ptr<ns> > namespace_decls;
        // the scope into which to insert the found declarations
        std::shared_ptr<scope>& to_scope = to.get_scope();


        // we get all the namespaces defined in the "from" program        
        for(auto& declaration : declarations) {
            if(declaration -> is_namespace()) {
                std::shared_ptr<ns> namespace_decl = std::static_pointer_cast<ns>(declaration);
                namespace_decls.push_back(namespace_decl);
                // we add all namespaces to the "to" scope
                to_scope -> add_namespace(namespace_decl -> get_name());
            }
        }

        // go over the namespaces and insert the public declarations found in them into the "to" scope
        for(auto& namespace_decl : namespace_decls) {
            std::vector<std::shared_ptr<decl> >& l_declarations = namespace_decl -> get_declarations();
            for(auto& l_declaration : l_declarations) {
                if(l_declaration -> is_type()) {
                    std::shared_ptr<type> type_decl = std::static_pointer_cast<type>(l_declaration);
                    if(type_decl -> is_public()) {
                        import_type(type_decl, to_scope, namespace_decl -> get_name());
                    }
                }
                else if(l_declaration -> is_function()) {
                    std::shared_ptr<function> function_decl = std::static_pointer_cast<function>(l_declaration);
                    if(function_decl -> is_public()) {
                        import_function(function_decl, to_scope, namespace_decl -> get_name());
                    }
                }
                else if(l_declaration -> is_variable()) {
                    std::shared_ptr<variable> variable_decl = std::static_pointer_cast<variable>(l_declaration);
                    if(variable_decl -> is_public()) {
                        import_variable(variable_decl, to_scope, namespace_decl -> get_name());
                    }
                }
            }
        }
    }

    /**
     * import_type
     * Given a namespace name and a type declaration, insert the type into the given scope
     */
    void weak_checker::import_type(std::shared_ptr<type>& type_decl, std::shared_ptr<scope>& scp, const std::string& namespace_name) {
        try {
            scp -> add_type(namespace_name, type_decl);
        } catch(symbol_already_declared err) {
            throw checking_error(true, type_decl -> get_token(), err.what());
        }
    }

    /**
     * import_function
     * Given a namespace name and a function declaration, insert the function into the given scope
     */
    void weak_checker::import_function(std::shared_ptr<function>& function_decl, std::shared_ptr<scope>& scp, const std::string& namespace_name) {
        try {
            scp -> add_function(namespace_name, function_decl);
        } catch(symbol_already_declared err) {
            throw checking_error(true, function_decl -> get_token(), err.what());
        }
    }

    /**
     * import_variable
     * Given a namespace name and a variable declaration, insert the variable into the given scope
     */
    void weak_checker::import_variable(std::shared_ptr<variable>& variable_decl, std::shared_ptr<scope>& scp, const std::string& namespace_name) {
        try {
            scp -> add_variable(namespace_name, variable_decl);
        } catch(symbol_already_declared err) {
            throw checking_error(true, variable_decl -> get_token(), err.what());
        }
    }

    /**
     * checking_error
     * contructs and returns an check_error exception
     */
    check_error weak_checker::checking_error(bool fatal, const token& tok, const std::string& message) {
        return check_error(m_error_handler, tok, fatal, message);
    }

    /**
     * create_declarations
     * this functions creates built-in declarations to be used during this checking
     */
    void weak_checker::create_declarations() {
        /** Built-in types **/
        // void type
        std::shared_ptr<type> void_type = std::make_shared<type>(void_type_tok, VALID);
        m_bits.push_back(void_type);

        // unit type
        std::shared_ptr<type> unit_type = std::make_shared<type>(unit_type_tok, VALID);
        default_constructor unit_cons(unit_cons_tok, unit_type);
        m_bits.push_back(unit_type);

        // bool type
        std::shared_ptr<type> bool_type = std::make_shared<type>(bool_type_tok, VALID);
        default_constructor true_cons(true_cons_tok, bool_type);
        default_constructor false_cons(false_cons_tok, bool_type);
        m_bits.push_back(bool_type);

        // integer type
        std::shared_ptr<type> int_type = std::make_shared<type>(int_type_tok, VALID);
        m_bits.push_back(int_type);

        // decimal type
        std::shared_ptr<type> dec_type = std::make_shared<type>(dec_type_tok, VALID);
        m_bits.push_back(dec_type);

        // floating point type
        std::shared_ptr<type> float_type = std::make_shared<type>(float_type_tok, VALID);
        m_bits.push_back(float_type);

        // string type
        std::shared_ptr<type> string_type = std::make_shared<type>(string_type_tok, VALID);
        m_bits.push_back(string_type);



        /** Built-in functions **/
        /* functions for the bool type */
        type_instance bool_instance(bool_type_tok, bool_type, "*");
        // variables
        variable bool_param(var_one_tok, false);
        bool_param.set_type_instance(bool_instance);

        // boolean and
        std::shared_ptr<function> bool_and_function = std::make_shared<function>(and_function_tok);
        bool_and_function -> add_param(bool_param);
        bool_and_function -> add_param(bool_param);
        bool_and_function -> set_return_type_instance(bool_instance);
        m_bifs.push_back(bool_and_function);

        // boolean or
        std::shared_ptr<function> bool_or_function = std::make_shared<function>(or_function_tok);
        bool_or_function -> add_param(bool_param);
        bool_or_function -> add_param(bool_param);
        bool_or_function -> set_return_type_instance(bool_instance);
        m_bifs.push_back(bool_or_function);

        // boolean not
        std::shared_ptr<function> bool_not_function = std::make_shared<function>(not_function_tok);
        bool_not_function -> add_param(bool_param);
        bool_not_function -> set_return_type_instance(bool_instance);
        m_bifs.push_back(bool_not_function);


        /* functions for the integer type */
        type_instance int_instance(int_type_tok, int_type, "*");
        // variables
        variable int_param(var_one_tok, false);
        int_param.set_type_instance(int_instance);

        // unary positive
        std::shared_ptr<function> int_pos_function = std::make_shared<function>(pos_function_tok);
        int_pos_function -> add_param(int_param);
        int_pos_function -> set_return_type_instance(int_instance);
        m_bifs.push_back(int_pos_function);

        // unary negative
        std::shared_ptr<function> int_neg_function = std::make_shared<function>(neg_function_tok);
        int_neg_function -> add_param(int_param);
        int_neg_function -> set_return_type_instance(int_instance);
        m_bifs.push_back(int_neg_function);

        // integer addition
        std::shared_ptr<function> int_add_function = std::make_shared<function>(add_function_tok);
        int_add_function -> add_param(int_param);
        int_add_function -> add_param(int_param);
        int_add_function -> set_return_type_instance(int_instance);
        m_bifs.push_back(int_add_function);

        // integer substraction
        std::shared_ptr<function> int_sub_function = std::make_shared<function>(sub_function_tok);
        int_sub_function -> add_param(int_param);
        int_sub_function -> add_param(int_param);
        int_sub_function -> set_return_type_instance(int_instance);
        m_bifs.push_back(int_sub_function);

        // integer multiplication
        std::shared_ptr<function> int_mul_function = std::make_shared<function>(mul_function_tok);
        int_mul_function -> add_param(int_param);
        int_mul_function -> add_param(int_param);
        int_mul_function -> set_return_type_instance(int_instance);
        m_bifs.push_back(int_mul_function);

        // integer division
        std::shared_ptr<function> int_div_function = std::make_shared<function>(div_function_tok);
        int_div_function -> add_param(int_param);
        int_div_function -> add_param(int_param);
        int_div_function -> set_return_type_instance(int_instance);
        m_bifs.push_back(int_div_function);

        // integer hashing
        std::shared_ptr<function> int_hash_function = std::make_shared<function>(hash_function_tok);
        int_hash_function -> add_param(int_param);
        int_hash_function -> set_return_type_instance(int_instance);
        m_bifs.push_back(int_hash_function);


        /* functions for the decimal type */
        type_instance dec_instance(dec_type_tok, dec_type, "*");
        // variables
        variable dec_param(var_one_tok, false);
        dec_param.set_type_instance(dec_instance);

        // unary positive
        std::shared_ptr<function> dec_pos_function = std::make_shared<function>(pos_function_tok);
        dec_pos_function -> add_param(dec_param);
        dec_pos_function -> set_return_type_instance(dec_instance);
        m_bifs.push_back(dec_pos_function);

        // unary negative
        std::shared_ptr<function> dec_neg_function = std::make_shared<function>(neg_function_tok);
        dec_neg_function -> add_param(dec_param);
        dec_neg_function -> set_return_type_instance(dec_instance);
        m_bifs.push_back(dec_neg_function);

        // decimal addition
        std::shared_ptr<function> dec_add_function = std::make_shared<function>(add_function_tok);
        dec_add_function -> add_param(dec_param);
        dec_add_function -> add_param(dec_param);
        dec_add_function -> set_return_type_instance(dec_instance);
        m_bifs.push_back(dec_add_function);

        // decimal substraction
        std::shared_ptr<function> dec_sub_function = std::make_shared<function>(sub_function_tok);
        dec_sub_function -> add_param(dec_param);
        dec_sub_function -> add_param(dec_param);
        dec_sub_function -> set_return_type_instance(dec_instance);
        m_bifs.push_back(dec_sub_function);

        // decimal multiplication
        std::shared_ptr<function> dec_mul_function = std::make_shared<function>(mul_function_tok);
        dec_mul_function -> add_param(dec_param);
        dec_mul_function -> add_param(dec_param);
        dec_mul_function -> set_return_type_instance(dec_instance);
        m_bifs.push_back(dec_mul_function);

        // decimal division
        std::shared_ptr<function> dec_div_function = std::make_shared<function>(div_function_tok);
        dec_div_function -> add_param(dec_param);
        dec_div_function -> add_param(dec_param);
        dec_div_function -> set_return_type_instance(dec_instance);
        m_bifs.push_back(dec_div_function);


        /* functions for the floating point type */
        type_instance float_instance(float_type_tok, float_type, "*");
        // variables
        variable float_param(var_one_tok, false);
        float_param.set_type_instance(float_instance);

        // unary positive
        std::shared_ptr<function> float_pos_function = std::make_shared<function>(pos_function_tok);
        float_pos_function -> add_param(float_param);
        float_pos_function -> set_return_type_instance(float_instance);
        m_bifs.push_back(float_pos_function);

        // unary negative
        std::shared_ptr<function> float_neg_function = std::make_shared<function>(neg_function_tok);
        float_neg_function -> add_param(float_param);
        float_neg_function -> set_return_type_instance(float_instance);
        m_bifs.push_back(float_neg_function);

        // floating point addition
        std::shared_ptr<function> float_add_function = std::make_shared<function>(add_function_tok);
        float_add_function -> add_param(float_param);
        float_add_function -> add_param(float_param);
        float_add_function -> set_return_type_instance(float_instance);
        m_bifs.push_back(float_add_function);

        // floating point substraction
        std::shared_ptr<function> float_sub_function = std::make_shared<function>(sub_function_tok);
        float_sub_function -> add_param(float_param);
        float_sub_function -> add_param(float_param);
        float_sub_function -> set_return_type_instance(float_instance);
        m_bifs.push_back(float_sub_function);

        // floating point multiplication
        std::shared_ptr<function> float_mul_function = std::make_shared<function>(mul_function_tok);
        float_mul_function -> add_param(float_param);
        float_mul_function -> add_param(float_param);
        float_mul_function -> set_return_type_instance(float_instance);
        m_bifs.push_back(float_mul_function);

        // floating point division
        std::shared_ptr<function> float_div_function = std::make_shared<function>(div_function_tok);
        float_div_function -> add_param(float_param);
        float_div_function -> add_param(float_param);
        float_div_function -> set_return_type_instance(float_instance);
        m_bifs.push_back(float_div_function);


        /* function for the string type */
        type_instance string_instance(string_type_tok, string_type, "*");
        // variables
        variable string_param(var_one_tok, false);
        string_param.set_type_instance(string_instance);

        // string concatenation
        std::shared_ptr<function> string_concat_function = std::make_shared<function>(add_function_tok);
        string_concat_function -> add_param(string_param);
        string_concat_function -> add_param(string_param);
        string_concat_function -> set_return_type_instance(string_instance);
        m_bifs.push_back(string_concat_function);

        // string hashing
        std::shared_ptr<function> string_hash_function = std::make_shared<function>(hash_function_tok);
        string_hash_function -> add_param(string_param);
        string_hash_function -> set_return_type_instance(int_instance);
        m_bifs.push_back(string_hash_function);
    }
}
