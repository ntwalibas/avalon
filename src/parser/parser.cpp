#include <algorithm>
#include <stdexcept>
#include <cstddef>
#include <utility>
#include <memory>
#include <string>
#include <vector>
#include <map>

/* AST */
/* Program */
#include "program/ast/program.hpp"
/* Declarations */
#include "program/ast/decl/ns.hpp"
#include "program/ast/decl/type.hpp"
#include "program/ast/decl/decl.hpp"
#include "program/ast/decl/import.hpp"
#include "program/ast/decl/function.hpp"
#include "program/ast/decl/variable.hpp"
#include "program/ast/decl/statement.hpp"
#include "program/ast/exceptions/type_error.hpp"
/* Statements */
#include "program/ast/stmt/stmt.hpp"
#include "program/ast/stmt/if_stmt.hpp"
#include "program/ast/stmt/for_stmt.hpp"
#include "program/ast/stmt/pass_stmt.hpp"
#include "program/ast/stmt/while_stmt.hpp"
#include "program/ast/stmt/block_stmt.hpp"
#include "program/ast/stmt/break_stmt.hpp"
#include "program/ast/stmt/switch_stmt.hpp"
#include "program/ast/stmt/return_stmt.hpp"
#include "program/ast/stmt/continue_stmt.hpp"
#include "program/ast/stmt/expression_stmt.hpp"
/* Expressions */
#include "program/ast/expr/expr.hpp"
#include "program/ast/expr/cast_expression.hpp"
#include "program/ast/expr/call_expression.hpp"
#include "program/ast/expr/unary_expression.hpp"
#include "program/ast/expr/tuple_expression.hpp"
#include "program/ast/expr/binary_expression.hpp"
#include "program/ast/expr/grouped_expression.hpp"
#include "program/ast/expr/literal_expression.hpp"
#include "program/ast/expr/identifier_expression.hpp"
#include "program/ast/expr/assignment_expression.hpp"
#include "program/ast/expr/underscore_expression.hpp"
/* Symbol table */
#include "program/symtable/fqn.hpp"
#include "program/symtable/scope.hpp"
#include "program/symtable/gtable.hpp"

/* Lexer */
#include "lexer/token.hpp"
#include "lexer/token_type.hpp"

/* Error */
#include "error/error.hpp"

/* Parser */
#include "parser/parser.hpp"


namespace avalon {
/**
 * the contructor expects:
 * - the error handler to be used to show errors
 * - the token where the error occured
 * - whether the error is fatal or not
 * - the message to show the user
 */
parse_error::parse_error(error& error_handler, token tok, bool fatal, const std::string& message) : std::runtime_error(message), m_error_handler(error_handler), m_tok(tok), m_fatal(fatal) {
}
    
    /**
     * what
     * get the error message
     */
    const char * parse_error::what() const noexcept {
        return std::runtime_error::what();
    }

    /**
     * is_fatal
     * returns true if the error is fatal, false otherwise
     */
    bool parse_error::is_fatal() {
        return m_fatal;
    }

    /**
     * show
     * calls the error handler error reporting function to display the error
     */
    void parse_error::show() {
        m_error_handler.log(m_tok . get_line(), m_tok . get_column(), what());
    }


parser::parser(
    std::vector<std::shared_ptr<token> >& tokens, const std::string& source_path, error& error_handler
) : m_tokens(tokens), m_source_path(source_path), m_error_handler(error_handler), m_current(0)
{
}
    
    /**
     * parse
     * the parser entry point
     * it constructs the program by calling the appropriate functions
     */
    program& parser::parse() {
        return prog();
    }

    /**
     * program
     * this function loops over the token stream until it reaches the end
     */
    program& parser::prog() {
        // set the fqn this program lives
        fqn l_fqn = fqn::from_path(m_source_path);
        m_program.set_fqn(l_fqn);

        // set the scope of this program
        std::shared_ptr<scope> l_scope = std::make_shared<scope>();
        l_scope -> set_start_line(1);
        m_program.set_scope(l_scope);

        while(!is_at_end()) {
            // if we get an error here, then it was fatal,
            // we print the error message and stop parsing
            try{
                declaration(l_scope);
            } catch(parse_error& err) {
                err.show();
                return m_program;
            }
        }

        if(!is_at_end())
            throw parsing_error(true, peek(), "Expected to be at the end of the program.");
        else
            l_scope -> set_end_line(lookback() -> get_line());

        return m_program;
    }

    /**
     * declaration
     * a program is a series of declarations:
     * - import declaration
     * - namespace declaration
     *
     * this function decides which type of declaration we are dealing with
     * and calls the appropriate subroutine to parse that declaration.
     */
    void parser::declaration(std::shared_ptr<scope>& parent_scope) {
        try {
            if(match(IMPORT)) {
                import_declaration();
            }
            else if(match(NAMESPACE)) {
                namespace_declaration(parent_scope);
            }
            else {
                throw parsing_error(true, lookback(), "Expected an import declaration or a namespace declaration.");
            }
        } catch(parse_error& err) {
            // if the error is fatal, we bubble the exception up
            if(err.is_fatal())
                throw err;
            // else, we show the error then synchronize
            else
                err.show();

            // synchronize
            synchronize();
        }
    }

    /**
     * import_declaration
     * if the declaration function matches the "import" keyword, then this function
     * continues parsing in anticipation of a full import declaration.
     */
    void parser::import_declaration() {
        std::shared_ptr<token>& import_tok = lookback();
        std::string fqn_name;

        // build the fqn name
        do {
            std::shared_ptr<token>& path_tok = consume(IDENTIFIER, "Expected an identifer in fully qualified name.");
            fqn_name.append(path_tok -> get_lexeme());
            if(check(DOT))
                fqn_name.push_back('.');                
        } while(match(DOT));

        // we now construct the import declaration
        std::shared_ptr<import> import_decl = std::make_shared<import>(* import_tok, fqn_name);

        // there must be a new line after each import declaration
        consume(NEWLINE, "Expected a new line after an import declaration.");

        // add the import declaration to the program
        std::shared_ptr<decl> declaration = import_decl;
        m_program.add_declaration(declaration);
    }

    /**
     * namespace declaration
     * if the declaration function matches the "namespace" keyword, then this function
     * continues parsing in anticipation of type, function, variable and statement declarations
     */
    void parser::namespace_declaration(std::shared_ptr<scope>& parent_scope) {
        if(!check(MUL) && !check(IDENTIFIER))
            throw parsing_error(true, peek(), "Expected the namespace name as a star (for the global namespace) or an identifer for a named namespace.");

        // the namespace name
        std::shared_ptr<token>& namespace_tok = advance();
        std::shared_ptr<ns> namespace_decl = std::make_shared<ns>(* namespace_tok);
        std::vector<std::shared_ptr<decl> > top_decls;
        bool indent_found = false;

        // we expect an opening parenthesis and a newline after the namespace name
        consume(NS_LEFT_PAREN, "Expected a namespace opening parenthesis after the namespace name");
        consume(NEWLINE, "Expected a newline after the opening parenthesis in namespace declaration.");
        if(match(INDENT))
            indent_found = true;

        // get the declarations defined in this scope
        while(((indent_found & !check(DEDENT)) || (!indent_found & !check(NS_RIGHT_PAREN))) && !is_at_end()) {
            /// get the declarations
            if(match(PUBLIC)) {
                top_decls = top_declaration(true, parent_scope);
            }
            else if(match(PRIVATE)) {
                top_decls = top_declaration(false, parent_scope);
            }
            else {
                top_decls = top_declaration(true, parent_scope);
            }

            // add them to the namespace
            for(auto& top_decl : top_decls)
                namespace_decl -> add_declaration(top_decl);
        }

        // if we had an identation, we expect a dedentation
        if(indent_found)
            consume(DEDENT, "Expected a dedentation in namespace declaration");

        // we expect a closing parenthesis and a newline to close a namespace declaration
        consume(NS_RIGHT_PAREN, "Expected a namespace closing parenthesis to close a namespace declaration.");
        consume(NEWLINE, "Expected a new line after a namespace declaration.");

        // add the namespace declaration to the program
        std::shared_ptr<decl> declaration = namespace_decl;
        m_program.add_declaration(declaration);
    }

