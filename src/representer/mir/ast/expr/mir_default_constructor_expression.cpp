#include <memory>
#include <vector>
#include <string>

#include "representer/mir/ast/expr/mir_default_constructor_expression.hpp"
#include "representer/mir/ast/expr/mir_expr.hpp"
#include "representer/mir/ast/decl/mir_type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with type source information
     */
    mir_default_constructor_expression::mir_default_constructor_expression(const std::string& name, token& tok, mir_type& l_type) : m_name(name), m_tok(tok), m_type(l_type) {
    }

    /**
     * get_name
     * returns the name of theconstructor
     */
    const std::string& mir_default_constructor_expression::get_name() const {
        return m_name;
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& mir_default_constructor_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_type
     * returns the type of this expression
     */
    mir_type& mir_default_constructor_expression::get_type() {
        return m_type;
    }

    /**
     * add_argument
     * adds an argument to the function call
     */
    void mir_default_constructor_expression::add_argument(std::shared_ptr<mir_expr>& arg) {
        m_args.push_back(arg);
    }

    /**
     * get_arguments
     * returns a vector of all the arguments of the constructor
     */
    std::vector<std::shared_ptr<mir_expr> >& mir_default_constructor_expression::get_arguments() {
        return m_args;
    }
}
