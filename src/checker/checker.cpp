#include <stdexcept>
#include <string>

/* Error display */
#include "checker/check_error.hpp"

/* AST */
#include "program/ast/program.hpp"
/* Symbol table */
#include "program/symtable/gtable.hpp"

/* Checker */
#include "checker/lax/lax_checker.hpp"
#include "checker/checker.hpp"


namespace avalon {
    /**
     * the constructor expects:
     * - the global symbol table with programs to check
     * - the path to the source containing the program we are validating
     * - the error handler to use in order to display errors
     */
    checker::checker(gtable& gtab, const std::string& source_path, error& error_handler) : m_source_path(source_path), m_error_handler(error_handler), m_gtable(gtab) {
    }

    /**
     * check
     * the checker entry point
     * it validates that all programs found in the gtable are correct.
     * returns the program that may contan the __main__ function.
     */
    program checker::check() {
        lax_checker checker_one(m_gtable, m_source_path, m_error_handler);
        try {
            program prog = checker_one.check();
            init_stage_two(prog);
            return prog;
        } catch(check_error err) {
            throw err;
        } catch(std::runtime_error err) {
            throw err;
        }
        
    }

    /**
     * init_stage_two
     * this function checks if the top program from stage one contains the __main__ function.
     * if it does, it initiates stage two.
     * if it does not, it will raise an exception stating that the __main__ function is missing.
     */
    void checker::init_stage_two(program& prog) {
        
    }
}
