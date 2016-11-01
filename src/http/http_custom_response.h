//
//  http_custom_response.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 09/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef http_custom_response_h
#define http_custom_response_h

#include "http_request.h"
#include "http_response.h"
#include "http_header.h"
#include "http_mime.h"

#include "../utils/file.h"

/*
	Initializes a response message with basic headers
	@return: pointer to the response struct
 */
http_response_t *http_response_basic_init();

/*
	Initializes a response message of html type
	@param text: the text of html file (in body)
	@return: pointer to the response struct
 */
http_response_t *http_response_with_html(const char *text);


/*
	Initializes a response message with a file
	@param path: the path of the file
	@param mime: parameter to recognize the format of the file
	@return: pointer to the response struct
 */
http_response_t *http_response_with_file(const char *path, const char *mime);

/*
	Makes a 'bad request' response
	@return: pointer to the response struct
 */
http_response_t *http_response_bad_request();

/*
	Makes a 'file not found' response
	@return: pointer to the response struct
 */
http_response_t *http_response_file_not_found();

#endif /* http_custom_response_h */
