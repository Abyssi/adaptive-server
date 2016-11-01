//
//  lrulist.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef lrulist_h
#define lrulist_h

#include <stdio.h>

#include "doublylinkedlist.h"

typedef struct {
    uint32_t max_size;
    int (*on_remove)(const void *item);
    
    doublylinkedlist_t *doublylinkedlist;
} lrulist_t;

typedef doublylinkedlistitem_t lrulistitem_t;

/*
	Initializes doubly linked list data structure
	@return: the pointer to the list
 */
lrulist_t *lrulist_init(uint32_t max_size, int (*on_remove)(const void *item));

/*
	Initializes lru list item
	@param item: pointer to the item
	@return: pointer to the list item
 */
lrulistitem_t *lrulistitem_init(const void *item);

/*
	Adds an item at the beginning of the list
	@param item: pointer to the item to push
 */
void lrulist_push(lrulist_t *lrulist, const void *item);

/*
	Removes an item at a certain index
	@param index: the index of the item to remove
 */
int lrulist_remove(lrulist_t *lrulist, uint32_t index);

/*	Gets the list item beginning from the half in which it is contained
	@param index: the index of the searched item
	@return: pointer to the list item
 */
const void *lrulist_get(lrulist_t *lrulist, uint32_t index);

/*
	Sets a list item as used
	@param index: the index of the item
 */
void lrulist_set_used(lrulist_t *lrulist, uint32_t index);

/*
	Deallocates an item from memory
	@param item: pointer to the item to be destroyed
 */
void lrulistitem_destroy(lrulistitem_t *lrulistitem);

/*
	Deallocates the list from memory
	@param doublylinkedlist: pointer to the list to be destroyed
 */
void lrulist_destroy(lrulist_t *dictionary);

#endif /* lrulist_h */
