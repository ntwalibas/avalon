#ifndef AVALON_CHECKER_HPP_
#define AVALON_CHECKER_HPP_

#include <stdexcept>
#include <string>

/* Error */
#include "error/error.hpp"

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "representer/hir/ast/program.hpp"
#include "representer/hir/ast/decl/decl.hpp"
#include "representer/hir/ast/decl/type.hpp"

/* Symbol table */
#include "representer/hir/symtable/scope.hpp"
#include "representer/hir/symtable/gtable.hpp"


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
         * - the global symbol table with programs to check
         * - the path to the source containing the program we are validating
         * - the error handler to use in order to display errors
         */
        checker(program& prog, gtable& gtab, const std::string& source_path, error& error_handler);

        /**
         * check
         * the checker entry point
         * it ensure that all the main program contains the main function
         * then check all declarations that the main function depends on
         */
        void check();

        /**
         * check_main
         * given the main function name, parameters' type instances and the return type,
         * find the function that matches the same and check said function under those assumptions
         */
        void check_main(const std::string& name, std::vector<type_instance>& param_instances, type_instance& ret_instance, std::shared_ptr<scope>& l_scope, const std::string& ns_name);

    private:
        /*
         * the source file currently being checked
         */
        std::string m_source_path;

        /*
         * the error handler to call in case of an error
         */
        error m_error_handler;

        /*
         * the program that contains the main function
         */
        program m_prog;

        /*
         * HIR global symbol table with all the programs to check
         */
        gtable m_gtable;

        /**
         * checking_error
         * contructs and returns a check_error exception
         */
        check_error checking_error(bool fatal, const std::string& message);
        check_error checking_error(bool fatal, const token& tok, const std::string& message);
    };
}

#endif
