#ifndef AVALON_LEXER_TOKEN_TYPE_HPP_
#define AVALON_LEXER_TOKEN_TYPE_HPP_

namespace avalon {
    enum token_type {
        /* single-character tokens - these tokens can happen in circumstances where they serve different purposes */
        DOT = 0,            // .
        LOGICAL_NOT,        // !  (not)
        BITWISE_NOT,        // ~  (bnot)
        BITWISE_OR,         // |  (bor)
        BITWISE_AND,        // &  (band)
        BITWISE_XOR,        // ^  (xor)
        LEFT_PAREN,         // (
        RIGHT_PAREN,        // )
        LEFT_BRACE,         // {
        RIGHT_BRACE,        // }
        LEFT_BRACKET,       // [
        RIGHT_BRACKET,      // ]
        VERTICAL_BAR,       // |
        UNDERSCORE,         // _
        PLUS,               // +
        MUL,                // *
        DIV,                // /
        MOD,                // %
        COMMA,              // ,
        COLON,              // :
        NEWLINE,            // \n
        INDENT,             // space or tab
        DEDENT,             // backspace(s)

        /* one or two character tokens - even if we get one character, we must make sure it is not actually part of a two or more character lexeme */
        EQUAL,              // =
        EQUAL_EQUAL,        // ==
        NOT_EQUAL,          // !=
        GREATER,            // >
        GREATER_EQUAL,      // >=
        LESS,               // <
        LESS_EQUAL,         // <=
        IN,                 // in
        NOT_IN,             // not in
        NEXT_IN,            // next in
        PREV_IN,            // previously in
        IS,                 // is
        IS_NOT,             // is not
        MINUS,              // -
        RETURN_TYPE,        // ->
        NS_LEFT_PAREN,      // -(
        NS_RIGHT_PAREN,     // )-
        LOGICAL_OR,         // || (or)
        LOGICAL_AND,        // && (and)
        LEFT_SHIFT,         // << (lsh)
        RIGHT_SHIFT,        // >> (rsh)

        /* literals */
        IDENTIFIER,         // variable, function or type name
        INTEGER,            // integers
        FLOATING_POINT,     // floating point numbers
        DECIMAL,            // decimal numbers
        STRING,             // strings

        /* keywords */
        IMPORT,             // import
        NAMESPACE,          // namespace
        PUBLIC,             // public
        PRIVATE,            // private
        TYPE,               // type
        FUNCTION,           // function
        VAR,                // var
        VAL,                // val
        CAST,               // cast
        SWITCH,             // switch for pattern matching
        CASE,               // switch instances
        DEFAULT,            // default case
        IF,                 // if
        ELIF,               // else if
        ELSE,               // else
        FOR,                // for
        EMPTY,              // empty
        WHILE,              // while
        CONTINUE,           // continue
        BREAK,              // break
        RETURN,             // return
        PASS,               // pass

        /* specials */
        AVL_EOF             // end file
    };

    std::string get_token_type_string(token_type type);

    std::ostream& operator<<(std::ostream& os, const token_type& type);
}

#endif
