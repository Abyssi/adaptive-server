//
//  http_mime.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 11/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef http_mime_h
#define http_mime_h

#include <stdio.h>
#include <errno.h>

#include "../config/config.h"
#include "../utils/string.h"
#include "../utils/dictionary.h"

typedef dictionary_t http_mimes_t;

typedef dictionaryitem_t http_mimeitem_t;

/*
	Initializes mimes from a file in file_path
 */
http_mimes_t *http_mimes_init_from_file(const char *file_path);

/*
	Initializes empty mimes
 */
http_mimes_t *http_mimes_init();

/*
	Initializes an item for mime
 */
http_mimeitem_t *http_mimeitem_init(const char *extension, const char *mime);

/*
	Adds mime item to mimes
 */
void http_mimes_add(http_mimes_t *http_mimes, const char *extension, const char *mime);

/*
	Removes mime item from mimes
 */
void http_mimes_remove(http_mimes_t *http_mimes, const char *extension);

/*
	Verifies if mimes contains an extension
 */
int http_mimes_contains(http_mimes_t *http_mimes, const char *extension);

/*
	Gets mime from extension
 */
const char *http_mimes_get(http_mimes_t *http_mimes, const char *extension);

/*
	Gets mime from file path
 */
const char *http_mime_get_from_file_path(http_mimes_t *http_mimes, const char *file_path);

#endif /* http_mime_h */
