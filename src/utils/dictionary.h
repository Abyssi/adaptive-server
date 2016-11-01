//
//  dictionary.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef dictionary_h
#define dictionary_h

#include <stdio.h>

#include "string.h"
#include "singlylinkedlist.h"

typedef singlylinkedlist_t dictionary_t;

typedef struct {
    const char *key;
    const void *value;
} dictionaryitem_t;

/*
	Initializes dictionary data structure
	@return: the pointer to the dictionary
 */
dictionary_t *dictionary_init();

/*
	Initializes dictionary item
	@param key: pointer to the key
	@param value: pointer to the value
	@return: pointer to the dictionary item
 */
dictionaryitem_t *dictionaryitem_init(const char *key, const void *value);

/*
	Adds (key, value) to the dictionary
	@param key: pointer to the key
	@param value: pointer to the value
 */
void dictionary_add(dictionary_t *dictionary, const char *key, const void *value);

/*
	Removes a (key, value) at a certain key
	@param key: pointer to the key
	@param value: pointer to the value
 */
void dictionary_remove(dictionary_t *dictionary, const char *key);

/*
	Verifies if the dictionary contains an object associated to a certain key
	@return: 0 if not contained, 1 if contained
 */
int dictionary_contains(dictionary_t *dictionary, const char *key);

/*	Gets the dictionary value from the key
	@param key: the key to be searched
	@return: pointer to the dictionary item
 */
const void *dictionary_get(dictionary_t *dictionary, const char *key);
/*
	Returns index of a key
	@param key: the key to be searched
	@return: index unsigned int
 */
uint32_t dictionary_index(dictionary_t *dictionary, const char *key);

/*
	Deallocates an item from memory
	@param item: pointer to the item to be destroyed
 */
void dictionaryitem_destroy(dictionaryitem_t *item);

/*
	Deallocates the dictionary from memory
	@param dictionary: pointer to the dictionary to be destroyed
 */
void dictionary_destroy(dictionary_t *dictionary);

#endif /* dictionary_h */
