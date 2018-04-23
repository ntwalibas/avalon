#include <unordered_map>
#include <stdexcept>
#include <utility>
#include <memory>
#include <vector>

/* Error */
#include "error/error.hpp"

/* AST */
#include "representer/hir/ast/decl/import.hpp"
#include "representer/hir/ast/decl/decl.hpp"
#include "representer/hir/ast/program.hpp"

/* Built-in types */
#include "representer/hir/builtins/avalon_string.hpp"
#include "representer/hir/builtins/avalon_float.hpp"
#include "representer/hir/builtins/avalon_unit.hpp"
#include "representer/hir/builtins/avalon_void.hpp"
#include "representer/hir/builtins/avalon_bool.hpp"
#include "representer/hir/builtins/avalon_dec.hpp"
#include "representer/hir/builtins/avalon_int.hpp"

/* Symbol table */
#include "representer/exceptions/symbol_already_declared.hpp"
#include "representer/hir/symtable/gtable.hpp"
#include "representer/hir/symtable/scope.hpp"
#include "representer/hir/symtable/fqn.hpp"

/* Checker */
#include "checker/decl/function/header_checker.hpp"

/* Parser */
#include "parser/parser.hpp"

/* Lexer */
#include "lexer/token.hpp"
#include "lexer/lexer.hpp"

/* Utilities */
#include "utils/exceptions/invalid_directory.hpp"
#include "utils/exceptions/file_not_found.hpp"
#include "utils/parse_util.hpp"

/* Importer */
#include "importer/importer.hpp"


namespace avalon {
/**
 * the contructor expects:
 * - the error handler to be used to show errors
 * - the token where the error occured
 * - whether the error is fatal or not
 * - the message to show the user
 */
import_error::import_error(error& error_handler, token tok, bool fatal, const std::string& message) : std::runtime_error(message), m_error_handler(error_handler), m_tok(tok), m_fatal(fatal) {
}
    
    /**
     * what
     * get the error message
     */
    const char * import_error::what() const noexcept {
        return std::runtime_error::what();
    }

    /**
     * is_fatal
     * returns true if the error is fatal, false otherwise
     */
    bool import_error::is_fatal() {
        return m_fatal;
    }

    /**
     * show
     * calls the error handler error reporting function to display the error
     */
    void import_error::show() {
        m_error_handler.log(m_tok . get_line(), m_tok . get_column(), what());
    }


/**
 * the constructor expects the main program that's root to all dependencies and the compiler to compile those dependencies
 */
importer::importer(program& prog, std::vector<std::string>& search_paths, error& error_handler) : m_program(prog), m_search_paths(search_paths), m_error_handler(error_handler) {
}

    /**
     * import_all
     * the importer entry point
     * it constructs the gtable with all the imported files parsed
     */
    gtable& importer::import_all() {
        // we generate all the dependencies that we will ultimately build the program
        generate_deps(m_program);

        // we sort the dependencies, making sure no cycles are allowed among them
        sort_deps();

        // we import public declarations from one program to another
        run_imports();

        // last, we return the gtable
        return m_gtable;
    }

    /**
     * generate_deps
     * given a program, this function gets all the program imports,
     * validates the imports and generates the dependency map.
     */
    void importer::generate_deps(program& prog) {
        std::string prog_fqn_name = prog.get_fqn().get_name();

        // if the program already exists in the global symbol table, we return
        if(m_gtable.program_exists(prog_fqn_name))
            return;
        else
            m_gtable.add_program(prog);

        // add the program fqn to the dependency graph
        std::vector<std::string> deps;
        m_deps.emplace(prog_fqn_name, deps);

        // add the program fqn to the dependency visitation map
        m_dep_states.emplace(prog_fqn_name, NOT_VISITED);

        // go over the declarations looking for import declarations
        std::vector<std::shared_ptr<decl> >& decls = prog.get_declarations();
        for(auto& decl : decls) {
            // if the declaration is not an import declaration, we go to the next
            if(decl -> is_import() == false)
                continue;

            std::shared_ptr<import> import_decl = std::static_pointer_cast<import>(decl);
            const std::string& import_fqn_name = import_decl -> get_fqn_name();
            fqn import_fqn = fqn::from_name(import_fqn_name);

            // now, we do the actual work of importing dependencies
            // first, we parse the file introduced by the dependency
            program import_prog;
            try {
                import_prog = parse_util::parse(import_fqn.get_path(), m_search_paths);
            } catch(file_not_found err) {
                throw importing_error(true, import_decl -> get_token(), "Failed to import <" + import_fqn_name + ">. Does the file associated with it exist?");
            } catch(lex_error err) {
                throw err;
            } catch(parse_error err) {
                throw err;
            }

            // then, if the parsing was successful, we add the dependency to the depedency graph
            m_deps.at(prog_fqn_name).push_back(import_fqn_name);
            generate_deps(import_prog);
        }
    }

