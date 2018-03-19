#ifndef AVALON_CHECKER_HPP_
#define AVALON_CHECKER_HPP_

#include <memory>
#include <string>

/* Error */
#include "error/error.hpp"

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "program/ast/program.hpp"
#include "program/ast/decl/decl.hpp"
/* Symbol table */
#include "program/symtable/gtable.hpp"
#include "program/symtable/scope.hpp"


namespace avalon {
    class check_error : public std::runtime_error {
    public:
        /**
         * the contructor expects:
         * - the error handler to be used to show errors
         * - the token where the error occured
         * - the message to show the user
         */
        check_error(error& error_handler, token tok, bool fatal, const std::string& message);

        /**
         * what
         * get the error message
         */
        virtual const char * what() const noexcept;

        /**
         * is_fatal
         * returns true if the error is fatal, false otherwise
         */
        bool is_fatal();

        /**
         * show
         * calls the error handler error reporting function to display the error
         */
        void show();

    private:
        /**
         * the error handler used to show errors to the user
         */
        error m_error_handler;

        /**
         * the token with information needed to display complete and useful error messages
         * information carried by the token include:
         * - the file with the faulty construct
         * - the line where the error occured
         * - the column where the faulty construct occured (combined with the token lexeme length, we can show better messages)
         */
        token m_tok;

        /**
         * fatal is used decide whether checking should terminate when an error occurs
         */
        bool m_fatal;
    };



    class checker {
    public:
        /**
         * the constructor expects:
         * - the global symbol table with programs to run
         * - the path to the source containing the program we are validating
         * - the error handler to use in order to display errors
         */
        checker(gtable& gtab, const std::string& source_path, error& error_handler);

        /**
         * check
         * the checker entry point
         * it validates that all programs found in the gtable are correct
         */
        void check();

    private:
        /**
         * check_program
         * given a program, check that all the declarations within it are correct
         */
        void check_program(program& prog);

        /**
         * check_declaration
         * given a declaration, determine which type it is and dispatch the checking to the appropriate function
         */
        void check_declaration(std::shared_ptr<decl>& declaration, program& prog);

        /**
         * check_import
         * given a declaration, check if it is a valid import declaration
         */
        void check_import(std::shared_ptr<decl>& declaration, program& prog);

        /**
         * check_namespace
         * given a declaration, check if it is a valid namespace
         */
        void check_namespace(std::shared_ptr<decl>& declaration, program& prog);

        /**
         * check_top_declaration
         * given a top declaration (type, variable, function or statement), find which is it and dispatch the checking to the appropriate function
         */
        void check_top_declaration(std::shared_ptr<decl>& declaration, program& prog, const std::string& namespace_name);

        /**
         * check_type
         * given a declaration, check if it is a valid type
         */
        void check_type(std::shared_ptr<decl>& declaration, program& prog, const std::string& namespace_name);

        /**
         * check_function
         * given a declaration, check if it is a valid function
         */
        void check_function(std::shared_ptr<decl>& declaration, program& prog, const std::string& namespace_name);

        /**
         * check_variable
         * given a declaration, check if it is a valid variable
         */
        void check_variable(std::shared_ptr<decl>& declaration, program& prog, const std::string& namespace_name);

        /**
         * check_statement
         * given a declaration, check if it is a valid statement
         */
        void check_statement(std::shared_ptr<decl>& declaration, program& prog, const std::string& namespace_name);

        /**
         * the source file currently being checked
         */
        std::string m_source_path;

        /**
         * the error handler to call in case of an error
         */
        error m_error_handler;

        /**
         * global symbol table with all the programs to check
         */
        gtable m_gtable;

        /**
         * import_declarations
         * Given two programs, import all the declarations in "from" scope into "to" scope
         */
        void import_declarations(program& from, program& to);

        /**
         * import_type
         * Given a namespace name and a type declaration, insert the type into the given scope
         */
        void import_type(std::shared_ptr<type>& type_decl, std::shared_ptr<scope>& scp, const std::string& namespace_name);

        /**
         * checking_error
         * contructs and returns a check_error exception
         */
        check_error checking_error(bool fatal, const token& tok, const std::string& message);
    };
}

#endif
