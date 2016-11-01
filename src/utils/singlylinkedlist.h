//
//  singlylinkedlist.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 28/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef singlylinkedlist_h
#define singlylinkedlist_h

#include <stdio.h>
#include <stdint.h>
#include "pthread.h"

#include "error.h"

typedef struct singlylinkedlist_t {
    uint32_t length;
    struct singlylinkedlistitem_t *first;
    
    pthread_rwlock_t mt_safe_rwlock;
} singlylinkedlist_t;

typedef struct singlylinkedlistitem_t {
    const void *item;
    struct singlylinkedlistitem_t *next;
} singlylinkedlistitem_t;

/*
	Initializes singly linked list data structure
	@return: the pointer to the list
 */
singlylinkedlist_t *singlylinkedlist_init();

/*
	Initializes singly linked list item
	@param item: pointer to the item
	@return: pointer to the list item
 */
singlylinkedlistitem_t *singlylinkedlistitem_init(const void *item);

/*	Gets the list item beginning from the half in which it is contained
	@param index: the index of the searched item
	@return: pointer to the list item
 */
singlylinkedlistitem_t *singlylinkedlist_get(singlylinkedlist_t *singlylinkedlist, uint32_t index);

singlylinkedlistitem_t *singlylinkedlist_unlink(singlylinkedlist_t *singlylinkedlist, uint32_t index);

/*
	Gets the real item
	@param index: the index of the searched item
	@return: pointer to the item
 */
const void *singlylinkedlist_get_item(singlylinkedlist_t *singlylinkedlist, uint32_t index);

/*
	Inserts an item in the doubly linked list
	@param index: the index where to insert the item
 */
void singlylinkedlist_insert(singlylinkedlist_t *singlylinkedlist, uint32_t index, const void *item);

/*
	Adds an item at the end of the list
	@param item: pointer to the item to add
 */
void singlylinkedlist_add(singlylinkedlist_t *singlylinkedlist, const void *item);

/*
	Adds an item at the beginning of the list
	@param item: pointer to the item to push
 */
void singlylinkedlist_push(singlylinkedlist_t *singlylinkedlist, const void *item);

/*
	Removes an item at a certain index
	@param index: the index of the item to remove
 */
void singlylinkedlist_remove(singlylinkedlist_t *singlylinkedlist, uint32_t index);

/*
	Moves an item from an index to another
	@param before_index: index to change
	@param after_index: target index
 */
void singlylinkedlist_move(singlylinkedlist_t *singlylinkedlist, uint32_t before_index, uint32_t after_index);

/*
	Deallocates an item from memory
	@param item: pointer to the item to be destroyed
 */
void singlylinkedlistitem_destroy(singlylinkedlistitem_t *item);

/*
	Deallocates the list from memory
	@param item: pointer to the list to be destroyed
 */
void singlylinkedlist_destroy(singlylinkedlist_t *singlylinkedlist);

#endif /* singlylinkedlist_h */
