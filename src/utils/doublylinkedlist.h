//
//  doublylinkedlist.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef doublylinkedlist_h
#define doublylinkedlist_h

#include <stdio.h>
#include <stdint.h>
#include "pthread.h"

#include "error.h"

typedef struct doublylinkedlist_t {
    uint32_t length;
    struct doublylinkedlistitem_t *first;
    struct doublylinkedlistitem_t *last;
    
    pthread_rwlock_t mt_safe_rwlock;
} doublylinkedlist_t;

typedef struct doublylinkedlistitem_t {
    const void *item;
    struct doublylinkedlistitem_t *next;
    struct doublylinkedlistitem_t *prev;
} doublylinkedlistitem_t;

/*
	Initializes doubly linked list data structure
	@return: the pointer to the list
 */
doublylinkedlist_t *doublylinkedlist_init();

/*
	Initializes doubly linked list item
	@param item: pointer to the item
	@return: pointer to the list item
 */
doublylinkedlistitem_t *doublylinkedlistitem_init(const void *item);

/*	Gets the list item beginning from the half in which it is contained
	@param index: the index of the searched item
	@return: pointer to the list item
 */
doublylinkedlistitem_t *doublylinkedlist_get(doublylinkedlist_t *doublylinkedlist, uint32_t index);

/*
	Gets the real item
	@param index: the index of the searched item
	@return: pointer to the item
 */
const void *doublylinkedlist_get_item(doublylinkedlist_t *doublylinkedlist, uint32_t index);

/*
	Inserts an item in the doubly linked list
	@param index: the index where to insert the item
 */
void doublylinkedlist_insert(doublylinkedlist_t *doublylinkedlist, uint32_t index, const void *item);

/*
	Adds an item at the end of the list
	@param item: pointer to the item to add
 */
void doublylinkedlist_add(doublylinkedlist_t *doublylinkedlist, const void *item);

/*
	Adds an item at the beginning of the list
	@param item: pointer to the item to push
 */
void doublylinkedlist_push(doublylinkedlist_t *doublylinkedlist, const void *item);

/*
	Removes an item at a certain index
	@param index: the index of the item to remove
 */
void doublylinkedlist_remove(doublylinkedlist_t *doublylinkedlist, uint32_t index);

/*
	Moves an item from an index to another
	@param before_index: index to change
	@param after_index: target index
 */
void doublylinkedlist_move(doublylinkedlist_t *doublylinkedlist, uint32_t before_index, uint32_t after_index);

/*
	Deallocates an item from memory
	@param item: pointer to the item to be destroyed
 */
void doublylinkedlistitem_destroy(doublylinkedlistitem_t *item);

/*
	Deallocates the list from memory
	@param doublylinkedlist: pointer to the list to be destroyed
 */
void doublylinkedlist_destroy(doublylinkedlist_t *doublylinkedlist);
/*
	Enumerates the list and calls the functor function for each item
 */
void doublylinkedlist_enumerate(doublylinkedlist_t *doublylinkedlist, void (*functor)(const void *item), int forward);

#endif /* doublylinkedlist_h */
