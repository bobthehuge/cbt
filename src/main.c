#include <assert.h>
#include <error.h>
#include <stdio.h>

#define BTH_LEX_IMPLEMENTATION
#include "../include/bth_lex.h"
typedef struct bth_lexer Lexer;

#define BTH_OPTION_IMPLEMENTATION
#include "../include/bth_option.h"
OPTION_TYPEDEF(size_t);

#define BTH_IO_IMPLEMENTATION
#include "../include/bth_io.h"

#include "../include/bth_types.h"
#include "../include/token.h"

int main(void)
{
#if CHECK_PREFIX_COLLISIONS
    {
        size_t hay;
        size_t sub;

        if (check_prefix_collisions(&hay, &sub))
            errx(1, "Collisions detected: idx(%s) > idx(%s)",
                 KEYWORD_TABLE[hay*2+1], KEYWORD_TABLE[sub*2+1]);
    }
#endif

    str buf;
    OPTION(size_t) buflen = readfn(&buf, 0, "./samples/sample_1.c");
    CHECK(buflen);
    
    Lexer lexer = {
        .buffer = buf, .size = buflen.some,
        .col = 1, .row = 1,
        .cur = 0,
        .symbols = KEYWORD_TABLE,
        .symbols_count = KEYWORD_COUNT,
        .delims = DELIM_TABLE,
        .delims_count = DELIM_COUNT,
    };

    struct bth_lex_token *tokens = collect_tokens(&lexer);

#if PRINT_TOKENS
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
#endif
    
    return 0;
}