    /**
     * sort_deps
     * sort the dependency graph that makes up the program,
     * making sure no cycles are allowed among dependencies
     */
    void importer::sort_deps() {
        // go over all the the dependencies and sort them
        for(auto& dep : m_deps)
            if(m_dep_states.at(dep.first) == NOT_VISITED)
                sort_deps_util(dep.first, dep.second);
    }

    /**
     * sort_deps_util
     * perform a topological sort of the dependency graph.
     *
     * REMARK: Initially, I thought about using the algorithm provided by boost::graph but I could not understand the thing.
     * When I do, I might just use it.
     */
    void importer::sort_deps_util(const std::string& dep, std::vector<std::string>& sub_deps) {
        // mark the current dependency as being visited
        m_dep_states[dep] = VISITING;

        // go over the current dependency sub-dependencies and sort them
        for(auto& sub_dep : sub_deps) {
            // if not visited, we carry on
            if(m_dep_states.at(sub_dep) == NOT_VISITED)
                sort_deps_util(sub_dep, m_deps.at(sub_dep));
            // if currently visiting, then we have a cycle
            else if(m_dep_states.at(sub_dep) == VISITING)
                throw std::runtime_error("There is a cycle between importer <" + dep + "> and importee <" + sub_dep + ">.");
        }

        // we add our dependency to the sort list(queue)
        m_sorted_deps.push(dep);

        // mark the current dependency as visited
        m_dep_states[dep] = VISITED;
    }

    /**
     * run_imports
     * performs imports of declarations from one program to another
     */
    void importer::run_imports() {
        // we then add user defined declarations
        while(!m_sorted_deps.empty()) {
            const std::string& fqn_name = m_sorted_deps.front();
            program& prog = m_gtable.get_program(fqn_name);
            run_imports_util(prog);
            m_sorted_deps.pop();
        }
    }

    /**
     * run_imports_util
     * goes through all top declarations in a program, finds import declarations and imports those declarations into the current program's scope
     */
    void importer::run_imports_util(program& prog) {
        // 1. we import built in declarations
        run_builtin_imports(prog);

        // 2. we then import custom imported declarations
        std::vector<std::shared_ptr<decl> >& declarations = prog.get_declarations();
        for(auto& declaration : declarations) {
            if(declaration -> is_import()) {
                const std::shared_ptr<import>& import_decl = std::static_pointer_cast<import>(declaration);
                program& imported_prog = m_gtable.get_program(import_decl -> get_fqn_name());
                import_declarations(imported_prog, prog, false);
            }
        }

        // 3. we last perform a self-import (essentally we add defined declarations from the current program into its scope)
        import_declarations(prog, prog, true);
    }

