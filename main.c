#include <assert.h>
#include <error.h>
#include <stdio.h>

#define BTH_LEX_IMPLEMENTATION
#include "bth_lex.h"
typedef struct bth_lexer Lexer;

#define BTH_OPTION_IMPLEMENTATION
#include "bth_option.h"
OPTION_TYPEDEF(size_t);

#define BTH_IO_IMPLEMENTATION
#include "bth_io.h"

#include "bth_types.h"

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

const char *SYMBOLS_TABLE[] = {
    // Keywords
    "TK_DEFINE", "define",
    "TK_INCLUDE", "include",
    "TK_IFDEF", "ifdef",
    "TK_IFNDEF", "ifndef",
    "TK_ENDIF", "endif",
    "TK_ELIF", "elif",
    "TK_PRAGMA", "pragma",
    "TK_ERROR", "error",
    "TK_AUTO", "auto",
    "TK_BREAK", "break",
    "TK_CASE", "case",
    "TK_CHAR", "char",
    "TK_CONST", "const",
    "TK_CONTINUE", "continue",
    "TK_DEFAULT", "default",
    "TK_DOUBLE", "double",
    "TK_DO", "do",
    "TK_ELSE", "else",
    "TK_ENUM", "enum",
    "TK_EXTERN", "extern",
    "TK_FLOAT", "float",
    "TK_FOR", "for",
    "TK_GOTO", "goto",
    "TK_IF", "if",
    "TK_INLINE", "inline",
    "TK_INT", "int",
    "TK_LONG", "long",
    "TK_REGISTER", "register",
    "TK_RESTRICT", "restrict",
    "TK_RETURN", "return",
    "TK_SHORT", "short",
    "TK_UNSIGNED", "unsigned",
    "TK_SIGNED", "signed",
    "TK_SIZEOF", "sizeof",
    "TK_STATIC", "static",
    "TK_STRUCT", "struct",
    "TK_SWITCH", "switch",
    "TK_TYPEDEF", "typedef",
    "TK_UNION", "union",
    "TK_VOID", "void",
    "TK_VOLATILE", "volatile",
    "TK_WHILE", "while",
    "TK_LINE", "line",

    // Punctuators
    "TK_SEMICOLON", ";",
    "TK_LPAREN", "(",
    "TK_RPAREN", ")",
    "TK_LBRACE", "{",
    "TK_RBRACE", "}",
    "TK_LBRACKET", "[",
    "TK_RBRACKET", "]",
    "TK_COMMA", ",",
    "TK_ELLIPSIS", "...",
    
    // Operators
    "TK_LSHIFT_ASSIGN", "<<=",
    "TK_RSHIFT_ASSIGN", ">>=",
    "TK_INC", "++",
    "TK_DEC", "--",
    "TK_ARROW", "->",
    "TK_LSHIFT", "<<",
    "TK_RSHIFT", ">>",
    "TK_LE", "<=",
    "TK_GE", ">=",
    "TK_EQ", "==",
    "TK_NE", "!=",
    "TK_AND_AND", "&&",
    "TK_OR_OR", "||",
    "TK_MUL_ASSIGN", "*=",
    "TK_DIV_ASSIGN", "/=",
    "TK_MOD_ASSIGN", "%=",
    "TK_ADD_ASSIGN", "+=",
    "TK_SUB_ASSIGN", "-=",
    "TK_AND_ASSIGN", "&=",
    "TK_XOR_ASSIGN", "^=",
    "TK_OR_ASSIGN", "|=",

    "TK_PLUS", "+",
    "TK_MINUS", "-",
    "TK_STAR", "*",
    "TK_SLASH", "/",
    "TK_PERCENT", "%",
    "TK_DOT", ".",
    "TK_AND", "&",
    "TK_OR", "|",
    "TK_TILDE", "~",
    "TK_NOT", "!",
    "TK_LT", "<",
    "TK_GT", ">",
    "TK_XOR", "^",
    "TK_QUESTION", "?",
    "TK_COLON", ":",
    "TK_ASSIGN", "=",

    // Preprocessor
    "TK_HASH_HASH", "##",
    "TK_HASH", "#",
    "TK_SPACE", " ",
    "TK_NEWLINE", "\n",
    "TK_ANTISLASH", "\\",
};

const size_t SYMBOLS_COUNT = sizeof(SYMBOLS_TABLE) / (sizeof(char *) * 2);

const char *DELIMS_TABLE[] = {
    "TK_STRING_LITERAL", "\"", "\"",
    "TK_CHAR_LITERAL",   "\'", "\'",
    "TK_COMMENT",        "//", "\n",
    "TK_CPP_COMMENT",    "/*", "*/",
};

const size_t DELIMS_COUNT = sizeof(DELIMS_TABLE) / (sizeof(char *) * 3);