    /**
     * top_declaration
     * types, functions and variables form the top declarations that can occur within a namespace
     * this function detect if we have any of them then calls the appropriate parsing functions
     */
    std::vector<std::shared_ptr<decl> > parser::top_declaration(bool is_public, std::shared_ptr<scope>& parent_scope) {        
        if(match(TYPE)) {
            std::vector<std::shared_ptr<decl> > type_decls;
            std::shared_ptr<decl> type_decl = type_declaration(is_public);
            type_decls.push_back(type_decl);
            return type_decls;
        }
        else if(match(FUNCTION)) {
            std::vector<std::shared_ptr<decl> > fun_decls;
            std::shared_ptr<decl> fun_decl = function_declaration(is_public, parent_scope);
            fun_decls.push_back(fun_decl);
            return fun_decls;
        }
        else if(match(VAR) || match(VAL)) {
            std::vector<std::shared_ptr<decl> > var_decls = variable_declaration(is_public);
            return var_decls;
        }
        else {
            std::vector<std::shared_ptr<decl> > stmt_decls;
            std::shared_ptr<decl> stmt_decl = statement_declaration(parent_scope);
            stmt_decls.push_back(stmt_decl);
            return stmt_decls;            
        }
    }

    /**
     * type_declaration
     * if the declaration function matches the "type" keyword, then this function
     * continues the parsing process in anticipation of a full type declaration.
     */
    std::shared_ptr<decl> parser::type_declaration(bool is_public) {
        std::shared_ptr<token>& type_tok = consume(IDENTIFIER, "Expected the type name in a type declaration.");
        std::shared_ptr<type> type_decl = std::make_shared<type>(* type_tok, UNKNOWN);
        type_decl -> is_public(is_public);

        if(match(EQUAL)) {
            // we expect parenthesis around type parameters (whether they were provided or not)
            consume(LEFT_PAREN, "Expected an opening parenthesis before possible type parameters.");
            type_parameters(type_decl);
            consume(RIGHT_PAREN, "Expected an closing parenthesis after possible type parameters.");
            consume(COLON, "Expected a colon after the closing parenthesis near type parameters.");

            // we expect a new line and an indentation before type constructors
            consume(NEWLINE, "Expected new line after type parameters.");
            consume(INDENT, "Expected an indentation before specifying constructors.");
            type_constructors(type_decl);
            consume(DEDENT, "Expected a dedentation after constructors.");
        }
        else {
            consume(NEWLINE, "Expected a new line after type declaration.");
        }

        // add the type declaration to the program
        std::shared_ptr<decl> declaration = type_decl;
        return declaration;
    }

    /**
     * type_parameters
     * if a type depends of type parameters, this function identifies them,
     * makes sure they are unique within the type and they don't match existing types
     */
    void parser::type_parameters(std::shared_ptr<type>& type_decl) {
        if(!is_at_end() && check(IDENTIFIER)) {
            do {
                std::shared_ptr<token>& type_param = consume(IDENTIFIER, "Expected type parameter.");
                type_decl -> add_param(* type_param);
            } while(match(COMMA));
        }
    }

    /**
     * type_constructors
     * if a type has type constructors, this function identifies what kind of constructor they are
     * and calls the appropriate function to complete said constructors.
     */
    void parser::type_constructors(std::shared_ptr<type>& type_decl) {
        do {
            std::shared_ptr<token>& constructor_tok = consume(IDENTIFIER, "Expected a constructor name in type declaration.");

            // we have the constructor name, now we handle paramters                
            if(match(LEFT_PAREN)) {
                // in anticipation of using peek()
                if(is_at_end())
                    throw parsing_error(true, lookback(), "Unexpected end of program in the middle of a type declaration.");
                else {
                    // in anticipation of using peek_next()
                    if(is_past_end())
                        throw parsing_error(true, lookback(), "Unexpected end of program in the middle of a type declaration.");
                }

                // decide if we have a default constructor or one for the record syntax
                if(check(RIGHT_PAREN)) {
                    parse_default_constructor(constructor_tok, type_decl);
                }
                // if the next token if an identifer instead, then we decide if we have a default constructor or record constructor
                else if(check(IDENTIFIER)) {
                    // if the token after the next token is a colon then we have a record constructor
                    if(check_next(COLON)) {
                        parse_record_constructor(constructor_tok, type_decl);
                    }
                    // if we don't see a colon after the first identifier, then we have a default constructor
                    else {
                        parse_default_constructor(constructor_tok, type_decl);
                    }
                }
                // anything else is an error
                else {
                    throw parsing_error(true, peek(), "Expected a closing parenthesis or a constructor parameter.");
                }

                // we are done building cunstructors, we expect a closing parenthesis
                consume(RIGHT_PAREN, "Expected a closing parenthesis after constructor arguments.");
            }
            else {
                parse_default_constructor(constructor_tok, type_decl);
            }

            // we expect a new line after every constructor
            consume(NEWLINE, "Expected a new line after constructor and parameters.");
        } while(!is_at_end() && match(VERTICAL_BAR));
    }

    /**
     * parse_default_constructor
     * this function parses a default constructors, makes sure it unique within this parsing unit,
     * adds it to the type declaration and add it to the constructor symbol table.
     */
    void parser::parse_default_constructor(std::shared_ptr<token>& constructor_tok, std::shared_ptr<type>& type_decl) {
        default_constructor def_constructor(* constructor_tok, type_decl);

        // we look for constructor arguments only if it was signaled by a opening parenthesis (before we got here)
        if(check_previous(LEFT_PAREN)) {
            do {
                type_instance param_type = parse_type_instance();
                def_constructor.add_param(param_type);
            } while(match(COMMA) && !is_at_end());
        }

        // add the constructor to the type declaration
        type_decl -> add_constructor(def_constructor);
    }

    /**
     * parse_record_constructor
     * this function parses a record constructors, makes sure it unique within this parsing unit,
     * adds it to the type declaration and add it to the constructor symbol table.
     */
    void parser::parse_record_constructor(std::shared_ptr<token>& constructor_tok, std::shared_ptr<type>& type_decl) {
        record_constructor rec_constructor(* constructor_tok, type_decl);

        do {
            std::shared_ptr<token>& param_name = consume(IDENTIFIER, "Expected the parameter name in constructor with record syntax.");
            consume(COLON, "Expected a colon after the parameter name in constructor with record syntax.");
            type_instance param_type = parse_type_instance();
            rec_constructor.add_param(* param_name, param_type);
        } while(!is_at_end() && match(COMMA));

        // add the constructor to the type declaration
        type_decl -> add_constructor(rec_constructor);
    }

