//
//  http_response.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/04/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef http_response_h
#define http_response_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if defined(__linux__) || defined(__sun)
#include <sys/sendfile.h>
#elif defined(__APPLE__) || defined(__FreeBSD__)
#include <sys/socket.h>
#endif

#include "http_header.h"

typedef enum {
    OK = 200,
    BadRequest = 400,
    NotFound = 404,
} http_status_code_enum;

typedef struct http_response_t {
    float http_version;
    http_status_code_enum status;
    http_headers_t *headers;
    
    const char *path, *body; //Once at time
    uint64_t body_lenght;
} http_response_t;

http_response_t *http_response_init();

const char *http_response_status_string(http_response_t *response);

const char *http_response_final_status_string(http_response_t *response);

void http_response_write(http_response_t *response, int destination);

void http_response_destroy(http_response_t *http_response);

#endif /* http_response_h */
