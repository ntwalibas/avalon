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
#include "representer/hir/builtins/avalon_unit.hpp"


namespace avalon {
    /**
     * the default constructor expects nothing
     */
    avalon_unit::avalon_unit() : m_unit_type(std::make_shared<type>(unit_type_tok, VALID)), m_unit_instance(unit_type_tok, m_unit_type, "*") {
        default_constructor unit_cons(unit_cons_tok, m_unit_type);
        m_unit_type -> add_constructor(unit_cons);
    }

    /**
     * get_type
     * returns the unit type
     */
    std::shared_ptr<type>& avalon_unit::get_type() {
        return m_unit_type;
    }

    /**
     * get_type_instance
     * returns the unit type instance
     */
    type_instance& avalon_unit::get_type_instance() {
        return m_unit_instance;
    }

    /**
     * get_program
     * returns a program that defines the unit type and all functions that operate on units
     */
    program& avalon_unit::get_program() {
        /* the program FQN */
        fqn l_fqn("__bifqn__", "__bifqn__");
        m_unit_prog.set_fqn(l_fqn);

        /* the program's scope */
        std::shared_ptr<scope> l_scope = std::make_shared<scope>();
        l_scope -> set_start_line(0);
        l_scope -> set_end_line(0);
        m_unit_prog.set_scope(l_scope);

        /* create the namespace to the program */
        std::shared_ptr<ns> l_namespace = std::make_shared<ns>(star_tok);
        l_namespace -> set_fqn(l_fqn);

        /* add the type to the namespace */
        std::shared_ptr<decl> type_decl = m_unit_type;
        l_namespace -> add_declaration(type_decl);

        /* add the namespace to the program */
        std::shared_ptr<decl> namespace_decl = l_namespace;
        m_unit_prog.add_declaration(namespace_decl);

        return m_unit_prog;
    }
}