    /**
     * function_declaration
     * if the declaration function matches the "function" keyword, then this function
     * continues the parsing process in anticipation of a full function declaration.
     */
    std::shared_ptr<decl> parser::function_declaration(bool is_public, std::shared_ptr<scope>& parent_scope) {
        std::shared_ptr<token>& function_tok = consume(IDENTIFIER, "Expected the function name in a function declaration.");
        std::shared_ptr<function> function_decl = std::make_shared<function>(* function_tok);
        function_decl -> is_public(is_public);

        // set the scope introduced by this function
        std::shared_ptr<scope> l_scope = std::make_shared<scope>();
        l_scope -> set_start_line(function_tok -> get_line());
        l_scope -> set_parent(parent_scope);
        function_decl -> set_scope(l_scope);

        // parse function parameter constraints, if any
        if(match(COLON))
            parse_parameters_constraints(function_decl);

        consume(EQUAL, "Expected the equal sign in anticipation of function parameters");

        // parse function parameters
        parse_parameters(function_decl);

        // parse the return type
        consume(RETURN_TYPE, "Expected the return type of the function.");
        type_instance return_type_instance = parse_type_instance();
        function_decl -> set_return_type_instance(return_type_instance);
        consume(COLON, "Excepted a colon after the function's return type.");
        consume(NEWLINE, "Expected a new line after the function return type.");

        // get the function body
        block_stmt body = block_statement(parent_scope); 
        function_decl -> set_body(body);

        // set the the end of the current scope
        l_scope -> set_end_line(lookback() -> get_line());

        // we add the function to the ast
        std::shared_ptr<decl> declaration = function_decl;
        return declaration;
    }

    /**
     * parse_parameters_constraints
     * parse the parameters a function depends on and add them to the function
     */
    void parser::parse_parameters_constraints(std::shared_ptr<function>& function_decl) {
        do {
            std::shared_ptr<token>& constraint_tok = consume(IDENTIFIER, "Expected the function constraint name.");
            function_decl -> add_constraint(* constraint_tok);
        } while(match(COMMA));
    }

    /**
     * parse_parameters
     * parse the function's parameters
     */
    void parser::parse_parameters(std::shared_ptr<function>& function_decl) {
        consume(LEFT_PAREN, "Expected an opening parenthesis before function parameters.");

        if(check(IDENTIFIER) || check(VAR) || check(VAL)) {
            do {
                // get the mutability specifier of the parameter
                bool is_mutable = false;
                if(match(VAR))
                    is_mutable = true;
                else if(match(VAL))
                    is_mutable = false;

                // get the parameter token
                std::shared_ptr<token>& param_tok = consume(IDENTIFIER, "Expected the function parameter name.");
                
                // a parameter is simply a variable declared inside a the function parameters list
                variable param(* param_tok, is_mutable);
                // if we match a colon then we can expect the parameter type instance
                consume(COLON, "Expected a colon in anticipation of the function parameter type.");
                type_instance param_type_instance = parse_type_instance();
                param.set_type_instance(param_type_instance);

                // we are done parsing the parameter, we add it to the function
                function_decl -> add_param(param);
            } while(match(COMMA));
        }

        consume(RIGHT_PAREN, "Expected a closing parenthesis after function parameters.");
    }

    /**
     * variable_declaration
     * if the declaration function matches the "var" or "val" keywords, then this function
     * continues the parsing process in anticipation of a full variable declaration.
     */
    std::vector<std::shared_ptr<decl> > parser::variable_declaration(bool is_public) {
        std::vector<std::shared_ptr<decl> > var_decls;        
        bool is_first_def = true;
        bool indent_found = false;
        bool is_mutable = false;
        if(check_previous(VAR))
            is_mutable = true;

        do {
            // it is possible that the next definition was put on a new line so we expect a NEWLINE and INDENT tokens to be generated
            // but we do not allow this for the first definition
            if (check(NEWLINE)) {
                if (is_first_def) {
                    throw parsing_error(false, lookback(), "The first variable definition must come immediately after the mutability specifier.");
                }
                else {
                    // consume the new line
                    advance();
                    // an INDENT token will be generated only after the first line
                    if(check(INDENT)) {
                        advance(); // consume the indentation
                        indent_found = true;
                    }

                    // if instead we found a dedentation, we consume it and let the user continue writing ugly code
                    if (check(DEDENT)) {
                        if(indent_found) {
                            advance();
                            indent_found = false;
                        }
                        // if there was no previous indentation, this is an error because it appears the user is leaving a block
                        else {
                            throw parsing_error(false, peek(), "Unexpected dedentation. It appears you are leaving a block during variable definitions which is not allowed.");
                        }
                    }
                }
            }

            // we clear the is_first_def flag
            is_first_def = false;            

            // get the variable name and construct the variable
            std::shared_ptr<token>& var_tok = consume(IDENTIFIER, "Expected a variable name.");
            std::shared_ptr<variable> var_decl = std::make_shared<variable>(* var_tok, is_mutable);
            var_decl -> is_public(is_public);

            // get the type if any
            if(match(COLON)) {
                type_instance var_type_instance = parse_type_instance();
                var_decl -> set_type_instance(var_type_instance);
            }

            // get the initializer if any
            if(match(EQUAL)) {
                std::shared_ptr<expr> val = parse_expression();
                var_decl -> set_value(val);
            }

            // add the type declaration to the program
            std::shared_ptr<decl> declaration = var_decl;
            var_decls.push_back(declaration);
        } while(match(COMMA));

        // we expect a new line after variable definitions
        consume(NEWLINE, "Expected a new line after variable definition(s).");

        // if an indentation was done by the user, we consume a DEDENT token here
        if (indent_found)
            consume(DEDENT, "Expected a dedentation after variable definition(s).");

        return var_decls;
    }

    /**
     * statement_declaration
     * when we can't any of the existing declarations, then all we have is either a statement declaration
     * or a malformed expression.
     */
    std::shared_ptr<decl>  parser::statement_declaration(std::shared_ptr<scope>& parent_scope) {
        std::shared_ptr<statement> statement_decl = std::make_shared<statement>();
        std::shared_ptr<stmt> statement = nullptr;

        if(match(SWITCH)) {
            statement = switch_statement(parent_scope);
        }
        else if(match(WHILE)) {
            statement = while_statement(parent_scope);
        }
        else if(match(IF)) {
            statement = if_statement(parent_scope);
        }
        else if(match(FOR)) {
            statement = for_statement(parent_scope);
        }
        else if(match(BREAK)) {
            statement = break_statement();
        }
        else if(match(CONTINUE)) {
            statement = continue_statement();
        }
        else if(match(PASS)) {
            statement = pass_statement();
        }
        else if(match(RETURN)) {
            statement = return_statement();
        }
        else {
            statement = expression_statement();
        }

        statement_decl -> set_statement(statement);
        std::shared_ptr<decl> declaration = statement_decl;
        return declaration;
    }

