//
//  http_header.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/04/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef http_header_h
#define http_header_h

#include "../utils/error.h"
#include "../utils/dictionary.h"
#include "../utils/string.h"

typedef dictionary_t http_headers_t;

typedef dictionaryitem_t http_headersitem_t;

/*
	Initializes headers (dictionary)
	@return: pointer to the headers struct
 */
http_headers_t *http_headers_init();

/*
	Initializes headers item (dictionary)
	@param key: key for the headers item
	@param value: value for the headers item
	@return: pointer to the headers item struct
 */
http_headersitem_t *http_headersitem_init(const char *key, const void *value);

/*
	Adds (key, value) to the headers
	@param key: pointer to the key
	@param value: pointer to the value
 */
void http_headers_add(http_headers_t *http_headers, const char *key, const void *value);

/*
	Removes a (key, value) at a certain key
	@param key: pointer to the key
	@param value: pointer to the value
 */
void http_headers_remove(http_headers_t *http_headers, const char *key);

/*
	Verifies if the headers dictionary contains an object associated to a certain key
	@return: 0 if not contained, 1 if contained
 */
int http_headers_contains(http_headers_t *http_headers, const char *key);

/*	Gets the dictionary value from the key
	@param key: the key to be searched
	@return: pointer to the dictionary item
 */
const char *http_headers_get(http_headers_t *http_headers, const char *key);

/*
	Returns index of a key
	@param key: the key to be searched
	@return: index unsigned int
 */
uint32_t http_headers_index(http_headers_t *http_headers, const char *key);

/*
	Gets and allocates a string for the header
	@param headersitem: the item from which the string has to be taken
	@return: the pointer to the area in which string has been written
 */
char *http_header_get_string(http_headersitem_t *headersitem);

/*
	Parses a header line by assigning key and value
	@param headersitem: the item in which it has to read
	@param out_key: pointer to the area in which key has to be written
	@param out_value: pointer to the area in which value has to be written
 */
void parse_header_line(char *line, char **out_key, char **out_value);

/*
	Deallocates an item from memory
	@param item: pointer to the item to be destroyed
 */
void http_headersitem_destroy(http_headersitem_t *http_headersitem);

/*
	Deallocates the headers dictionary from memory
	@param dictionary: pointer to the dictionary to be destroyed
 */
void http_headers_destroy(http_headers_t *http_headers);

#endif /* http_header_h */