int check_prefix_collisions(size_t *h, size_t *s)
{
    bool mark[SYMBOLS_COUNT];
    memset(mark, false, SYMBOLS_COUNT * sizeof(bool));
    
    for (size_t i = 0; i < SYMBOLS_COUNT; i++)
    {
        if (mark[i])
            continue;

        for (size_t j = 0; j < SYMBOLS_COUNT; j++)
        {
            if (mark[j] || i == j)
                continue;

            size_t hidx = i;
            size_t sidx = j;
            
            const char *hay = SYMBOLS_TABLE[hidx*2+1];
            const char *sub = SYMBOLS_TABLE[sidx*2+1];

            if (strlen(hay) < strlen(sub))
            {
                hidx ^= sidx;
                sidx ^= hidx;
                hidx ^= sidx;
            }

            hay = SYMBOLS_TABLE[hidx*2+1];
            sub = SYMBOLS_TABLE[sidx*2+1];

            if (strstr(hay, sub) && hidx > sidx)
            {
                *h = hidx;
                *s = sidx;
                return 1;
            }
        }

        mark[i] = true;
    }
    return 0;
}

struct bth_lex_token *collect_tokens(Lexer *lexer)
{
    struct bth_lex_token *toks = malloc(0);
    size_t c = 0;
    struct bth_lex_token tok;

    do
    {
        tok = bth_lex_get_token(lexer);

        switch (tok.kind)
        {
        case INVALID:
            errx(1, "at %zu:%zu: INVALID", tok.col, tok.row);
            break;
        case LK_END:
        // FALLTHROUGH
        case LK_IDENT:
        // FALLTHROUGH
        case LK_SYMBOL:
        // FALLTHROUGH
        case LK_DELIMITED:
            if (c > 0 && tok.kind == toks[c - 1].kind
                && !strncmp(tok.begin, toks[c - 1].begin, tok.end - tok.begin))
            {
                toks[c - 1].repeat++;
            }
            else
            {
                toks = realloc(toks, (c + 1) * sizeof(struct bth_lex_token));
                toks[c] = tok;
                c++;
            }
            break;
        default:
            errx(1, "UNREACHABLE");
        }
    } while (tok.kind != LK_END);

    return toks;
}

char *strndup(const char *str, size_t n)
{
    size_t len = strlen(str);
    len = len > n ? n : len;

    char *res = malloc(len + 1);
    memcpy(res, str, len);

    res[len] = 0;

    return res;
}

char *stresc(const char *str)
{
    const char table[7] = {
        ['\a'%7] = 'a',
        ['\b'%7] = 'b',
        ['\t'%7] = 't',
        ['\n'%7] = 'n',
        ['\v'%7] = 'v',
        ['\f'%7] = 'f',
        ['\r'%7] = 'r',
    };

    const char *iter;
    size_t flen = 0;

    for (iter = str; *iter; iter++)
    {
        if (*iter >= '\a' && *iter <= '\r')
            flen++;
        flen++;
    }

    char *res = malloc(flen + 1);
    char *ptr = res;
    
    for (iter = str; *iter; iter++)
    {
        if (*iter >= '\a' && *iter <= '\r')
        {
            *ptr++ = '\\';
            *ptr++ = table[(*iter) % 7];
            continue;
        }

        *ptr++ = *iter;
    }

    *ptr = 0;

    return res;
}

int main(void)
{
#if 0
    {
        printf("heheheheh\n");
        size_t hay;
        size_t sub;

        if (check_prefix_collisions(&hay, &sub))
            errx(1, "Collisions detected: idx(%s) > idx(%s)",
                 SYMBOLS_TABLE[hay*2+1], SYMBOLS_TABLE[sub*2+1]);
    }
#endif
    
    str buf;
    OPTION(size_t) buflen = readfn(&buf, 0, "./samples/sample_1.c");
    CHECK(buflen);
    
    Lexer lexer = {
        .buffer = buf, .size = buflen.some,
        .col = 1, .row = 1,
        .cur = 0,
        .symbols = SYMBOLS_TABLE,
        .symbols_count = SYMBOLS_COUNT,
        .delims = DELIMS_TABLE,
        .delims_count = DELIMS_COUNT,
    };

    struct bth_lex_token *tokens = collect_tokens(&lexer);

    {
        size_t i = 0;
        while (tokens[i].kind != LK_END)
        {
            size_t len = tokens[i].end - tokens[i].begin;
            char *content = strndup(tokens[i].begin, len);
            char *todisp = stresc(content);
            free(content);

            printf("(bth_lex_token){name='%s', value='%s'}\n",
                   tokens[i].name, todisp);

            free(todisp);
            i++;
        }
    }
    
    return 0;
}