    /**
     * switch_statement
     * match a switch statement
     */
    std::shared_ptr<stmt> parser::switch_statement(std::shared_ptr<scope>& parent_scope) {
        std::shared_ptr<token>& tok = lookback();
        std::shared_ptr<switch_stmt> sw_stmt = std::make_shared<switch_stmt>(* tok);

        // we get switch parameters which are essentially variable expressions to match against
        bool has_paren = false;
        if(match(LEFT_PAREN))
            has_paren = true;

        do {
            std::shared_ptr<expr> param = parse_expression();
            sw_stmt -> add_param(param);
        } while(match(COMMA));

        if(has_paren == true)
            consume(RIGHT_PAREN, "Expected a closing parenthesis after switch parameters.");

        // we expect a colon followed by a newline and an indent
        consume(COLON, "Expected a colon after switch parameters.");

        bool is_indented = false;
        consume(NEWLINE, "Expected a new line after switch parameters and the colon.");
        if(check(INDENT)) {            
            consume(INDENT, "Expected an indentation before cases in switch statement after the newline.");
            is_indented = true;
        }        

        // we handle cases
        if(check(CASE) == false)
            throw parsing_error(true, peek(), "Expected at least one case in switch statement.");

        while(match(CASE) && !is_at_end()) {
            std::shared_ptr<token>& case_tok = lookback();
            switch_case cas(* case_tok);

            // set the scope introduced by this case
            std::shared_ptr<scope> l_scope = std::make_shared<scope>();
            l_scope -> set_start_line(case_tok -> get_line());
            l_scope -> set_parent(parent_scope);
            cas.set_scope(l_scope);

            bool case_has_paren = false;
            if(match(LEFT_PAREN))
                case_has_paren = true;

            do {
                std::shared_ptr<expr> match = parse_expression();
                cas.add_match(match);
            } while(match(COMMA));

            if(case_has_paren == true)
                consume(RIGHT_PAREN, "Expected a closing parenthesis after case possible matches.");

            // we expect a colon followed by a newline and an indent before statements
            consume(COLON, "Expected a colon after case matches.");
            consume(NEWLINE, "Expected a new line after case matches and the colon.");

            // if we get an indentation, then we can expect a statement
            if(check(INDENT)) {
                block_stmt case_block = block_statement(parent_scope); 
                cas.set_block(case_block);
            }
            else {
                block_stmt case_block;
                cas.set_block(case_block);
            }

            // set the end line for the new scope and return to the old scope
            l_scope -> set_end_line(lookback() -> get_line());

            // add the case to the switch statement
            sw_stmt -> add_case(cas);
        }

        // we handle default
        if(match(DEFAULT)) {
            std::shared_ptr<token>& def_tok = lookback();
            switch_default def(* def_tok);

            // default creates a new scope
            std::shared_ptr<scope> l_scope = std::make_shared<scope>();
            l_scope -> set_start_line(def_tok -> get_line());
            l_scope -> set_parent(parent_scope);
            def.set_scope(l_scope);

            consume(COLON, "Expected a colon after default case.");
            consume(NEWLINE, "Expected a new line after default case and the colon.");

            if(!check(INDENT))
                throw parsing_error(true, def_tok, "The default case of a switch statement requires a block statement.");

            // get and set the default case block
            block_stmt def_block = block_statement(parent_scope);            
            def.set_block(def_block);

            // set the end line for the new scope
            l_scope -> set_end_line(lookback() -> get_line());

            // add the default case to the switch statement
            sw_stmt -> set_default(def);
        }

        if(is_indented == true)
            consume(DEDENT, "Expected a dedentation after switch cases and default.");

        std::shared_ptr<stmt> stmt = sw_stmt;
        return stmt;
    }

    /**
     * while_statement
     * match a while statement
     */
    std::shared_ptr<stmt> parser::while_statement(std::shared_ptr<scope>& parent_scope) {
        std::shared_ptr<token>& tok = lookback();
        std::shared_ptr<while_stmt> whl_stmt = std::make_shared<while_stmt>(* tok);

        // a while loop introduces a new scope
        std::shared_ptr<scope> l_scope = std::make_shared<scope>();
        l_scope -> set_start_line(tok -> get_line());
        l_scope -> set_parent(parent_scope);
        whl_stmt -> set_scope(l_scope);

        // parse the while loop condition
        bool has_paren = false;
        if(match(LEFT_PAREN))
            has_paren = true;

        std::shared_ptr<expr> condition = parse_expression();
        whl_stmt -> set_condition(condition);

        if(has_paren)
            consume(RIGHT_PAREN, "Expected a closing parenthesis after while condition.");

        consume(COLON, "Expected a colon after while loop condition.");
        consume(NEWLINE, "Expected a new line after the colon in while loop.");

        if(!check(INDENT))
            throw parsing_error(true, tok, "A while loop must have a body.");

        // get the body of the loop
        block_stmt body = block_statement(parent_scope);
        whl_stmt -> set_block(body);

        // set the end line for the new scope
        l_scope -> set_end_line(lookback() -> get_line());

        std::shared_ptr<stmt> stmt = whl_stmt;
        return stmt;
    }

    /**
     * if_statement
     * match an if statement
     */
    std::shared_ptr<stmt> parser::if_statement(std::shared_ptr<scope>& parent_scope) {
        std::shared_ptr<token>& tok = lookback();
        std::shared_ptr<if_stmt> f_stmt = std::make_shared<if_stmt>(* tok);

        // an if statement introduces a new scope
        std::shared_ptr<scope> f_scope = std::make_shared<scope>();
        f_scope -> set_start_line(tok -> get_line());
        f_scope -> set_parent(parent_scope);
        f_stmt -> set_scope(f_scope);

        bool has_paren = false;
        if(match(LEFT_PAREN))
            has_paren = true;

        std::shared_ptr<expr> condition = parse_expression();
        f_stmt -> set_condition(condition);

        if(has_paren)
            consume(RIGHT_PAREN, "Expected a closing parenthesis after if condition.");

        consume(COLON, "Expected a colon after if statement condition.");
        consume(NEWLINE, "Expected a new line after the colon in if statement.");

        if(!check(INDENT))
            throw parsing_error(true, tok, "An if statement must have a body.");

        block_stmt body = block_statement(parent_scope);
        f_stmt -> set_block(body);

        // set the end line for the new scope introduced by the if statement
        f_scope -> set_end_line(lookback() -> get_line());

        // try to parse additional elif statements if any
        if(check(ELIF)) {
            while(match(ELIF) && !is_at_end()) {
                std::shared_ptr<token>& tok = lookback();
                elif_branch elf_branch(* tok);

                // an elif branch introduces a new scope
                std::shared_ptr<scope> elf_scope = std::make_shared<scope>();
                elf_scope -> set_start_line(tok -> get_line());
                elf_scope -> set_parent(parent_scope);
                elf_branch.set_scope(elf_scope);

                bool has_paren = false;
                if(match(LEFT_PAREN))
                    has_paren = true;

                std::shared_ptr<expr> condition = parse_expression();
                elf_branch.set_condition(condition);

                if(has_paren)
                    consume(RIGHT_PAREN, "Expected a closing parenthesis after elif condition.");

                consume(COLON, "Expected a colon after elif branch condition.");
                consume(NEWLINE, "Expected a new line after the colon in elif branch.");

                if(!check(INDENT))
                    throw parsing_error(true, tok, "An elif branch must have a body.");

                block_stmt body = block_statement(parent_scope);
                elf_branch.set_block(body);

                // set the end line for the new scope introduced by the elif branch
                elf_scope -> set_end_line(lookback() -> get_line());

                f_stmt -> add_elif(elf_branch);
            }
        }

        // try to match an else branch
        if(match(ELSE)) {
            std::shared_ptr<token>& tok = lookback();
            else_branch els_branch(* tok);

            // an else branch introduces a new scope
            std::shared_ptr<scope> els_scope = std::make_shared<scope>();
            els_scope -> set_start_line(tok -> get_line());
            els_scope -> set_parent(parent_scope);
            els_branch.set_scope(els_scope);

            consume(COLON, "Expected a colon after else branch.");
            consume(NEWLINE, "Expected a new line after the colon in else branch.");

            if(!check(INDENT))
                throw parsing_error(true, tok, "An else branch must have a body.");

            block_stmt body = block_statement(parent_scope);
            els_branch.set_block(body);

            // set the end line for the new scope introduced by the else branch
            els_scope -> set_end_line(lookback() -> get_line());

            f_stmt -> set_else(els_branch);
        }

        std::shared_ptr<stmt> stmt = f_stmt;
        return stmt;
    }

