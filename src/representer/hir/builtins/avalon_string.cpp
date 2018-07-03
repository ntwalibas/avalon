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
#include "representer/hir/builtins/avalon_string.hpp"


namespace avalon {
    /**
     * the default constructor expects nothing
     */
    avalon_string::avalon_string() : m_string_type(std::make_shared<type>(string_type_tok, VALID)), m_string_instance(string_type_tok, m_string_type, "*") {
    }

    /**
     * get_type
     * returns the int type
     */
    std::shared_ptr<type>& avalon_string::get_type() {
        return m_string_type;
    }

    /**
     * get_type_instance
     * returns the int type instance
     */
    type_instance& avalon_string::get_type_instance() {
        return m_string_instance;
    }

    /**
     * get_program
     * returns a program that defines the int type and all functions that operate on ints
     */
    program& avalon_string::get_program() {
        /* the program FQN */
        fqn l_fqn("__bifqn__", "__bifqn__");
        m_string_prog.set_fqn(l_fqn);

        /* the program's scope */
        std::shared_ptr<scope> l_scope = std::make_shared<scope>();
        l_scope -> set_start_line(0);
        l_scope -> set_end_line(0);
        m_string_prog.set_scope(l_scope);

        /* create the namespace to the program */
        std::shared_ptr<ns> l_namespace = std::make_shared<ns>(star_tok);
        l_namespace -> set_fqn(l_fqn);

        /* add the type to the scope and the namespace */
        l_scope -> add_type(l_namespace -> get_name(), m_string_type);
        std::shared_ptr<decl> type_decl = m_string_type;
        l_namespace -> add_declaration(type_decl);

        /* add function declarations to the namespace */
        // variables
        variable param_one(var_one_tok, false);
        param_one.set_type_instance(m_string_instance);
        variable param_two(var_two_tok, false);
        param_two.set_type_instance(m_string_instance);

        // string concatenation
        std::shared_ptr<function> string_concat_function = std::make_shared<function>(add_function_tok);
        string_concat_function -> set_fqn(l_fqn);
        string_concat_function -> is_public(true);
        string_concat_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> string_concat_scope = std::make_shared<scope>();
        string_concat_scope -> set_parent(l_scope);
        string_concat_function -> set_scope(string_concat_scope);
        string_concat_function -> add_param(param_one);
        string_concat_function -> add_param(param_two);
        string_concat_function -> set_return_type_instance(m_string_instance);
        std::shared_ptr<decl> concat_function_decl = string_concat_function;
        l_namespace -> add_declaration(concat_function_decl);

        // string hashing
        std::shared_ptr<function> string_hash_function = std::make_shared<function>(hash_function_tok);
        string_hash_function -> set_fqn(l_fqn);
        string_hash_function -> is_public(true);
        string_hash_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> string_hash_scope = std::make_shared<scope>();
        string_hash_scope -> set_parent(l_scope);
        string_hash_function -> set_scope(string_hash_scope);
        string_hash_function -> add_param(param_one);
        string_hash_function -> set_return_type_instance(m_string_instance);
        std::shared_ptr<decl> hash_function_decl = string_hash_function;
        l_namespace -> add_declaration(hash_function_decl);

        /* add the namespace to the program */
        std::shared_ptr<decl> namespace_decl = l_namespace;
        m_string_prog.add_declaration(namespace_decl);

        return m_string_prog;
    }
}
