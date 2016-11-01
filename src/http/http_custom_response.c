//
//  http_custom_response.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 09/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "http_custom_response.h"

#define kSERVER_ERROR_BAD_REQUEST_HTML "<html><body><H1>AdaptiveServer: Error 400<br>Bad Request</H1></body></html>"
#define kSERVER_ERROR_FILE_NOT_FOUND_HTML "<html><body><H1>AdaptiveServer: Error 404<br>File Not Found</H1></body></html>"

http_response_t *http_response_basic_init()
{
    http_response_t *response = http_response_init();
    http_headers_add(response->headers, "Server", "AdaptiveServer v1.0");
    http_headers_add(response->headers, "Connection", "keep-alive"); //Default in HTTP 1.1
    char formatted_time_string[30];
    formatted_time(formatted_time_string, sizeof(formatted_time_string), "%a, %d %b %Y %H:%M:%S %Z");
    http_headers_add(response->headers, "Date", formatted_time_string); //RFC2616 format
    return response;
}

http_response_t *http_response_with_html(const char *text)
{
    uint64_t body_lenght = strlen(text);
    
    http_response_t *response = http_response_basic_init();
    
    http_headers_add(response->headers, "Content-Type", "text/html");
    http_headers_add(response->headers, "Content-Length", str((int)body_lenght));
    
    response->body = string_copy(text);
    response->body_lenght = body_lenght;
    
    return response;
}

http_response_t *http_response_with_file(const char *path, const char *mime)
{
    uint64_t body_lenght = filesize(path);
    
    http_response_t *response = http_response_basic_init();
    
    http_headers_add(response->headers, "Content-Type", mime);
    http_headers_add(response->headers, "Content-Length", str((int)body_lenght));
    
    response->path = string_copy(path);
    
    response->body_lenght = body_lenght;
    
    return response;
}

http_response_t *http_response_bad_request()
{
    http_response_t *http_response = http_response_with_html(kSERVER_ERROR_BAD_REQUEST_HTML);
    http_response->status = BadRequest;
    return http_response;
}

http_response_t *http_response_file_not_found()
{
    http_response_t *http_response = http_response_with_html(kSERVER_ERROR_FILE_NOT_FOUND_HTML);
    http_response->status = NotFound;
    return http_response;
}
