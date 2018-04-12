#include <iostream>
#include <string>

#include "lexer/token_type.hpp"


namespace avalon {
    static std::string token_type_to_string[] = {
        "DOT",
        "LOGICAL_NOT",
        "BITWISE_NOT",
        "BITWISE_OR",
        "BITWISE_AND",
        "BITWISE_XOR",
        "LEFT_PAREN",
        "RIGHT_PAREN",
        "LEFT_BRACE",
        "RIGHT_BRACE",
        "LEFT_BRACKET",
        "RIGHT_BRACKET",
        "VERTICAL_BAR",
        "UNDERSCORE",
        "PLUS",
        "MUL",
        "DIV",
        "MOD",
        "COMMA",
        "COLON",
        "NEWLINE",
        "INDENT",
        "DEDENT",

        /* One or two character tokens - even if we get one character, we must make sure it is not actually part of a two or more character lexeme */
        "EQUAL",
        "EQUAL_EQUAL",
        "MATCH",
        "NOT_EQUAL",
        "NOT_MATCH",
        "GREATER",
        "GREATER_EQUAL",
        "LESS",
        "LESS_EQUAL",
        "IN",
        "NOT_IN",
        "NEXT_IN",
        "PREV_IN",
        "IS",
        "IS_NOT",
        "MINUS",
        "RETURN_TYPE",
        "NS_LEFT_PAREN",
        "NS_RIGHT_PAREN",
        "LOGICAL_OR",
        "LOGICAL_AND",
        "LEFT_SHIFT",
        "RIGHT_SHIFT",

        /* Literals */
        "IDENTIFIER",
        "INTEGER",
        "FLOATING_POINT",
        "DECIMAL",
        "STRING",

        /* Keywords */
        "IMPORT",
        "NAMESPACE",
        "PUBLIC",
        "PRIVATE",
        "TYPE",
        "FUNCTION",
        "VAR",
        "VAL",
        "CAST",
        "SWITCH",
        "CASE",
        "DEFAULT",
        "IF",
        "ELIF",
        "ELSE",
        "FOR",
        "EMPTY",
        "WHILE",
        "CONTINUE",
        "BREAK",
        "RETURN",
        "PASS",

        /* Specials */
        "AVL_EOF",
    };

    std::string get_token_type_string(token_type type) {
        return std::string(token_type_to_string[type]);
    }

    std::ostream& operator<<(std::ostream& os, const token_type& type) {
        if(type != AVL_EOF)
            return os << std::string(token_type_to_string[type]);
        else
            return os << "";
    }
}
