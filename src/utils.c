#include <string.h>
#include "../include/utils.h"

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
