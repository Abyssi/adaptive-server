//
//  http_response.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/04/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "http_response.h"

static const char *http_status_code_strings[] = {
    "OK", "Bad Request", "Not Found",
};

http_response_t *http_response_init()
{
    http_response_t *http_response = malloc(sizeof(http_response_t));
    exit_on_error(http_response == NULL, "malloc: http_response_t");
    
    http_response->status = OK;
    http_response->headers = http_headers_init();
    http_response->path = NULL;
    http_response->body = NULL;
    http_response->body_lenght = 0;
    
    return http_response;
}

const char *http_response_status_string(http_response_t *response)
{
    switch (response->status) {
        case OK:
            return http_status_code_strings[0];
            
        case BadRequest:
            return http_status_code_strings[1];
            
        case NotFound:
            return http_status_code_strings[2];
            
        default:
            return NULL;
    }
}

const char *http_response_final_status_string(http_response_t *response)
{
    char *http_version_string = response->http_version == 1. ? "1.0" : "1.1";
    const char *http_status_string = http_response_status_string(response);
    unsigned long string_length = strlen("HTTP/") + strlen(http_version_string) + intlen(response->status) + strlen(http_status_string) + 4;
    char *buffer = malloc(string_length * sizeof(char *));
    snprintf(buffer, string_length, "HTTP/%s %s %s\n", http_version_string, str(response->status), http_status_string);
    return buffer;
}

void http_response_write(http_response_t *response, int destination)
{
    const char *satus_string = http_response_final_status_string(response);
    write(destination, satus_string, strlen(satus_string));
    free((void *)satus_string);
    
    if (response->headers->length > 0)
    {
        singlylinkedlistitem_t *iterator = response->headers->first;
        for (int i = 0; i < response->headers->length; i++) {
            http_headersitem_t *http_headersitem = (http_headersitem_t *)iterator->item;
            
            char *header_string = http_header_get_string(http_headersitem);
            write(destination, header_string, strlen(header_string));
            
            free(header_string);
            
            iterator = iterator->next;
        }
    }
    
    write(destination, "\n", 1);
    
    if (response->path != NULL) {
        int rc;
        int fd = open(response->path, O_RDONLY);
        
        exit_on_error(fd == -1, "open");
        
#if defined(__linux__) || defined(__sun)
        rc = sendfile(destination, fd, 0, response->body_lenght);
#elif defined(__APPLE__) || defined(__FreeBSD__)
        off_t *content_length = (off_t *)&(response->body_lenght);
        rc = sendfile(fd, destination, 0, content_length, NULL, 0);
#endif
        log_on_error(rc == -1, "sendfile");
        
        close(fd);
    }
    else if (response->body != NULL)
    {
        write(destination, response->body, response->body_lenght);
    }
    
}

void http_response_destroy(http_response_t *http_response)
{
    http_headers_destroy(http_response->headers);
    if(http_response->body) free((void *)http_response->body);
    if(http_response->path) free((void *)http_response->path);
    free(http_response);
}

