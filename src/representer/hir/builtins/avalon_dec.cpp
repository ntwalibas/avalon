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
#include "representer/hir/builtins/avalon_dec.hpp"


namespace avalon {
    /**
     * the default constructor expects nothing
     */
    avalon_dec::avalon_dec() : m_dec_type(std::make_shared<type>(dec_type_tok, VALID)), m_dec_instance(dec_type_tok, m_dec_type, "*") {
    }

    /**
     * get_type
     * returns the dec type
     */
    std::shared_ptr<type>& avalon_dec::get_type() {
        return m_dec_type;
    }

    /**
     * get_type_instance
     * returns the dec type instance
     */
    type_instance& avalon_dec::get_type_instance() {
        return m_dec_instance;
    }

    /**
     * get_program
     * returns a program that defines the dec type and all functions that operate on decs
     */
    program& avalon_dec::get_program() {
        /* the program FQN */
        fqn l_fqn("__bifqn__", "__bifqn__");
        m_dec_prog.set_fqn(l_fqn);

        /* the program's scope */
        std::shared_ptr<scope> l_scope = std::make_shared<scope>();
        l_scope -> set_start_line(0);
        l_scope -> set_end_line(0);
        m_dec_prog.set_scope(l_scope);

        /* create the namespace to the program */
        std::shared_ptr<ns> l_namespace = std::make_shared<ns>(star_tok);
        l_namespace -> set_fqn(l_fqn);

        /* add the type to the namespace */
        l_scope -> add_type(l_namespace -> get_name(), m_dec_type);
        std::shared_ptr<decl> type_decl = m_dec_type;
        l_namespace -> add_declaration(type_decl);

        /* add function declarations to the namespace */
        // variables
        variable param_one(var_one_tok, false);
        param_one.set_type_instance(m_dec_instance);
        variable param_two(var_two_tok, false);
        param_two.set_type_instance(m_dec_instance);

        // unary positive
        std::shared_ptr<function> dec_pos_function = std::make_shared<function>(pos_function_tok);
        dec_pos_function -> set_fqn(l_fqn);
        dec_pos_function -> is_public(true);
        dec_pos_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> dec_pos_scope = std::make_shared<scope>();
        dec_pos_scope -> set_parent(l_scope);
        dec_pos_function -> set_scope(dec_pos_scope);
        dec_pos_function -> add_param(param_one);
        dec_pos_function -> set_return_type_instance(m_dec_instance);
        std::shared_ptr<decl> pos_function_decl = dec_pos_function;
        l_namespace -> add_declaration(pos_function_decl);

        // unary negative
        std::shared_ptr<function> dec_neg_function = std::make_shared<function>(neg_function_tok);
        dec_neg_function -> set_fqn(l_fqn);
        dec_neg_function -> is_public(true);
        dec_neg_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> dec_neg_scope = std::make_shared<scope>();
        dec_neg_scope -> set_parent(l_scope);
        dec_neg_function -> set_scope(dec_neg_scope);
        dec_neg_function -> add_param(param_one);
        dec_neg_function -> set_return_type_instance(m_dec_instance);
        std::shared_ptr<decl> neg_function_decl = dec_neg_function;
        l_namespace -> add_declaration(neg_function_decl);

        // decimal addition
        std::shared_ptr<function> dec_add_function = std::make_shared<function>(add_function_tok);
        dec_add_function -> set_fqn(l_fqn);
        dec_add_function -> is_public(true);
        dec_add_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> dec_add_scope = std::make_shared<scope>();
        dec_add_scope -> set_parent(l_scope);
        dec_add_function -> set_scope(dec_add_scope);
        dec_add_function -> add_param(param_one);
        dec_add_function -> add_param(param_two);
        dec_add_function -> set_return_type_instance(m_dec_instance);
        std::shared_ptr<decl> add_function_decl = dec_add_function;
        l_namespace -> add_declaration(add_function_decl);

        // decimal substraction
        std::shared_ptr<function> dec_sub_function = std::make_shared<function>(sub_function_tok);
        dec_sub_function -> set_fqn(l_fqn);
        dec_sub_function -> is_public(true);
        dec_sub_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> dec_sub_scope = std::make_shared<scope>();
        dec_sub_scope -> set_parent(l_scope);
        dec_sub_function -> set_scope(dec_sub_scope);
        dec_sub_function -> add_param(param_one);
        dec_sub_function -> add_param(param_two);
        dec_sub_function -> set_return_type_instance(m_dec_instance);
        std::shared_ptr<decl> sub_function_decl = dec_sub_function;
        l_namespace -> add_declaration(sub_function_decl);

        // decimal multiplication
        std::shared_ptr<function> dec_mul_function = std::make_shared<function>(mul_function_tok);
        dec_mul_function -> set_fqn(l_fqn);
        dec_mul_function -> is_public(true);
        dec_mul_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> dec_mul_scope = std::make_shared<scope>();
        dec_mul_scope -> set_parent(l_scope);
        dec_mul_function -> set_scope(dec_mul_scope);
        dec_mul_function -> add_param(param_one);
        dec_mul_function -> add_param(param_two);
        dec_mul_function -> set_return_type_instance(m_dec_instance);
        std::shared_ptr<decl> mul_function_decl = dec_mul_function;
        l_namespace -> add_declaration(mul_function_decl);

        // decimal division
        std::shared_ptr<function> dec_div_function = std::make_shared<function>(div_function_tok);
        dec_div_function -> set_fqn(l_fqn);
        dec_div_function -> is_public(true);
        dec_div_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> dec_div_scope = std::make_shared<scope>();
        dec_div_scope -> set_parent(l_scope);
        dec_div_function -> set_scope(dec_div_scope);
        dec_div_function -> add_param(param_one);
        dec_div_function -> add_param(param_two);
        dec_div_function -> set_return_type_instance(m_dec_instance);
        std::shared_ptr<decl> div_function_decl = dec_div_function;
        l_namespace -> add_declaration(div_function_decl);

        // decimal hashing
        std::shared_ptr<function> dec_hash_function = std::make_shared<function>(hash_function_tok);
        dec_hash_function -> set_fqn(l_fqn);
        dec_hash_function -> is_public(true);
        dec_hash_function -> set_namespace(l_namespace -> get_name());
        std::shared_ptr<scope> dec_hash_scope = std::make_shared<scope>();
        dec_hash_scope -> set_parent(l_scope);
        dec_hash_function -> set_scope(dec_hash_scope);
        dec_hash_function -> add_param(param_one);
        dec_hash_function -> set_return_type_instance(m_dec_instance);
        std::shared_ptr<decl> hash_function_decl = dec_hash_function;
        l_namespace -> add_declaration(hash_function_decl);

        /* add the namespace to the program */
        std::shared_ptr<decl> namespace_decl = l_namespace;
        m_dec_prog.add_declaration(namespace_decl);

        return m_dec_prog;
    }
}
