#include <utility>
#include <memory>
#include <vector>
#include <string>

#include "representer/hir/ast/expr/call_expression.hpp"
#include "representer/hir/ast/expr/expr.hpp"
#include "representer/hir/ast/decl/type.hpp"
#include "lexer/token.hpp"


namespace avalon {
    /**
     * the constructor expects the token with type source information
     */
    call_expression::call_expression(token& tok) : m_tok(tok), m_name(tok.get_lexeme()), m_expr_type(FUNCTION_CALL_EXPR), m_type_instance_from_parser(false), m_return_type(star_instance) {
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
     * set_type_instance
     * sets the type instance of this expression
     */
    void call_expression::set_type_instance(type_instance& instance) {
        m_instance = instance;
    }

    /**
     * set_type_instance
     * sets the type instance of this expression with the possibility of specifying
     * if the parser (true case) set the type instance or the inference engine (false case)
     */
    void call_expression::set_type_instance(type_instance& instance, bool from_parser) {
        m_instance = instance;
        m_type_instance_from_parser = from_parser;
    }

    /**
     * get_type_instance
     * returns the type instance of this expression
     */
    type_instance& call_expression::get_type_instance() {
        return m_instance;
    }

    /**
     * has_type_instance
     * returns true if the type instance is not star
     */
    bool call_expression::has_type_instance() {
        if(m_instance.get_name() == "*")
            return false;
        else
            return true;
    }

    /**
     * type_instance_from_parser
     * returns true if the type instance was set by the parser
     * this will happen when the user specifies a type directly on an expression
     */
    bool call_expression::type_instance_from_parser() const {
        return m_type_instance_from_parser;
    }

    /*
     * set_expression_type
     * sets the type of expression this is
     */
    void call_expression::set_expression_type(call_expression_type expr_type) {
        m_expr_type = expr_type;
    }

    /**
     * get_expression_type
     * returns the type of expression we are dealing
     */
    const call_expression_type& call_expression::get_expression_type() const {
        return m_expr_type;
    }

    /**
     * add_specialization
     * add a type a type instance that corresponds to a constraint
     */
    void call_expression::add_specialization(type_instance& specialization) {
        m_specializations.push_back(specialization);
    }

    /**
     * get_specializations
     * returns a vector of type instances that will be used to specialize a function against constraints
     */
    std::vector<type_instance>& call_expression::get_specializations() {
        return m_specializations;
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

    /**
     * has_return_type_instance
     * returns true if this call expression has a return type instance specified
     */
    bool call_expression::has_return_type_instance() {
        if(m_return_type.get_name() == "*")
            return false;
        else
            return true;
    }

    /**
     * has_record_syntax
     * returns true if arguments named, false otherwise
     */
    bool call_expression::has_record_syntax() {
        if(m_arguments.size() == 0) {
            return false;
        }
        else {
            std::pair<token, std::shared_ptr<expr> >& first_argument = m_arguments[0];
            // we use the star token as a placeholder in case no argument was given
            // so its presence as argument token indicates that the user didn't provide any
            if(first_argument.first.get_lexeme() == "*")
                return false;
            else
                return true;
        }        
    }
}
