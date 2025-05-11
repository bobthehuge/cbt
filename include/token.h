#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

typedef enum {
    // Keywords
    TK_AUTO,
    TK_BREAK,
    TK_CASE,
    TK_CHAR,
    TK_CONST,
    TK_CONTINUE,
    TK_DEFAULT,
    TK_DO,
    TK_DOUBLE,
    TK_ELSE,
    TK_ENUM,
    TK_EXTERN,
    TK_FLOAT,
    TK_FOR,
    TK_GOTO,
    TK_IF,
    TK_INLINE,
    TK_INT,
    TK_LONG,
    TK_REGISTER,
    TK_RESTRICT,
    TK_RETURN,
    TK_SHORT,
    TK_SIGNED,
    TK_SIZEOF,
    TK_STATIC,
    TK_STRUCT,
    TK_SWITCH,
    TK_TYPEDEF,
    TK_UNION,
    TK_UNSIGNED,
    TK_VOID,
    TK_VOLATILE,
    TK_WHILE,

    // Constants
    TK_INT_CONST,
    TK_FLOAT_CONST,
    TK_DOUBLE_CONST,
    TK_CHAR_CONST,
    TK_STRING_LITERAL,

    // Identifiers
    TK_IDENTIFIER,

    // Operators
    TK_PLUS,        // +
    TK_MINUS,       // -
    TK_STAR,        // *
    TK_SLASH,       // /
    TK_PERCENT,     // %
    TK_INC,         // ++
    TK_DEC,         // --
    TK_ARROW,       // ->
    TK_DOT,         // .
    TK_AND,         // &
    TK_OR,          // |
    TK_TILDE,       // ~
    TK_NOT,         // !
    TK_LSHIFT,      // <<
    TK_RSHIFT,      // >>
    TK_LT,          // <
    TK_GT,          // >
    TK_LE,          // <=
    TK_GE,          // >=
    TK_EQ,          // ==
    TK_NE,          // !=
    TK_XOR,         // ^
    TK_AND_AND,     // &&
    TK_OR_OR,       // ||
    TK_QUESTION,    // ?
    TK_COLON,       // :
    TK_ASSIGN,      // =
    TK_MUL_ASSIGN,  // *=
    TK_DIV_ASSIGN,  // /=
    TK_MOD_ASSIGN,  // %=
    TK_ADD_ASSIGN,  // +=
    TK_SUB_ASSIGN,  // -=
    TK_LSHIFT_ASSIGN, // <<=
    TK_RSHIFT_ASSIGN, // >>=
    TK_AND_ASSIGN,  // &=
    TK_XOR_ASSIGN,  // ^=
    TK_OR_ASSIGN,   // |=

    // Punctuators
    TK_SEMICOLON,   // ;
    TK_LPAREN,      // (
    TK_RPAREN,      // )
    TK_LBRACE,      // {
    TK_RBRACE,      // }
    TK_LBRACKET,    // [
    TK_RBRACKET,    // ]
    TK_COMMA,       // ,
    TK_ELLIPSIS,    // ...

    // Preprocessor
    TK_HASH,        // #
    TK_HASH_HASH,   // ##
    TK_DEFINE,
    TK_INCLUDE,
    TK_IFDEF,
    TK_IFNDEF,
    TK_ENDIF,
    TK_ELIF,
    TK_PRAGMA,
    TK_ERROR,
    TK_LINE,

    // Special tokens
    TK_COMMENT,
    TK_CPP_COMMENT,
    TK_NEWLINE,
    TK_EOF,
    TK_UNKNOWN
} TokenKind;

extern const char *KEYWORD_TABLE[];
extern const size_t KEYWORD_COUNT;
extern const char *DELIM_TABLE[];
extern const size_t DELIM_COUNT;

int check_prefix_collisions(size_t *h, size_t *s);
struct bth_lex_token *collect_tokens(struct bth_lexer *lexer);

#endif