    /**
     * for_statement
     * match a for statement
     */
    std::shared_ptr<stmt> parser::for_statement(std::shared_ptr<scope>& parent_scope) {
        std::shared_ptr<token>& tok = lookback();
        std::shared_ptr<for_stmt> fr_stmt = std::make_shared<for_stmt>(* tok);

        // a for loop introduces a new scope
        std::shared_ptr<scope> fr_scope = std::make_shared<scope>();
        fr_scope -> set_start_line(tok -> get_line());
        fr_scope -> set_parent(parent_scope);
        fr_stmt -> set_scope(fr_scope);

        // parse the for loop iteration expression
        bool has_paren = false;
        if(match(LEFT_PAREN))
            has_paren = true;

        std::shared_ptr<expr> iteration = parse_expression();
        fr_stmt -> set_iteration(iteration);

        if(has_paren)
            consume(RIGHT_PAREN, "Expected a closing parenthesis after for iteration expression.");

        consume(COLON, "Expected a colon after for loop iteration expression.");
        consume(NEWLINE, "Expected a new line after the colon in for loop.");

        if(!check(INDENT))
            throw parsing_error(true, tok, "A for loop must have a body.");

        // get the body of the loop
        block_stmt body = block_statement(parent_scope);
        fr_stmt -> set_block(body);

        // set the end line for the new scope
        fr_scope -> set_end_line(lookback() -> get_line());        

        // try to match an empty branch
        if(match(EMPTY)) {
            std::shared_ptr<token>& tok = lookback();
            empty_branch ept_branch(* tok);

            // an empty branch introduces a new scope
            std::shared_ptr<scope> ept_scope = std::make_shared<scope>();
            ept_scope -> set_start_line(tok -> get_line());
            ept_scope -> set_parent(parent_scope);
            ept_branch.set_scope(ept_scope);

            consume(COLON, "Expected a colon after empty branch.");
            consume(NEWLINE, "Expected a new line after the colon in empty branch.");

            if(!check(INDENT))
                throw parsing_error(true, tok, "An empty branch must have a body.");

            block_stmt body = block_statement(parent_scope);
            ept_branch.set_block(body);

            // set the end line for the new scope introduced by the else branch
            ept_scope -> set_end_line(lookback() -> get_line());

            fr_stmt -> set_empty(ept_branch);
        }

        std::shared_ptr<stmt> stmt = fr_stmt;
        return stmt;
    }

    /**
     * break_statement
     * match a break statement
     */
    std::shared_ptr<stmt> parser::break_statement() {
        std::shared_ptr<token>& break_tok = lookback();
        
        std::shared_ptr<break_stmt> brk_stmt = std::make_shared<break_stmt>(* break_tok);
        consume(NEWLINE, "Expected a new line after break statement.");

        std::shared_ptr<stmt> stmt = brk_stmt;
        return stmt;
    }

    /**
     * continue_statement
     * match a continue statement
     */
    std::shared_ptr<stmt> parser::continue_statement() {
        std::shared_ptr<token>& continue_tok = lookback();
        
        std::shared_ptr<continue_stmt> cnt_stmt = std::make_shared<continue_stmt>(* continue_tok);
        consume(NEWLINE, "Expected a new line after continue statement.");

        std::shared_ptr<stmt> stmt = cnt_stmt;
        return stmt;
    }

    /**
     * pass_statement
     * match a pass statement
     */
    std::shared_ptr<stmt> parser::pass_statement() {
        std::shared_ptr<token>& pass_tok = lookback();
        
        std::shared_ptr<pass_stmt> pss_stmt = std::make_shared<pass_stmt>(* pass_tok);
        consume(NEWLINE, "Expected a new line after pass statement.");

        std::shared_ptr<stmt> stmt = pss_stmt;
        return stmt;
    }

    /**
     * return_statement
     * match a return statement
     */
    std::shared_ptr<stmt> parser::return_statement() {
        std::shared_ptr<token>& ret_tok = lookback();
        
        std::shared_ptr<return_stmt> ret_stmt = std::make_shared<return_stmt>(* ret_tok);
        // if the upcoming token is token is not a new line, then we have an expression to be returned
        if(check(NEWLINE) == false) {
            std::shared_ptr<expr> ret_expression = parse_expression();
            ret_stmt -> set_expression(ret_expression);
        }
        consume(NEWLINE, "Expected a new line after return statement.");

        std::shared_ptr<stmt> stmt = ret_stmt;
        return stmt;
    }

    /**
     * parse_block
     * parses a series a statement to form a block
     */
    block_stmt parser::block_statement(std::shared_ptr<scope>& parent_scope) {
        consume(INDENT, "Expected indentation before entering a block.");
        block_stmt blc;

        while(!check(DEDENT) && !is_at_end()) {
            if(match(INDENT))
                throw parsing_error(true, lookback(), "Unexpected indentation in block.");

            std::vector<std::shared_ptr<decl> > top_decls = top_declaration(false, parent_scope);
            for(auto& top_decl : top_decls)
                blc.add_declaration(top_decl);
        }
        consume(DEDENT, "Expected a dedentation before exiting block.");

        return blc;
    }

    /**
     * expression_statement
     * macthes a expression followed by a new line
     */
    std::shared_ptr<stmt> parser::expression_statement() {
        std::shared_ptr<expression_stmt> expr_stmt = std::make_shared<expression_stmt>();
        std::shared_ptr<expr> l_expression = nullptr;

        l_expression = parse_expression();
        consume(NEWLINE, "Expected a new line after an expression statement.");

        expr_stmt -> set_expression(l_expression);
        std::shared_ptr<stmt> stmt = expr_stmt;
        return stmt;
    }

