#include <stdexcept>
#include <memory>
#include <string>
#include <vector>

/* Error */
#include "error/error.hpp"

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "program/ast/program.hpp"
#include "program/ast/decl/ns.hpp"
#include "program/ast/decl/decl.hpp"
#include "program/ast/decl/type.hpp"
#include "program/ast/decl/import.hpp"
#include "program/ast/decl/function.hpp"
#include "program/ast/decl/variable.hpp"
#include "program/ast/decl/statement.hpp"
/* Symbol table */
#include "program/symtable/exceptions/symbol_already_declared.hpp"
#include "program/symtable/gtable.hpp"
#include "program/symtable/scope.hpp"

/* Checker */
#include "checker/checker.hpp"
/* Import checker */
#include "checker/decl/import_checker.hpp"
#include "checker/exceptions/invalid_import.hpp"
/* Type checker */
#include "checker/exceptions/invalid_type.hpp"
#include "checker/decl/type_checker.hpp"


namespace avalon {
/**
 * the contructor expects:
 * - the error handler to be used to show errors
 * - the token where the error occured
 * - the message to show the user
 */
check_error::check_error(error& error_handler, token tok, bool fatal, const std::string& message) : std::runtime_error(message), m_error_handler(error_handler), m_tok(tok), m_fatal(fatal) {
}
    
    /**
     * what
     * get the error message
     */
    const char * check_error::what() const noexcept {
        return std::runtime_error::what();
    }

    /**
     * is_fatal
     * returns true if the error is fatal, false otherwise
     */
    bool check_error::is_fatal() {
        return m_fatal;
    }

    /**
     * show
     * calls the error handler error reporting function to display the error
     */
    void check_error::show() {
        m_error_handler.log(m_tok . get_line(), m_tok . get_column(), what());
    }

/**
 * the constructor expects:
 * - the path to the source containing the program we are validating
 * - the program to validate
 * - the error handler to use in order to display errors
 */
checker::checker(gtable& gtab, const std::string& source_path, error& error_handler) : m_source_path(source_path), m_error_handler(error_handler), m_gtable(gtab) {
}

    /**
     * check
     * the checker entry point
     * it validates that all programs found in the gtable are correct
     */
    void checker::check() {
        std::queue<std::string> checking_order = m_gtable.get_checking_order();
        while(!checking_order.empty()) {
            const std::string& fqn_name = checking_order.front();
            program& prog = m_gtable.get_program(fqn_name);
            check_program(prog);
            checking_order.pop();
        }
    }

    /**
     * check_program
     * given a program, check that all the declarations within it are correct
     */
    void checker::check_program(program& prog) {
        std::vector<std::shared_ptr<decl> >& declarations = prog.get_declarations();
        for(auto& declaration : declarations)
            check_declaration(declaration, prog);
    }

    /**
     * check_declaration
     * given a declaration, determine which type it is and dispatch the checking to the appropriate function
     */
    void checker::check_declaration(std::shared_ptr<decl>& declaration, program& prog) {
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
    void checker::check_import(std::shared_ptr<decl>& declaration, program& prog) {
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
    void checker::check_namespace(std::shared_ptr<decl>& declaration, program& prog) {
        std::shared_ptr<ns> namespace_decl = std::static_pointer_cast<ns>(declaration);
        std::shared_ptr<scope>& l_scope = prog.get_scope();

        // we add the namespace to the scope
        l_scope -> add_namespace(namespace_decl -> get_name());

        // we validate all declarations inside the namespace
        std::vector<std::shared_ptr<decl> >& declarations = namespace_decl -> get_declarations();
        for(auto& declaration : declarations) {
            // we don't allow import declarations inside namespaces
            if(declaration -> is_import()) {
                throw checking_error(true, namespace_decl -> get_token(), "Import declarations must be outside namespace declarations.");
            }
            // we don't allow other namespace declarations inside namespaces
            else if(declaration -> is_namespace()) {
                throw checking_error(true, namespace_decl -> get_token(), "Nested namespace declarations are not allowed.");
            }
            // we don't allow statement declarations inside namespaces
            else if(declaration -> is_statement()) {
                throw checking_error(true, namespace_decl -> get_token(), "Statement declarations are not allowed inside namespace declarations.");
            }
            else {
                check_top_declaration(declaration, prog, namespace_decl -> get_name());
            }
        }
    }

    /**
     * check_top_declaration
     * given a top declaration (type, variable, function or statement), find which is it and dispatch the checking to the appropriate function
     */
    void checker::check_top_declaration(std::shared_ptr<decl>& declaration, program& prog, const std::string& namespace_name) {
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
    void checker::check_type(std::shared_ptr<decl>& declaration, program& prog, const std::string& namespace_name) {
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
    void checker::check_function(std::shared_ptr<decl>& declaration, program& prog, const std::string& namespace_name) {
        std::shared_ptr<function> function_decl = std::static_pointer_cast<function>(declaration);
        
    }

    /**
     * check_variable
     * given a declaration, check if it is a valid variable
     */
    void checker::check_variable(std::shared_ptr<decl>& declaration, program& prog, const std::string& namespace_name) {
        std::shared_ptr<variable> variable_decl = std::static_pointer_cast<variable>(declaration);
    }

    /**
     * check_statement
     * given a declaration, check if it is a valid statement
     */
    void checker::check_statement(std::shared_ptr<decl>& declaration, program& prog, const std::string& namespace_name) {
        std::shared_ptr<statement> statement_decl = std::static_pointer_cast<statement>(declaration);
    }

    /**
     * importer
     * Given two programs, import all the declarations in "from" scope into "to" scope
     */
    void checker::import_declarations(program& from, program& to) {
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
                    if(type_decl -> is_public())
                        import_type(type_decl, to_scope, namespace_decl -> get_name());
                }
                else if(l_declaration -> is_function()) {
                    //throw std::runtime_error("[compiler error] Function declarations cannot be imported yet.");
                }
                else if(l_declaration -> is_variable()) {
                    //throw std::runtime_error("[compiler error] Variable declarations cannot be imported yet.");
                }
            }
        }
    }

    /**
     * import_type
     * Given a namespace name and a type declaration, insert the type into the given scope
     */
    void checker::import_type(std::shared_ptr<type>& type_decl, std::shared_ptr<scope>& scp, const std::string& namespace_name) {
        try {
            scp -> add_type(namespace_name, type_decl);
        } catch(symbol_already_declared err) {
            throw checking_error(true, type_decl -> get_token(), err.what());
        }
    }

    /**
     * checking_error
     * contructs and returns an check_error exception
     */
    check_error checker::checking_error(bool fatal, const token& tok, const std::string& message) {
        return check_error(m_error_handler, tok, fatal, message);
    }
}
