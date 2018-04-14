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
    const std::shared_ptr<type>& avalon_dec::get_type() const {
        return m_dec_type;
    }

    /**
     * get_type_instance
     * returns the dec type instance
     */
    const type_instance& avalon_dec::get_type_instance() const {
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
        std::shared_ptr<decl> type_decl = m_dec_type;
        l_namespace -> add_declaration(type_decl);

        /* add function declarations to the namespace */
        // variables
        variable dec_param(var_one_tok, false);
        dec_param.set_type_instance(m_dec_instance);

        // unary positive
        std::shared_ptr<function> dec_pos_function = std::make_shared<function>(pos_function_tok);
        dec_pos_function -> add_param(dec_param);
        dec_pos_function -> set_return_type_instance(m_dec_instance);
        std::shared_ptr<decl> pos_function_decl = dec_pos_function;
        l_namespace -> add_declaration(pos_function_decl);

        // unary negative
        std::shared_ptr<function> dec_neg_function = std::make_shared<function>(neg_function_tok);
        dec_neg_function -> add_param(dec_param);
        dec_neg_function -> set_return_type_instance(m_dec_instance);
        std::shared_ptr<decl> neg_function_decl = dec_neg_function;
        l_namespace -> add_declaration(neg_function_decl);

        // decimal addition
        std::shared_ptr<function> dec_add_function = std::make_shared<function>(add_function_tok);
        dec_add_function -> add_param(dec_param);
        dec_add_function -> add_param(dec_param);
        dec_add_function -> set_return_type_instance(m_dec_instance);
        std::shared_ptr<decl> add_function_decl = dec_add_function;
        l_namespace -> add_declaration(add_function_decl);

        // decimal substraction
        std::shared_ptr<function> dec_sub_function = std::make_shared<function>(sub_function_tok);
        dec_sub_function -> add_param(dec_param);
        dec_sub_function -> add_param(dec_param);
        dec_sub_function -> set_return_type_instance(m_dec_instance);
        std::shared_ptr<decl> sub_function_decl = dec_sub_function;
        l_namespace -> add_declaration(sub_function_decl);

        // decimal multiplication
        std::shared_ptr<function> dec_mul_function = std::make_shared<function>(mul_function_tok);
        dec_mul_function -> add_param(dec_param);
        dec_mul_function -> add_param(dec_param);
        dec_mul_function -> set_return_type_instance(m_dec_instance);
        std::shared_ptr<decl> mul_function_decl = dec_mul_function;
        l_namespace -> add_declaration(mul_function_decl);

        // decimal division
        std::shared_ptr<function> dec_div_function = std::make_shared<function>(div_function_tok);
        dec_div_function -> add_param(dec_param);
        dec_div_function -> add_param(dec_param);
        dec_div_function -> set_return_type_instance(m_dec_instance);
        std::shared_ptr<decl> div_function_decl = dec_div_function;
        l_namespace -> add_declaration(div_function_decl);

        // decimal hashing
        std::shared_ptr<function> dec_hash_function = std::make_shared<function>(hash_function_tok);
        dec_hash_function -> add_param(dec_param);
        dec_hash_function -> set_return_type_instance(m_dec_instance);
        std::shared_ptr<decl> hash_function_decl = dec_hash_function;
        l_namespace -> add_declaration(hash_function_decl);

        /* add the namespace to the program */
        std::shared_ptr<decl> namespace_decl = l_namespace;
        m_dec_prog.add_declaration(namespace_decl);

        return m_dec_prog;
    }
}
