#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../include/bth_lex.h"
typedef struct bth_lexer Lexer;

const char *KEYWORD_TABLE[] = {
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

const size_t KEYWORD_COUNT = sizeof(KEYWORD_TABLE) / (sizeof(char *) * 2);

const char *DELIM_TABLE[] = {
    "TK_STRING_LITERAL", "\"", "\"",
    "TK_CHAR_LITERAL",   "\'", "\'",
    "TK_COMMENT",        "//", "\n",
    "TK_CPP_COMMENT",    "/*", "*/",
};

const size_t DELIM_COUNT = sizeof(DELIM_TABLE) / (sizeof(char *) * 3);

// checks if a substring of a symbol is placed before it in the table
int check_prefix_collisions(size_t *h, size_t *s)
{
    bool mark[KEYWORD_COUNT];
    memset(mark, false, KEYWORD_COUNT * sizeof(bool));
    
    for (size_t i = 0; i < KEYWORD_COUNT; i++)
    {
        if (mark[i])
            continue;

        for (size_t j = 0; j < KEYWORD_COUNT; j++)
        {
            if (mark[j] || i == j)
                continue;

            size_t hidx = i;
            size_t sidx = j;

            if (j > i)
            {
                hidx ^= sidx;
                sidx ^= hidx;
                hidx ^= sidx;
            }
            
            const char *hay = KEYWORD_TABLE[hidx*2+1];
            const char *sub = KEYWORD_TABLE[sidx*2+1];

            if (!strncmp(hay, sub, strlen(sub)))
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
