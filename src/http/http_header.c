//
//  http_header.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/04/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "http_header.h"

http_headers_t *http_headers_init()
{
    return dictionary_init();
}

http_headersitem_t *http_headersitem_init(const char *key, const void *value)
{
    return dictionaryitem_init(key, value);
}

void http_headers_add(http_headers_t *http_headers, const char *key, const void *value)
{
    dictionary_add(http_headers, key, value);
}

void http_headers_remove(http_headers_t *http_headers, const char *key)
{
    dictionary_remove(http_headers, key);
}

int http_headers_contains(http_headers_t *http_headers, const char *key)
{
    return dictionary_contains(http_headers, key);
}

const char *http_headers_get(http_headers_t *http_headers, const char *key)
{
    return dictionary_get(http_headers, key);
}

uint32_t http_headers_index(http_headers_t *http_headers, const char *key)
{
    return dictionary_index(http_headers, key);
}

char *http_header_get_string(http_headersitem_t *headersitem)
{
    unsigned long string_length = strlen(headersitem->key) + strlen(headersitem->value) + 4;
    char *buffer = malloc(string_length * sizeof(char *));
    snprintf(buffer, string_length, "%s: %s\n", headersitem->key, (char *)headersitem->value);
    return buffer;
}

void parse_header_line(char *line, char **out_key, char **out_value)
{
    int index;
    if ((index = index_of_char(line, ':')) > 0)
    {
        *out_key = substr(line, 0, index);
        *out_value = substr(line + index + 2, 0, (int)strlen(line) - index - 3);
    }
}

void http_headersitem_destroy(http_headersitem_t *http_headersitem)
{
    dictionaryitem_destroy(http_headersitem);
}

void http_headers_destroy(http_headers_t *http_headers)
{
    dictionary_destroy(http_headers);
}