    /**
     * run_builtin_imports
     * imports built in declarations into the given program
     */
    void importer::run_builtin_imports(program& to) {
        // void declarations
        avalon_void avl_void;
        program& void_prog = avl_void.get_program();
        import_declarations(void_prog, to, false);

        // unit declarations
        avalon_unit avl_unit;
        program& unit_prog = avl_unit.get_program();
        import_declarations(unit_prog, to, false);

        // bool declarations
        avalon_bool avl_bool;
        program& bool_prog = avl_bool.get_program();
        import_declarations(bool_prog, to, false);

        // integer declarations
        avalon_int avl_int;
        program& int_prog = avl_int.get_program();
        import_declarations(int_prog, to, false);

        // decimal declarations
        avalon_dec avl_dec;
        program& dec_prog = avl_dec.get_program();
        import_declarations(dec_prog, to, false);

        // floating point declarations
        avalon_float avl_float;
        program& float_prog = avl_float.get_program();
        import_declarations(float_prog, to, false);

        // string declarations
        avalon_string avl_string;
        program& string_prog = avl_string.get_program();
        import_declarations(string_prog, to, false);
    }

    /**
     * import_declarations
     * given two programs, import all the declarations in "from" program into "to" program's scope
     */
    void importer::import_declarations(program& from, program& to, bool include_privates) {
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
                to_scope -> add_namespace(namespace_decl -> get_name());
            }
        }

        // go over the namespaces and insert the public declarations found in them into the "to" scope
        for(auto& namespace_decl : namespace_decls) {
            std::vector<std::shared_ptr<decl> >& l_declarations = namespace_decl -> get_declarations();
            for(auto& l_declaration : l_declarations) {
                if(l_declaration -> is_type()) {
                    std::shared_ptr<type> type_decl = std::static_pointer_cast<type>(l_declaration);
                    // if "include_privates" is true, it doesn't matter whether the declaration is public, we import it
                    // if "include_privates" is false, we only import the declaration if it is public
                    if(include_privates || type_decl -> is_public()) {
                        import_type(type_decl, to_scope, namespace_decl -> get_name());
                    }
                }
                else if(l_declaration -> is_function()) {
                    std::shared_ptr<function> function_decl = std::static_pointer_cast<function>(l_declaration);
                    // if "include_privates" is true, it doesn't matter whether the declaration is public, we import it
                    // if "include_privates" is false, we only import the declaration if it is public
                    if(include_privates || function_decl -> is_public()) {
                        header_checker::check_header(* function_decl, to_scope, namespace_decl -> get_name());
                        import_function(function_decl, to_scope, namespace_decl -> get_name());
                    }
                }
                else if(l_declaration -> is_variable()) {
                    std::shared_ptr<variable> variable_decl = std::static_pointer_cast<variable>(l_declaration);
                    // if "include_privates" is true, it doesn't matter whether the declaration is public, we import it
                    // if "include_privates" is false, we only import the declaration if it is public
                    if(include_privates || variable_decl -> is_public()) {
                        import_variable(variable_decl, to_scope, namespace_decl -> get_name());
                    }
                }
            }
        }
    }

    /**
     * import_type
     * given a namespace name and a type declaration, insert the type into the given scope
     */
    void importer::import_type(std::shared_ptr<type>& type_decl, std::shared_ptr<scope>& scp, const std::string& namespace_name) {
        try {
            scp -> add_type(namespace_name, type_decl);
        } catch(symbol_already_declared err) {
            throw importing_error(true, type_decl -> get_token(), err.what());
        }
    }

    /**
     * import_function
     * given a namespace name and a function declaration, insert the function into the given scope
     */
    void importer::import_function(std::shared_ptr<function>& function_decl, std::shared_ptr<scope>& scp, const std::string& namespace_name) {
        try {
            scp -> add_function(namespace_name, function_decl);
        } catch(symbol_already_declared err) {
            throw importing_error(true, function_decl -> get_token(), err.what());
        }
    }

    /**
     * import_variable
     * given a namespace name and a variable declaration, insert the variable into the given scope
     */
    void importer::import_variable(std::shared_ptr<variable>& variable_decl, std::shared_ptr<scope>& scp, const std::string& namespace_name) {
        try {
            scp -> add_variable(namespace_name, variable_decl);
        } catch(symbol_already_declared err) {
            throw importing_error(true, variable_decl -> get_token(), err.what());
        }
    }

    /**
     * importing_error
     * contructs and returns an import_error exception
     */
    import_error importer::importing_error(bool fatal, const token& tok, const std::string& message) {
        return import_error(m_error_handler, tok, fatal, message);
    }
}
