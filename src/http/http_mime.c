//
//  http_mime.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 11/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "http_mime.h"

http_mimes_t *http_mimes_init_from_file(const char *file_path)
{
    http_mimes_t *http_mimes = http_mimes_init();
    FILE *fp;
    
    char mime_line[1024];
    char mime_extensions_string[512];
    char mime_mime_string[512];
    
    exit_on_error((fp = fopen(file_path, "r")) == NULL, "fopen");
    
    while(!feof(fp))
    {
        fgets(mime_line, sizeof(mime_line), fp);
        
        if (mime_line[0] == '#' || mime_line[0] == '\n') continue; //Skip lines staring with these chars
        
        sscanf(mime_line, "%s	%[^\n]", mime_mime_string, mime_extensions_string);
        
        int rc;
        char **mime_extensions;
        int mime_extensions_count;
        
        rc = str_split(mime_extensions_string, ' ', &mime_extensions, &mime_extensions_count);
        if(rc) printf("Error: %s errno: %d \n", strerror(errno), errno);
        
        for (int i = 0; i < mime_extensions_count; i++) {
            http_mimes_add(http_mimes, mime_extensions[i], mime_mime_string);
        }
        
        free(mime_extensions);
    }
    
    fclose(fp);
    return http_mimes;
}

http_mimes_t *http_mimes_init()
{
    return dictionary_init();
}

http_mimeitem_t *http_mimeitem_init(const char *extension, const char *mime)
{
    return dictionaryitem_init(extension, mime);
}

void http_mimes_add(http_mimes_t *http_mimes, const char *extension, const char *mime)
{
    dictionary_add(http_mimes, extension, mime);
}

void http_mimes_remove(http_mimes_t *http_mimes, const char *extension)
{
    dictionary_remove(http_mimes, extension);
}

int http_mimes_contains(http_mimes_t *http_mimes, const char *extension)
{
    return dictionary_contains(http_mimes, extension);
}

const char *http_mimes_get(http_mimes_t *http_mimes, const char *extension)
{
    return dictionary_get(http_mimes, extension);
}

const char *http_mime_get_from_file_path(http_mimes_t *http_mimes, const char *file_path)
{
    const char *file_extension = file_path_extension(file_path);
    const char *mime = http_mimes_get(http_mimes, file_extension) ?: "application/octet-stream";
    free((void *)file_extension);
    return mime;
}
