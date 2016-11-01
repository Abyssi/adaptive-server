//
//  http_request.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/04/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef http_request_h
#define http_request_h

#include "http_header.h"

#include "../utils/string.h"

typedef enum { GET, HEAD, UNSUPPORTED } http_method_enum;

typedef struct http_request_t {
    http_method_enum method;
    char *path;
    float http_version;
    http_headers_t *headers;
} http_request_t;

/*
	Initializes a request
 */
http_request_t *http_request_init();

/*
	
 */
http_request_t *http_request_init_with_request_line(const char *line);

int http_request_is_valid(http_request_t *http_request);

const char *http_request_method_string(http_request_t *http_request);

const float http_request_get_relative_quality_parameter(http_request_t *http_request, const char *mime);

void http_request_destroy(http_request_t *http_request);

#endif /* http_request_h */
