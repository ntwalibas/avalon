#include <utility>
#include <memory>
#include <vector>
#include <string>

#include "program/ast/expr/call_expression.hpp"
#include "program/ast/expr/expr.hpp"
#include "program/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with type source information
     */
    call_expression::call_expression(token& tok) : m_tok(tok), m_name(tok.get_lexeme()), m_return_type(type_instance(star_tok, "*")) {        
    }

    /**
     * get_token
     * returns a token with type source information
     */
    const token& call_expression::get_token() const {
        return m_tok;
    }

    /**
     * get_name
     * returns the name of the function to call
     */
    const std::string& call_expression::get_name() const {
        return m_name;
    }

    /**
     * add_argument
     * adds an argument to the function call
     */
    void call_expression::add_argument(token& arg_name, std::shared_ptr<expr>& arg_value) {
        m_arguments.emplace_back(arg_name, arg_value);
    }

    /**
     * get_arguments
     * returns a vector of all the arguments of the function call
     */
    std::vector<std::pair<token, std::shared_ptr<expr> > >& call_expression::get_arguments() {
        return m_arguments;
    }

    /**
     * set_return_type_instance
     * set the expected return type instance of the function that corresponds to this function call
     */
    void call_expression::set_return_type_instance(type_instance& return_type) {
        m_return_type = return_type;
    }

    /**
     * get_return_type_instance
     * returns the expected return type instance of the function that corresponds to this function call
     */
    type_instance& call_expression::get_return_type_instance() {
        return m_return_type;
    }
}
