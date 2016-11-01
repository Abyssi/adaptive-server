//
//  http_request.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/04/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "http_request.h"

http_request_t *http_request_init()
{
    http_request_t *http_request = malloc(sizeof(http_request_t));
    exit_on_error(http_request == NULL, "malloc: http_request_t");
    
    http_request->method = UNSUPPORTED;
    http_request->path = NULL;
    http_request->headers = http_headers_init();
    return http_request;
}

http_request_t *http_request_init_with_request_line(const char *line)
{
    http_request_t *request = http_request_init();
    
    int index;
    if ((index = index_of_char(line, ' ')) > 0)
    {
        char *method_string = substr(line, 0, (int)index);
        
        if(streq(method_string, "GET"))
            request->method = GET;
        else if(streq(method_string, "HEAD"))
            request->method = HEAD;
        else
            request->method = UNSUPPORTED;
        
        free(method_string);
    }
    
    line += index + 1;
    
    if ((index = index_of_char(line, ' ')) > 0)
    {
        char *path_string = substr(line, 0, (int)index);
        request->path = path_string;
        
        urldecode2(request->path, request->path); // In loco
    }
    
    line += index + 1;
    
    if ((index = index_of_char(line, '/')) > 0) //Skip HTTP/
    {
        line += index + 1;
        
        const char *http_version_string = line;
        request->http_version = atof(http_version_string);
    }
    return request;
}

int http_request_is_valid(http_request_t *http_request)
{
    //check method
    int http_method_test_is_valid = http_request->method != UNSUPPORTED;
    
    //check path
    char *traversal_attack_bad_string = ".."; // Check if its a directory traversal attack
    int traversal_attack_test_is_valid = strstr(http_request->path, traversal_attack_bad_string) == NULL;
    
    return http_method_test_is_valid && traversal_attack_test_is_valid;
}

const char *http_request_method_string(http_request_t *http_request)
{
    switch (http_request->method) {
        case GET:
            return "GET";
            
        case HEAD:
            return "HEAD";
            
        default:
            return NULL;
    }
}

const float http_request_get_relative_quality_parameter(http_request_t *http_request, const char *mime)
{
    float relative_quality_parameter = 1.; // Default
    const char *accept_header_value = http_headers_get(http_request->headers, "Accept");
    
    char mime_first[256];
    char mime_second[256];
    
    sscanf(mime, "%s/%s", mime_first, mime_second);
    
    const char *mime_asterisk = string_by_appending_string(mime_first, "/*");
    const char *asterisk_mime = string_by_appending_string("*/", mime_second);
    const char *asterisk_asterisK = "*/*";
    
    int accept_parameters_count;
    char ***accept_parameters = split_parameters((char *)accept_header_value, &accept_parameters_count);
    
    char *current_accept_parameters_mime = NULL;
    char *current_accept_parameters_mime_q_value = NULL;
    
    for (int i = 0; i < accept_parameters_count; i++) {
        current_accept_parameters_mime = accept_parameters[i][0];
        current_accept_parameters_mime_q_value = accept_parameters[i][1];
        
        if(streq(current_accept_parameters_mime, mime) ||
           streq(current_accept_parameters_mime, mime_asterisk) ||
           streq(current_accept_parameters_mime, asterisk_mime) ||
           streq(current_accept_parameters_mime, asterisk_asterisK))
        {
            if (current_accept_parameters_mime_q_value)
                sscanf(current_accept_parameters_mime_q_value, "q=%f", &relative_quality_parameter);
            
            break;
        }
    }
    
    free((void *)mime_asterisk);
    free((void *)asterisk_mime);
    free(accept_parameters);
    
    return relative_quality_parameter;
}

void http_request_destroy(http_request_t *http_request)
{
    http_headers_destroy(http_request->headers);
    free((void *)http_request->path);
    free(http_request);
}
