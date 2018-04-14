#include <memory>

/* Lexer */
#include "lexer/token.hpp"

/* AST */
#include "representer/hir/ast/decl/function.hpp"
#include "representer/hir/ast/decl/variable.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "representer/hir/ast/decl/decl.hpp"
#include "representer/hir/ast/decl/ns.hpp"
#include "representer/hir/ast/program.hpp"

/* Symbol table */
#include "representer/hir/symtable/scope.hpp"
#include "representer/hir/symtable/fqn.hpp"

/* Builtins */
#include "representer/hir/builtins/avalon_bool.hpp"


namespace avalon {
    /**
     * the default constructor expects nothing
     */
    avalon_bool::avalon_bool() : m_bool_type(std::make_shared<type>(bool_type_tok, VALID)), m_bool_instance(bool_type_tok, m_bool_type, "*") {
        default_constructor true_cons(true_cons_tok, m_bool_type);
        default_constructor false_cons(false_cons_tok, m_bool_type);
        m_bool_type -> add_constructor(true_cons);
        m_bool_type -> add_constructor(false_cons);
    }

    /**
     * get_type
     * returns the bool type
     */
    const std::shared_ptr<type>& avalon_bool::get_type() const {
        return m_bool_type;
    }

    /**
     * get_type_instance
     * returns the bool type instance
     */
    const type_instance& avalon_bool::get_type_instance() const {
        return m_bool_instance;
    }

    /**
     * get_program
     * returns a program that defines the bool type and all functions that operate on bools
     */
    program& avalon_bool::get_program() {
        /* the program FQN */
        fqn l_fqn("__bifqn__", "__bifqn__");
        m_bool_prog.set_fqn(l_fqn);

        /* the program's scope */
        std::shared_ptr<scope> l_scope = std::make_shared<scope>();
        l_scope -> set_start_line(0);
        l_scope -> set_end_line(0);
        m_bool_prog.set_scope(l_scope);

        /* create the namespace to the program */
        std::shared_ptr<ns> l_namespace = std::make_shared<ns>(star_tok);
        l_namespace -> set_fqn(l_fqn);

        /* add the type to the namespace */
        std::shared_ptr<decl> type_decl = m_bool_type;
        l_namespace -> add_declaration(type_decl);

        /* add function declarations to the namespace */
        // variables
        variable bool_param(var_one_tok, false);
        bool_param.set_type_instance(m_bool_instance);

        // boolean and
        std::shared_ptr<function> bool_and_function = std::make_shared<function>(and_function_tok);
        bool_and_function -> add_param(bool_param);
        bool_and_function -> add_param(bool_param);
        bool_and_function -> set_return_type_instance(m_bool_instance);
        std::shared_ptr<decl> and_function_decl = bool_and_function;
        l_namespace -> add_declaration(and_function_decl);

        // boolean or
        std::shared_ptr<function> bool_or_function = std::make_shared<function>(or_function_tok);
        bool_or_function -> add_param(bool_param);
        bool_or_function -> add_param(bool_param);
        bool_or_function -> set_return_type_instance(m_bool_instance);
        std::shared_ptr<decl> or_function_decl = bool_or_function;
        l_namespace -> add_declaration(or_function_decl);

        // boolean not
        std::shared_ptr<function> bool_not_function = std::make_shared<function>(not_function_tok);
        bool_not_function -> add_param(bool_param);
        bool_not_function -> set_return_type_instance(m_bool_instance);
        std::shared_ptr<decl> not_function_decl = bool_not_function;
        l_namespace -> add_declaration(not_function_decl);

        /* add the namespace to the program */
        std::shared_ptr<decl> namespace_decl = l_namespace;
        m_bool_prog.add_declaration(namespace_decl);

        return m_bool_prog;
    }
}
