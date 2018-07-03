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
#include "representer/hir/builtins/avalon_int.hpp"


namespace avalon {
    /**
     * the default constructor expects nothing
     */
    avalon_int::avalon_int() : m_int_type(std::make_shared<type>(int_type_tok, VALID)), m_int_instance(int_type_tok, m_int_type, "*") {
    }

    /**
     * get_type
     * returns the int type
     */
    std::shared_ptr<type>& avalon_int::get_type() {
        return m_int_type;
    }

    /**
     * get_type_instance
     * returns the int type instance
     */
    type_instance& avalon_int::get_type_instance() {
        return m_int_instance;
    }

    /**
     * get_program
     * returns a program that defines the int type and all functions that operate on ints
     */
    program& avalon_int::get_program() {
        /* the program FQN */
        fqn l_fqn("__bifqn__", "__bifqn__");
        m_int_prog.set_fqn(l_fqn);

        /* the program's scope */
        std::shared_ptr<scope> l_scope = std::make_shared<scope>();
        l_scope -> set_start_line(0);
        l_scope -> set_end_line(0);
        m_int_prog.set_scope(l_scope);

        /* create the namespace to the program */
        std::shared_ptr<ns> l_namespace = std::make_shared<ns>(star_tok);
        l_namespace -> set_fqn(l_fqn);

        /* add the type to the namespace */
        l_scope -> add_type(l_namespace -> get_name(), m_int_type);
        std::shared_ptr<decl> type_decl = m_int_type;
        l_namespace -> add_declaration(type_decl);

        /* add function declarations to the namespace */
        // variables
        variable param_one(var_one_tok, false);
        param_one.set_type_instance(m_int_instance);
        variable param_two(var_two_tok, false);
        param_two.set_type_instance(m_int_instance);

        // unary positive
        std::shared_ptr<function> int_pos_function = std::make_shared<function>(pos_function_tok);
        int_pos_function -> set_fqn(l_fqn);
        int_pos_function -> is_public(true);
        int_pos_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> int_pos_scope = std::make_shared<scope>();
        int_pos_scope -> set_parent(l_scope);
        int_pos_function -> set_scope(int_pos_scope);
        int_pos_function -> add_param(param_one);
        int_pos_function -> set_return_type_instance(m_int_instance);
        std::shared_ptr<decl> pos_function_decl = int_pos_function;
        l_namespace -> add_declaration(pos_function_decl);

        // unary negative
        std::shared_ptr<function> int_neg_function = std::make_shared<function>(neg_function_tok);
        int_neg_function -> set_fqn(l_fqn);
        int_neg_function -> is_public(true);
        int_neg_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> int_neg_scope = std::make_shared<scope>();
        int_neg_scope -> set_parent(l_scope);
        int_neg_function -> set_scope(int_neg_scope);
        int_neg_function -> add_param(param_one);
        int_neg_function -> set_return_type_instance(m_int_instance);
        std::shared_ptr<decl> neg_function_decl = int_neg_function;
        l_namespace -> add_declaration(neg_function_decl);

        // integer addition
        std::shared_ptr<function> int_add_function = std::make_shared<function>(add_function_tok);
        int_add_function -> set_fqn(l_fqn);
        int_add_function -> is_public(true);
        int_add_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> int_add_scope = std::make_shared<scope>();
        int_add_scope -> set_parent(l_scope);
        int_add_function -> set_scope(int_add_scope);
        int_add_function -> add_param(param_one);
        int_add_function -> add_param(param_two);
        int_add_function -> set_return_type_instance(m_int_instance);
        std::shared_ptr<decl> add_function_decl = int_add_function;
        l_namespace -> add_declaration(add_function_decl);

        // integer substraction
        std::shared_ptr<function> int_sub_function = std::make_shared<function>(sub_function_tok);
        int_sub_function -> set_fqn(l_fqn);
        int_sub_function -> is_public(true);
        int_sub_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> int_sub_scope = std::make_shared<scope>();
        int_sub_scope -> set_parent(l_scope);
        int_sub_function -> set_scope(int_sub_scope);
        int_sub_function -> add_param(param_one);
        int_sub_function -> add_param(param_two);
        int_sub_function -> set_return_type_instance(m_int_instance);
        std::shared_ptr<decl> sub_function_decl = int_sub_function;
        l_namespace -> add_declaration(sub_function_decl);

        // integer multiplication
        std::shared_ptr<function> int_mul_function = std::make_shared<function>(mul_function_tok);
        int_mul_function -> set_fqn(l_fqn);
        int_mul_function -> is_public(true);
        int_mul_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> int_mul_scope = std::make_shared<scope>();
        int_mul_scope -> set_parent(l_scope);
        int_mul_function -> set_scope(int_mul_scope);
        int_mul_function -> add_param(param_one);
        int_mul_function -> add_param(param_two);
        int_mul_function -> set_return_type_instance(m_int_instance);
        std::shared_ptr<decl> mul_function_decl = int_mul_function;
        l_namespace -> add_declaration(mul_function_decl);

        // integer division
        std::shared_ptr<function> int_div_function = std::make_shared<function>(div_function_tok);
        int_div_function -> set_fqn(l_fqn);
        int_div_function -> is_public(true);
        int_div_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> int_div_scope = std::make_shared<scope>();
        int_div_scope -> set_parent(l_scope);
        int_div_function -> set_scope(int_div_scope);
        int_div_function -> add_param(param_one);
        int_div_function -> add_param(param_two);
        int_div_function -> set_return_type_instance(m_int_instance);
        std::shared_ptr<decl> div_function_decl = int_div_function;
        l_namespace -> add_declaration(div_function_decl);

        // integer hashing
        std::shared_ptr<function> int_hash_function = std::make_shared<function>(hash_function_tok);
        int_hash_function -> set_fqn(l_fqn);
        int_hash_function -> is_public(true);
        int_hash_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> int_hash_scope = std::make_shared<scope>();
        int_hash_scope -> set_parent(l_scope);
        int_hash_function -> set_scope(int_hash_scope);
        int_hash_function -> add_param(param_one);
        int_hash_function -> set_return_type_instance(m_int_instance);
        std::shared_ptr<decl> hash_function_decl = int_hash_function;
        l_namespace -> add_declaration(hash_function_decl);

        /* add the namespace to the program */
        std::shared_ptr<decl> namespace_decl = l_namespace;
        m_int_prog.add_declaration(namespace_decl);

        return m_int_prog;
    }
}
