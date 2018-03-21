#ifndef AVALON_CHECKER_CHECKER_HPP_
#define AVALON_CHECKER_CHECKER_HPP_

#include <string>

/* AST */
#include "program/ast/program.hpp"
/* Symbol table */
#include "program/symtable/gtable.hpp"


namespace avalon {
    class checker {
    public:
        /**
         * the constructor expects:
         * - the global symbol table with programs to check
         * - the path to the source containing the program we are validating
         * - the error handler to use in order to display errors
         */
        checker(gtable& gtab, const std::string& source_path, error& error_handler);

        /**
         * check
         * the checker entry point
         * it validates that all programs found in the gtable are correct.
         * returns the program that may contan the __main__ function.
         */
        program check();

    private:
        /**
         * init_stage_two
         * this function checks if the top program from stage one contains the __main__ function.
         * if it does, it initiates stage two.
         * if it does not, it will raise an exception stating that the __main__ function is missing.
         */
        void init_stage_two(program& prog);

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
    };
}

#endif
