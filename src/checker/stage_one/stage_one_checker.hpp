#ifndef AVALON_CHECKER_STAGE_ONE_CHECKER_HPP_
#define AVALON_CHECKER_STAGE_ONE_CHECKER_HPP_

#include <memory>
#include <vector>
#include <string>

/* Error display */
#include "checker/check_error.hpp"

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "program/ast/decl/decl.hpp"
#include "program/ast/decl/type.hpp"
#include "program/ast/program.hpp"
/* Symbol table */
#include "program/symtable/gtable.hpp"
#include "program/symtable/scope.hpp"


namespace avalon {
    class stage_one_checker {
    public:
        /**
         * the constructor expects:
         * - the global symbol table with programs to check
         * - the path to the source containing the program we are validating
         * - the error handler to use in order to display errors
         */
        stage_one_checker(gtable& gtab, const std::string& source_path, error& error_handler);

        /**
         * check
         * the checker entry point
         * it validates that all programs found in the gtable are correct.
         * returns the program that may contan the __main__ function.
         */
        program check();

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

        /*
         * the source file currently being checked
         */
        std::string m_source_path;

        /*
         * the error handler to call in case of an error
         */
        error m_error_handler;

        /*
         * global symbol table with all the programs to check
         */
        gtable m_gtable;

        /*
         * the program we return that may eventually contain the main program
         */
        program m_prog;

        /*
         * vector of default types to be made available throughout the program
         */
        std::vector<std::shared_ptr<type> > m_bits;

        /**
         * import_declarations
         * Given two programs, import all the declarations in "from" program's scope into "to" program's scope
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
