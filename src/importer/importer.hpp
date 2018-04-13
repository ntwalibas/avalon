#ifndef AVALON_IMPORTER_HPP_
#define AVALON_IMPORTER_HPP_

#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <string>
#include <vector>
#include <queue>

/* Error */
#include "error/error.hpp"

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "representer/hir/ast/program.hpp"
/* Symbol table */
#include "representer/hir/symtable/scope.hpp"
#include "representer/hir/symtable/gtable.hpp"

/* Compiler */
#include "compiler/compiler.hpp"


namespace avalon {
    class import_error : public std::runtime_error {
    public:
        /**
         * the contructor expects:
         * - the error handler to be used to show errors
         * - the token where the error occured
         * - whether the error is fatal or not
         * - the message to show the user
         */
        import_error(error& error_handler, token tok, bool fatal, const std::string& message);

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
         * fatal is used decide whether parsing should terminate when an error occurs
         */
        bool m_fatal;
    };

    class importer {
    public:
        /**
         * the constructor expects the main program that's root to all dependencies and the compiler to compile those dependencies
         */
        importer(program& prog, compiler& comp, error& error_handler);

        /**
         * import_all
         * the importer entry point
         * it constructs the gtable with all the imported files compiled
         */
        gtable& import_all();

    private:
        /**
         * visitation states used to detect cycles when topologically sorting dependencies
         */
        enum VISIT_STATES {
            NOT_VISITED,
            VISITING,
            VISITED
        };

        /**
         * generate_deps
         * given a program, this function gets all the pogram imports,
         * validates the imports and generates the dependency map.
         */
        void generate_deps(program& prog);

        /**
         * sort_deps
         * sort the dependency graph that makes up the program,
         * making sure no cycles are allowed among dependencies
         */
        void sort_deps();

        /**
         * run_imports
         * performs imports of declarations from one program to another
         */
        void run_imports();

        /*
         * the origin program
         * this is the program all other imports depend on
         */
        program m_program;

        /*
         * the compiler we need to call in order to compile dependencies starting with the main prgram's
         */
        compiler m_compiler;

        /*
         * the error handler to call in case of an error
         */
        error m_error_handler;

        /*
         * the global symbol table containing all the declarations made available by compiled programs
         */
        gtable m_gtable;

        /**
         * sort_deps_util
         * perform a topological sort of the dependency graph.
         *
         * REMARK: Initially, I thought about using the algorithm provided by boost::graph but I could not understand the thing.
         * When I do, I might just use it.
         */
        void sort_deps_util(const std::string& dep_name, std::vector<std::string>& sub_deps);

        /**
         * run_imports_util
         * goes through all top declarations in a program, finds import declarations and imports those declarations into the current program's scope
         */
        void run_imports_util(program& prog);

        /**
         * import_declarations
         * given two programs, import all the declarations in "from" program into "to" program's scope
         */
        void import_declarations(program& from, program& to);

        /**
         * import_type
         * given a namespace name and a type declaration, insert the type into the given scope
         */
        void import_type(std::shared_ptr<type>& type_decl, std::shared_ptr<scope>& scp, const std::string& namespace_name);

        /**
         * import_function
         * given a namespace name and a function declaration, insert the function into the given scope
         */
        void import_function(std::shared_ptr<function>& function_decl, std::shared_ptr<scope>& scp, const std::string& namespace_name);

        /**
         * import_variable
         * given a namespace name and a variable declaration, insert the variable into the given scope
         */
        void import_variable(std::shared_ptr<variable>& variable_decl, std::shared_ptr<scope>& scp, const std::string& namespace_name);

        /**
         * importing_error
         * contructs and returns an import_error exception
         */
        import_error importing_error(bool fatal, const token& tok, const std::string& message);

        /*
         * a dependency map between different the importing program and the imported programs
         */
        std::unordered_map<std::string, std::vector<std::string> > m_deps;

        /*
         * a map between dependencies and their visitation state indicating if dependency has been visited or not or is being visisted
         */
        std::unordered_map<std::string, VISIT_STATES> m_dep_states;

        /*
         * a queue of all dependencies sorted from the one with no dependency to the one with most dependencies
         */
        std::queue<std::string> m_sorted_deps;
    };
}

#endif