    /**
     * parse_expression
     * matches an expression
     */
    std::shared_ptr<expr> parser::parse_expression() {
        return assignment();
    }

    /**
     * assignment
     * matches an assignment
     * an assigment can be a construction assignment where a variable is given a value
     * or a deconstruction assignment where the content of a variable is split apart and
     * some of that content put into another variable.
     */
    std::shared_ptr<expr> parser::assignment() {
        std::shared_ptr<expr> l_expression = nullptr;
        std::shared_ptr<expr> lval = logical_or();

        while(match(EQUAL)) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> rval = assignment();
            std::shared_ptr<assignment_expression> expr = std::make_shared<assignment_expression>(* op, lval, rval);
            lval = expr;
        }

        l_expression = lval;
        return l_expression;
    }

    /**
     * logical_or
     * matches a logical or expression
     */
    std::shared_ptr<expr> parser::logical_or() {
        std::shared_ptr<expr> l_expression = nullptr;
        std::shared_ptr<expr> lval = logical_and();

        while(match(LOGICAL_OR)) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> rval = logical_and();
            std::shared_ptr<binary_expression> expr = std::make_shared<binary_expression>(* op, LOGICAL_OR_EXPR, lval, rval);
            lval = expr;
        }

        l_expression = lval;
        return l_expression;
    }

    /**
     * logical_and
     * matches a logical and expression
     */
    std::shared_ptr<expr> parser::logical_and() {
        std::shared_ptr<expr> l_expression = nullptr;
        std::shared_ptr<expr> lval = logical_not();

        while(match(LOGICAL_AND)) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> rval = logical_not();
            std::shared_ptr<binary_expression> expr = std::make_shared<binary_expression>(* op, LOGICAL_AND_EXPR, lval, rval);
            lval = expr;
        }

        l_expression = lval;
        return l_expression;
    }

    /**
     * logical_not
     * matches a logical not expression
     */
    std::shared_ptr<expr> parser::logical_not() {
        std::shared_ptr<expr> l_expression = nullptr;

        if(match(LOGICAL_NOT)) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> val = logical_not();
            std::shared_ptr<unary_expression> expr = std::make_shared<unary_expression>(* op, LOGICAL_NOT_EXPR, val);
            l_expression = expr;
        }
        else {
            l_expression = comparison();
        }

        return l_expression;
    }

    /**
     * comparison
     * matches comparison expersions
     */
    std::shared_ptr<expr> parser::comparison() {
        std::shared_ptr<expr> l_expression = nullptr;
        std::shared_ptr<expr> lval = bitwise_or();

        while(
            match(IN)           ||
            match(NOT_IN)       ||
            match(NEXT_IN)      ||
            match(PREV_IN)      ||
            match(IS)           ||
            match(IS_NOT)       ||
            match(LESS)         ||
            match(LESS_EQUAL)   ||
            match(GREATER)      ||
            match(GREATER_EQUAL)||
            match(NOT_EQUAL)    ||
            match(EQUAL_EQUAL)
        ) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> rval = bitwise_or();
            std::shared_ptr<binary_expression> expr = nullptr;
            switch(op -> get_type()) {
            case IN:
                expr = std::make_shared<binary_expression>(* op, IN_EXPR, lval, rval);
                break;
            case NOT_IN:
                expr = std::make_shared<binary_expression>(* op, NOT_IN_EXPR, lval, rval);
                break;
            case NEXT_IN:
                expr = std::make_shared<binary_expression>(* op, NEXT_IN_EXPR, lval, rval);
                break;
            case PREV_IN:
                expr = std::make_shared<binary_expression>(* op, PREV_IN_EXPR, lval, rval);
                break;
            case IS:
                expr = std::make_shared<binary_expression>(* op, IS_EXPR, lval, rval);
                break;
            case IS_NOT:
                expr = std::make_shared<binary_expression>(* op, IS_NOT_EXPR, lval, rval);
                break;
            case LESS:
                expr = std::make_shared<binary_expression>(* op, LESS_EXPR, lval, rval);
                break;
            case LESS_EQUAL:
                expr = std::make_shared<binary_expression>(* op, LESS_EQUAL_EXPR, lval, rval);
                break;
            case GREATER:
                expr = std::make_shared<binary_expression>(* op, GREATER_EXPR, lval, rval);
                break;
            case GREATER_EQUAL:
                expr = std::make_shared<binary_expression>(* op, GREATER_EQUAL_EXPR, lval, rval);
                break;
            case NOT_EQUAL:
                expr = std::make_shared<binary_expression>(* op, NOT_EQUAL_EXPR, lval, rval);
                break;
            case EQUAL_EQUAL:
                expr = std::make_shared<binary_expression>(* op, EQUAL_EQUAL_EXPR, lval, rval);
                break;
            // empty default case to avoid warnings by [-Wswitch]
            default:
                ;
            }
            lval = expr;
        }

        l_expression = lval;
        return l_expression;
    }

    /**
     * bitwise_or
     * matches a bitwise or expression
     */
    std::shared_ptr<expr> parser::bitwise_or() {
        std::shared_ptr<expr> l_expression = nullptr;
        std::shared_ptr<expr> lval = bitwise_xor();

        while(match(BITWISE_OR)) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> rval = bitwise_xor();
            std::shared_ptr<binary_expression> expr = std::make_shared<binary_expression>(* op, BITWISE_OR_EXPR, lval, rval);
            lval = expr;
        }

        l_expression = lval;
        return l_expression;
    }

    /**
     * bitwise_xor
     * matches a bitwise xor expression
     */
    std::shared_ptr<expr> parser::bitwise_xor() {
        std::shared_ptr<expr> l_expression = nullptr;
        std::shared_ptr<expr> lval = bitwise_and();

        while(match(BITWISE_XOR)) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> rval = bitwise_and();
            std::shared_ptr<binary_expression> expr = std::make_shared<binary_expression>(* op, BITWISE_XOR_EXPR, lval, rval);
            lval = expr;
        }

        l_expression = lval;
        return l_expression;
    }

    /**
     * bitwise_and
     * matches a bitwise and expression
     */
    std::shared_ptr<expr> parser::bitwise_and() {
        std::shared_ptr<expr> l_expression = nullptr;
        std::shared_ptr<expr> lval = bit_shift();

        while(match(BITWISE_AND)) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> rval = bit_shift();
            std::shared_ptr<binary_expression> expr = std::make_shared<binary_expression>(* op, BITWISE_AND_EXPR, lval, rval);
            lval = expr;
        }

        l_expression = lval;
        return l_expression;
    }

    /**
     * bit_shift
     * matches bit shift expressions
     */
    std::shared_ptr<expr> parser::bit_shift() {
        std::shared_ptr<expr> l_expression = nullptr;
        std::shared_ptr<expr> lval = term();

        while(match(LEFT_SHIFT) || match(RIGHT_SHIFT)) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> rval = term();
            std::shared_ptr<binary_expression> expr = nullptr;
            if(op -> get_type() == LEFT_SHIFT)
                expr = std::make_shared<binary_expression>(* op, LEFT_SHIFT_EXPR, lval, rval);
            else
                expr = std::make_shared<binary_expression>(* op, RIGHT_SHIFT_EXPR, lval, rval);
            lval = expr;
        }

        l_expression = lval;
        return l_expression;
    }

    /**
     * term
     * matches addition and substraction expressions
     */
    std::shared_ptr<expr> parser::term() {
        std::shared_ptr<expr> l_expression = nullptr;
        std::shared_ptr<expr> lval = factor();

        while(match(PLUS) || match(MINUS)) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> rval = factor();
            std::shared_ptr<binary_expression> expr = nullptr;
            if(op -> get_type() == PLUS)
                expr = std::make_shared<binary_expression>(* op, PLUS_EXPR, lval, rval);
            else
                expr = std::make_shared<binary_expression>(* op, MINUS_EXPR, lval, rval);
            lval = expr;
        }

        l_expression = lval;
        return l_expression;
    }

    /**
     * factor
     * matches multiplication, division and modulus expressions
     */
    std::shared_ptr<expr> parser::factor() {
        std::shared_ptr<expr> l_expression = nullptr;
        std::shared_ptr<expr> lval = cast();

        while(match(MUL) || match(DIV) || match(MOD)) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> rval = cast();
            std::shared_ptr<binary_expression> expr = nullptr;
            if(op -> get_type() == PLUS)
                expr = std::make_shared<binary_expression>(* op, MUL_EXPR, lval, rval);
            else if(op -> get_type() == DIV)
                expr = std::make_shared<binary_expression>(* op, DIV_EXPR, lval, rval);
            else
                expr = std::make_shared<binary_expression>(* op, MOD_EXPR, lval, rval);
            lval = expr;
        }

        l_expression = lval;
        return l_expression;
    }

    /**
     * cast
     * matches a type casting expression
     */
    std::shared_ptr<expr> parser::cast() {
        std::shared_ptr<expr> l_expression = nullptr;

        if(match(CAST)) {
            std::shared_ptr<token>& op = lookback();
            
            // we get the type instance to cast to
            consume(LEFT_PAREN, "Excepted an opening parenthesis before the type to cast to.");
            type_instance cast_type_instance = parse_type_instance();
            consume(RIGHT_PAREN, "Excepted a closing parenthesis after the type to cast to.");

            // we get the expression to cast to
            std::shared_ptr<expr> val = parse_expression();
            std::shared_ptr<cast_expression> expr = std::make_shared<cast_expression>(* op, cast_type_instance, val);
            l_expression = expr;
        }
        else {
            l_expression = sign();
        }

        return l_expression;
    }

    /**
     * sign
     * matches a sign inversion expression
     */
    std::shared_ptr<expr> parser::sign() {
        std::shared_ptr<expr> l_expression = nullptr;

        if(match(PLUS) || match(MINUS)) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> val = sign();
            std::shared_ptr<unary_expression> expr = nullptr;
            if(op -> get_type() == PLUS)
                expr = std::make_shared<unary_expression>(* op, PLUS_SIGN_EXPR, val);
            else
                expr = std::make_shared<unary_expression>(* op, MINUS_SIGN_EXPR, val);
            l_expression = expr;
        }
        else {
            l_expression = bitwise_not();
        }

        return l_expression;
    }

    /**
     * bitwise_not
     * matches a bitwise not expression
     */
    std::shared_ptr<expr> parser::bitwise_not() {
        std::shared_ptr<expr> l_expression = nullptr;

        if(match(BITWISE_NOT)) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> val = bitwise_not();
            std::shared_ptr<unary_expression> expr = std::make_shared<unary_expression>(* op, BITWISE_NOT_EXPR, val);
            l_expression = expr;
        }
        else {
            l_expression = dot();
        }

        return l_expression;
    }

    /**
     * dot
     * matches a dot expression
     */
    std::shared_ptr<expr> parser::dot() {
        std::shared_ptr<expr> l_expression = nullptr;
        std::shared_ptr<expr> lval = subscript();

        while(match(DOT)) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> rval = subscript();
            std::shared_ptr<binary_expression> expr = std::make_shared<binary_expression>(* op, DOT_EXPR, lval, rval);
            lval = expr;
        }

        l_expression = lval;
        return l_expression;
    }

    /**
     * subscript
     * matches a subscript expression
     */
    std::shared_ptr<expr> parser::subscript() {
        std::shared_ptr<expr> l_expression = nullptr;
        std::shared_ptr<expr> lval = primary();

        while(match(LEFT_BRACKET)) {
            std::shared_ptr<token>& op = lookback();
            std::shared_ptr<expr> rval = primary();
            consume(RIGHT_BRACKET, "Expected a closing bracket after subscript expression.");
            std::shared_ptr<binary_expression> expr = std::make_shared<binary_expression>(* op, SUBSCRIPT_EXPR, lval, rval);
            lval = expr;
        }

        l_expression = lval;
        return l_expression;
    }

    /**
     * primary
     * matches a primary expression
     * a primary expression can be:
     * - a call expression : which includes function calls and constructors with arguments
     * - an identifier expression : which includes variable expressions and constructors without arguments
     * - a tuple expression
     * - a grouped expression
     * - a literal expression
     * - an underscore expression
     */
    std::shared_ptr<expr> parser::primary() {
        std::shared_ptr<expr> l_expression = nullptr;

        if(check(IDENTIFIER) && check_next(LEFT_PAREN)) {
            l_expression = parse_call_expression();
        }
        else if(check(IDENTIFIER)) {
            std::shared_ptr<token>& id_tok = consume(IDENTIFIER, "Expected an identifier.");
            std::shared_ptr<identifier_expression> id_expr = std::make_shared<identifier_expression>(* id_tok);
            l_expression = id_expr;
        }
        else if(match(LEFT_PAREN)) {
            std::shared_ptr<token>& left_paren = lookback();
            std::shared_ptr<expr> inner_expression = parse_expression();
            if(match(COMMA)) {
                l_expression = parse_tuple_expression(left_paren, inner_expression);
            }
            else {
                std::shared_ptr<grouped_expression> grouped_expr = std::make_shared<grouped_expression>(* left_paren, inner_expression);
                consume(RIGHT_BRACKET, "Expected a closing parenthesis after grouped expression.");
                l_expression = grouped_expr;
            }
        }
        else if(match(INTEGER) || match(FLOATING_POINT) || match(DECIMAL) || match(STRING)) {
            std::shared_ptr<token>& literal_tok = lookback();
            std::shared_ptr<literal_expression> literal_expr = std::make_shared<literal_expression>(* literal_tok, literal_tok -> get_lexeme());
            l_expression = literal_expr;
        }
        else if(match(UNDERSCORE)) {
            std::shared_ptr<token>& underscore_tok = lookback();
            std::shared_ptr<underscore_expression> underscore_expr = std::make_shared<underscore_expression>(* underscore_tok);
            l_expression = underscore_expr;
        }
        else {
            throw parsing_error(false, peek(), "Expected an expression.");
        }

        return l_expression;
    }

    /**
     * parse_call_expression
     * if an expression starts by identifier followed by an opening parenthesis
     * this function parses the remainder of the token stream until it reads a call expression.
     */
    std::shared_ptr<expr> parser::parse_call_expression() {
        std::shared_ptr<expr> l_expression = nullptr;
        std::shared_ptr<token>& function_name = consume(IDENTIFIER, "Expected the name of the function to call.");
        std::shared_ptr<call_expression> fun_call_expr = std::make_shared<call_expression>(* function_name);

        consume(LEFT_PAREN, "Expected an opening parenthesis before the function call arguments.");
        do {
            if(check(IDENTIFIER) && check_next(EQUAL)) {
                std::shared_ptr<token>& arg_name = consume(IDENTIFIER, "Expected the argument name in function call.");
                consume(EQUAL, "Expected an equal sign before the argument value.");
                std::shared_ptr<expr> arg_value = parse_expression();
                fun_call_expr -> add_argument(* arg_name, arg_value);
            }
            else {
                std::shared_ptr<expr> arg_value = parse_expression();
                fun_call_expr -> add_argument(star_tok, arg_value); // we use the general purpose underscore token in lieu of the actual argument name as we have none
            }
        } while(match(COMMA));
        consume(LEFT_PAREN, "Expected a closing parenthesis after the function call arguments.");

        if(match(RETURN_TYPE)) {
            type_instance return_type_instance = parse_type_instance();
            fun_call_expr -> set_return_type_instance(return_type_instance);
        }

        l_expression = fun_call_expr;
        return l_expression;
    }

    /**
     * parse_tuple_expression
     * if we encounter an open parenthesis followed by an expression then by a comma,
     * then we know we have a tuple instead of a grouped expression. this function parses the remainder of the tuple.
     */
    std::shared_ptr<expr> parser::parse_tuple_expression(std::shared_ptr<token>& left_paren, std::shared_ptr<expr>& first_element) {
        std::shared_ptr<expr> l_expression = nullptr;
        std::shared_ptr<tuple_expression> tuple_expr = std::make_shared<tuple_expression>(* left_paren);
        tuple_expr -> add_element(first_element);

        std::shared_ptr<expr> next_element = nullptr;
        do {
            next_element = parse_expression();
            tuple_expr -> add_element(next_element);
        } while(match(COMMA));

        l_expression = tuple_expr;
        return l_expression;
    }

    /**
     * parse_function_call_type_instance
     * parses the type instance to of a function call return type
     */
    type_instance parser::parse_type_instance() {
        // if we have an identifier followed by a dot, that identifier is the namespace name
        std::string namespace_name("*");
        if(check(IDENTIFIER) && check_next(DOT)) {
            std::shared_ptr<token>& namespace_tok = consume(IDENTIFIER, "Expected the namespace name where to find the type.");
            consume(DOT, "Expected a dot after the namespace name in type instance.");
            namespace_name = namespace_tok -> get_lexeme();
        }

        // if we get an identifier, then we have a simple type instance
        if(match(IDENTIFIER)) {
            std::shared_ptr<token>& instance_tok = lookback();
            type_instance instance(* instance_tok, namespace_name);

            // if the type instance admits parameters, we build it as such
            // and check if there exists a type constructor that builds it
            if(match(LEFT_PAREN)) {
                do {
                    type_instance param = parse_type_instance();
                    instance.add_param(param);
                } while(match(COMMA));
                consume(RIGHT_PAREN, "Expected closing parenthesis after type instance parameters.");
            }
            return instance;
        }
        // anything else is wrong
        else {
            throw parsing_error(true, lookback(), "Expected a valid type instance.");
        }
    }


    /**
     * lookback
     * returns the token that comes before the current token being worked on
     */
    std::shared_ptr<token>& parser::lookback() {
        if (m_current == 0)
            return m_tokens.at(m_current);
        else
            return m_tokens.at(m_current - 1);
    }

    /**
     * peek
     * returns the token that comes after the current token being worked on
     */
    std::shared_ptr<token>& parser::peek() {
        return m_tokens.at(m_current);
    }

    /**
     * is_at_end
     * return true if we have reached the last token
     */
    bool parser::is_at_end() {
        std::shared_ptr<token>& tok = peek();

        if(tok -> get_type() == AVL_EOF)
            return true;
        else
            return false;
    }

    /**
     * peek_next
     * return the token that comes after the token that comes after the current token
     */
    std::shared_ptr<token>& parser::peek_next() {
        if (is_at_end())
            return m_tokens.at(m_current);
        else
            return m_tokens.at(m_current + 1);
    }

    /**
     * is_past_end
     * returns true if we are one token away from the end of the token stream
     */
    bool parser::is_past_end() {
        std::shared_ptr<token>& tok = peek_next();
        
        if(tok -> get_type() == AVL_EOF)
            return true;
        else
            return false;
    }

    /**
     * check
     * given a token type, this function returns true if the current token is of the given token type
     */
    bool parser::check(const token_type type) {
        if(is_at_end()) {
            return false;
        }
        else {
            std::shared_ptr<token>& tok = peek();
            if(tok -> get_type() == type)
                return true;
            else
                return false;
        }
    }

    /**
     * check_previous
     * given a token type, this function returns true if the previous token is of the given token type
     */
    bool parser::check_previous(const token_type type) {
        if(m_current == 0) {
            return false;
        }
        else {
            std::shared_ptr<token>& tok = lookback();
            if(tok -> get_type() == type)
                return true;
            else
                return false;
        }
    }

    /**
     * check_next
     * given a token type, this function returns true if the next token is of the given token type
     */
    bool parser::check_next(const token_type type) {
        if(is_at_end() || is_past_end()) {
            return false;
        }
        else {
            std::shared_ptr<token>& tok = peek_next();
            if(tok -> get_type() == type)
                return true;
            else
                return false;
        }
    }

    /**
     * advance
     * moves to the next token
     */
    std::shared_ptr<token>& parser::advance() {
        if(!is_at_end())
            m_current++;

        return lookback();
    }

    /**
     * match
     * returns true if the current token is of the given token type
     * and moves to the next token
     */
    bool parser::match(const token_type type) {
        if(check(type)) {
            advance();
            return true;
        }

        return false;
    }

    /**
     * consume
     * if the next token is of the given token type, return it and advance
     * else, raise an error.
     * all errors raise by this function are fatal.
     */
    std::shared_ptr<token>& parser::consume(token_type type, const std::string& message) {
        if(check(type))
            return advance();
        else
            throw parsing_error(true, peek(), message);
    }

    /**
     * parsing_error
     * contructs and returns a parse_error exception
     */
    parse_error parser::parsing_error(bool fatal, std::shared_ptr<token>& tok, const std::string& message) {
        return parse_error(m_error_handler, * tok, fatal, message);
    }

    /**
     * synchronize
     * for non-fatal errors, instead of terminating the parsing process, we use this function
     * to decide where to resume parsing from.
     */
    void parser::synchronize() {
        advance();

        while(!is_at_end()) {
            std::shared_ptr<token>& previous_token = lookback();
            if(previous_token -> get_type() == NEWLINE) return;

            std::shared_ptr<token>& next_token = peek();
            switch(next_token -> get_type()) {
                case IMPORT:
                case NAMESPACE:
                case PUBLIC:
                case PRIVATE:
                case TYPE:
                case VERTICAL_BAR:
                case VAR:
                case VAL:
                case FUNCTION:
                case IF:
                case SWITCH:
                case FOR:
                case WHILE:
                case RETURN:
                case BREAK:
                case CONTINUE:
                case PASS:
                    return;
                default:
                    break;
            }

            advance();
        }
    }
}
