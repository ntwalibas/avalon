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
#include "representer/hir/builtins/avalon_tuple.hpp"


namespace avalon {
    /**
     * the default constructor expects the dependent type instance parameter
     */
    avalon_tuple::avalon_tuple() : m_tuple_type(std::make_shared<type>(tuple_type_tok, VALID)), m_tuple_instance(tuple_type_tok, m_tuple_type, "*") {
        m_tuple_instance.set_category(TUPLE);
    }

    /**
     * get_type
     * returns the tuple type
     */
    std::shared_ptr<type>& avalon_tuple::get_type() {
        return m_tuple_type;
    }

    /**
     * get_type_instance
     * returns the tuple type instance
     */
    type_instance avalon_tuple::get_type_instance() {
        return m_tuple_instance;
    }

    /**
     * get_program
     * returns a program that defines the tuple type and all functions that operate on tuples
     */
    program& avalon_tuple::get_program() {
        /* the program FQN */
        fqn l_fqn("__bifqn__", "__bifqn__");
        m_tuple_prog.set_fqn(l_fqn);

        /* the program's scope */
        std::shared_ptr<scope> l_scope = std::make_shared<scope>();
        l_scope -> set_start_line(0);
        l_scope -> set_end_line(0);
        m_tuple_prog.set_scope(l_scope);

        /* create the namespace to the program */
        std::shared_ptr<ns> l_namespace = std::make_shared<ns>(star_tok);
        l_namespace -> set_fqn(l_fqn);

        /* add the type to the scope and the namespace */
        l_scope -> add_type(l_namespace -> get_name(), m_tuple_type);
        std::shared_ptr<decl> type_decl = m_tuple_type;
        l_namespace -> add_declaration(type_decl);

        /* add function declarations to the namespace */
        // functions to be added later        

        /* add the namespace to the program */
        std::shared_ptr<decl> namespace_decl = l_namespace;
        m_tuple_prog.add_declaration(namespace_decl);

        return m_tuple_prog;
    }
}
