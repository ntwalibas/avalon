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
#include "representer/hir/builtins/avalon_float.hpp"


namespace avalon {
    /**
     * the default constructor expects nothing
     */
    avalon_float::avalon_float() : m_float_type(std::make_shared<type>(float_type_tok, VALID)), m_float_instance(float_type_tok, m_float_type, "*") {
    }

    /**
     * get_type
     * returns the int type
     */
    const std::shared_ptr<type>& avalon_float::get_type() const {
        return m_float_type;
    }

    /**
     * get_type_instance
     * returns the int type instance
     */
    const type_instance& avalon_float::get_type_instance() const {
        return m_float_instance;
    }

    /**
     * get_program
     * returns a program that defines the int type and all functions that operate on ints
     */
    program& avalon_float::get_program() {
        /* the program FQN */
        fqn l_fqn("__bifqn__", "__bifqn__");
        m_float_prog.set_fqn(l_fqn);

        /* the program's scope */
        std::shared_ptr<scope> l_scope = std::make_shared<scope>();
        l_scope -> set_start_line(0);
        l_scope -> set_end_line(0);
        m_float_prog.set_scope(l_scope);

        /* create the namespace to the program */
        std::shared_ptr<ns> l_namespace = std::make_shared<ns>(star_tok);
        l_namespace -> set_fqn(l_fqn);

        /* add the type to the namespace */
        std::shared_ptr<decl> type_decl = m_float_type;
        l_namespace -> add_declaration(type_decl);

        /* add function declarations to the namespace */
        // variables
        variable float_param(var_one_tok, false);
        float_param.set_type_instance(m_float_instance);

        // unary positive
        std::shared_ptr<function> float_pos_function = std::make_shared<function>(pos_function_tok);
        float_pos_function -> add_param(float_param);
        float_pos_function -> set_return_type_instance(m_float_instance);
        std::shared_ptr<decl> pos_function_decl = float_pos_function;
        l_namespace -> add_declaration(pos_function_decl);

        // unary negative
        std::shared_ptr<function> float_neg_function = std::make_shared<function>(neg_function_tok);
        float_neg_function -> add_param(float_param);
        float_neg_function -> set_return_type_instance(m_float_instance);
        std::shared_ptr<decl> neg_function_decl = float_neg_function;
        l_namespace -> add_declaration(neg_function_decl);

        // floating point addition
        std::shared_ptr<function> float_add_function = std::make_shared<function>(add_function_tok);
        float_add_function -> add_param(float_param);
        float_add_function -> add_param(float_param);
        float_add_function -> set_return_type_instance(m_float_instance);
        std::shared_ptr<decl> add_function_decl = float_add_function;
        l_namespace -> add_declaration(add_function_decl);

        // floating point substraction
        std::shared_ptr<function> float_sub_function = std::make_shared<function>(sub_function_tok);
        float_sub_function -> add_param(float_param);
        float_sub_function -> add_param(float_param);
        float_sub_function -> set_return_type_instance(m_float_instance);
        std::shared_ptr<decl> sub_function_decl = float_sub_function;
        l_namespace -> add_declaration(sub_function_decl);

        // floating point multiplication
        std::shared_ptr<function> float_mul_function = std::make_shared<function>(mul_function_tok);
        float_mul_function -> add_param(float_param);
        float_mul_function -> add_param(float_param);
        float_mul_function -> set_return_type_instance(m_float_instance);
        std::shared_ptr<decl> mul_function_decl = float_mul_function;
        l_namespace -> add_declaration(mul_function_decl);

        // floating point division
        std::shared_ptr<function> float_div_function = std::make_shared<function>(div_function_tok);
        float_div_function -> add_param(float_param);
        float_div_function -> add_param(float_param);
        float_div_function -> set_return_type_instance(m_float_instance);
        std::shared_ptr<decl> div_function_decl = float_div_function;
        l_namespace -> add_declaration(div_function_decl);

        // floating point hashing
        std::shared_ptr<function> float_hash_function = std::make_shared<function>(hash_function_tok);
        float_hash_function -> add_param(float_param);
        float_hash_function -> set_return_type_instance(m_float_instance);
        std::shared_ptr<decl> hash_function_decl = float_hash_function;
        l_namespace -> add_declaration(hash_function_decl);

        /* add the namespace to the program */
        std::shared_ptr<decl> namespace_decl = l_namespace;
        m_float_prog.add_declaration(namespace_decl);

        return m_float_prog;
    }
}
