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
#include "representer/hir/builtins/avalon_void.hpp"


namespace avalon {
    /**
     * the default constructor expects nothing
     */
    avalon_void::avalon_void() : m_void_type(std::make_shared<type>(void_type_tok, VALID)), m_void_instance(void_type_tok, m_void_type, "*") {
    }

    /**
     * get_type
     * returns the void type
     */
    std::shared_ptr<type>& avalon_void::get_type() {
        return m_void_type;
    }

    /**
     * get_type_instance
     * returns the void type instance
     */
    type_instance& avalon_void::get_type_instance() {
        return m_void_instance;
    }

    /**
     * get_program
     * returns a program that defines the void type and all functions that operate on voids
     */
    program& avalon_void::get_program() {
        /* the program FQN */
        fqn l_fqn("__bifqn__", "__bifqn__");
        m_void_prog.set_fqn(l_fqn);

        /* the program's scope */
        std::shared_ptr<scope> l_scope = std::make_shared<scope>();
        l_scope -> set_start_line(0);
        l_scope -> set_end_line(0);
        m_void_prog.set_scope(l_scope);

        /* create the namespace to the program */
        std::shared_ptr<ns> l_namespace = std::make_shared<ns>(star_tok);
        l_namespace -> set_fqn(l_fqn);

        /* add the type to the namespace */
        std::shared_ptr<decl> type_decl = m_void_type;
        l_namespace -> add_declaration(type_decl);

        /* add the namespace to the program */
        std::shared_ptr<decl> namespace_decl = l_namespace;
        m_void_prog.add_declaration(namespace_decl);

        return m_void_prog;
    }
}
