#include <memory>
#include <string>
#include <map>

#include "representer/mir/ast/expr/mir_match_record_constructor_expression.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with type source information
     */
    mir_match_record_constructor_expression::mir_match_record_constructor_expression(const std::string& name, token& tok) : m_name(name), m_tok(tok) {
    }

    /**
     * get_name
     * returns the name of theconstructor
     */
    const std::string& mir_match_record_constructor_expression::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_match_record_constructor_expression::get_token() const {
        return m_tok;
    }

    /**
     * add_argument
     * adds an argument to the function call
     */
    void mir_match_record_constructor_expression::add_argument(const std::string& arg_name, std::shared_ptr<mir_expr>& arg_val) {
        m_args.emplace(arg_name, arg_val);
    }

    /**
     * get_arguments
     * returns a map of all the arguments of the constructor
     */
    std::map<std::string, std::shared_ptr<mir_expr> >& mir_match_record_constructor_expression::get_arguments() {
        return m_args;
    }
}
