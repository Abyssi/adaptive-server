//
//  doublylinkedlist.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "doublylinkedlist.h"

doublylinkedlist_t *doublylinkedlist_init()
{
    doublylinkedlist_t *doublylinkedlist = malloc(sizeof(doublylinkedlist_t));
    exit_on_error(doublylinkedlist == NULL, "malloc: doublylinkedlist_t");
    
    doublylinkedlist->length = 0;
    doublylinkedlist->first = NULL;
    doublylinkedlist->last = NULL;
    PTHREAD_RWLOCK_INIT(doublylinkedlist->mt_safe_rwlock);
    return doublylinkedlist;
}

doublylinkedlistitem_t *doublylinkedlistitem_init(const void *item)
{
    doublylinkedlistitem_t *doublylinkedlistitem = malloc(sizeof(doublylinkedlistitem_t));
    exit_on_error(doublylinkedlistitem == NULL, "malloc: doublylinkedlistitem_t");
    
    doublylinkedlistitem->next = NULL;
    doublylinkedlistitem->prev = NULL;
    doublylinkedlistitem->item = item;
    return doublylinkedlistitem;
}

doublylinkedlistitem_t *doublylinkedlist_get(doublylinkedlist_t *doublylinkedlist, uint32_t index)
{
    if (index > 0 && index > doublylinkedlist->length) return NULL; // Out of range
    
    doublylinkedlistitem_t *iterator = NULL;
    
    if (index < doublylinkedlist->length / 2) {
        iterator = doublylinkedlist->first;
        for (uint32_t i = 0; i < index; i++) {
            iterator = iterator->next;  //Forward scan
        }
    }
    else
    {
        iterator = doublylinkedlist->last;
        for (uint32_t i = 0; i < doublylinkedlist->length - index - 1; i++) {
            iterator = iterator->prev;  //Backward scan
        }
        
    }
    return iterator;
}

const void *doublylinkedlist_get_item(doublylinkedlist_t *doublylinkedlist, uint32_t index)
{
    
    return doublylinkedlist_get(doublylinkedlist, index)->item;
}

void doublylinkedlist_insert(doublylinkedlist_t *doublylinkedlist, uint32_t index, const void *item) {
    if(index > 0 && index > doublylinkedlist->length) return;
    
    if (index == 0) doublylinkedlist_push(doublylinkedlist, item);
    else if (index == doublylinkedlist->length - 1) doublylinkedlist_add(doublylinkedlist, item);
    
    else
    {
        doublylinkedlistitem_t *new_item = doublylinkedlistitem_init(item);
        doublylinkedlistitem_t *current_item = doublylinkedlist_get(doublylinkedlist, index);
        
        doublylinkedlistitem_t *prev_item = current_item->prev;
        doublylinkedlistitem_t *next_item = current_item;
        
        prev_item->next = new_item;
        new_item->prev = prev_item;
        
        next_item->prev = new_item;
        new_item->next = next_item;
        
        doublylinkedlist->length++;
    }
    
}

void doublylinkedlist_add(doublylinkedlist_t *doublylinkedlist, const void *item) {
    doublylinkedlistitem_t *new_item = doublylinkedlistitem_init(item);
    
    if (doublylinkedlist->first == NULL)
    {
        doublylinkedlist->first = new_item;
        doublylinkedlist->last = new_item;
    }
    else
    {
        doublylinkedlist->last->next = new_item;
        new_item->prev = doublylinkedlist->last;
        doublylinkedlist->last = new_item;
    }
    
    doublylinkedlist->length++;
}

void doublylinkedlist_push(doublylinkedlist_t *doublylinkedlist, const void *item) {
    doublylinkedlistitem_t *new_item = doublylinkedlistitem_init(item);
    
    if (doublylinkedlist->first == NULL)
    {
        doublylinkedlist->first = new_item;
        doublylinkedlist->last = new_item;
    }
    else
    {
        doublylinkedlist->first->prev = new_item;
        new_item->next = doublylinkedlist->first;
        doublylinkedlist->first = new_item;
    }
    
    doublylinkedlist->length++;
}

void doublylinkedlist_remove(doublylinkedlist_t *doublylinkedlist, uint32_t index) {
    if(index > doublylinkedlist->length) return;
    
    doublylinkedlistitem_t *current_item = doublylinkedlist_get(doublylinkedlist, index);
    
    doublylinkedlistitem_t *prev_item = current_item->prev;
    doublylinkedlistitem_t *next_item = current_item->next;
    
    if (prev_item != NULL) prev_item->next = next_item ?: NULL;
    if (next_item != NULL) next_item->prev = prev_item ?: NULL;
    
    if (index == 0) doublylinkedlist->first = next_item;
    if (index == doublylinkedlist->length - 1) doublylinkedlist->last = prev_item;
    
    doublylinkedlistitem_destroy(current_item);
    
    doublylinkedlist->length--;
}

void doublylinkedlist_move(doublylinkedlist_t *doublylinkedlist, uint32_t before_index, uint32_t after_index) {
    if (before_index == after_index) return; /// Nothing to do
    if (before_index >= doublylinkedlist->length) return; // Out of range
    if (after_index >= doublylinkedlist->length) return; // Out of range
    
    const void *item = doublylinkedlist_get_item(doublylinkedlist, before_index);
    doublylinkedlist_insert(doublylinkedlist, after_index, item);
    
    doublylinkedlist_remove(doublylinkedlist, before_index + (before_index > after_index ? 1 : 0));
}

void doublylinkedlistitem_destroy(doublylinkedlistitem_t *item)
{
    free(item);
}

void doublylinkedlist_destroy(doublylinkedlist_t *doublylinkedlist)
{
    doublylinkedlistitem_t *iterator = doublylinkedlist->first;
    for (int i = 0; i < doublylinkedlist->length; i++) {
        doublylinkedlistitem_t *tmp = iterator;
        iterator = iterator->next;
        doublylinkedlistitem_destroy(tmp);
    }
    PTHREAD_RWLOCK_DESTROY(doublylinkedlist->mt_safe_rwlock);
}

void doublylinkedlist_enumerate(doublylinkedlist_t *doublylinkedlist, void (*functor)(const void *item), int forward)
{
    if (doublylinkedlist->length == 0) return;
    
    doublylinkedlistitem_t *iterator = NULL;
    
    if (forward) {
        iterator = doublylinkedlist->first;
        for (uint32_t i = 0; i < doublylinkedlist->length; i++) {
            functor(iterator->item);
            iterator = iterator->next;  //Forward scan
        }
    }
    else
    {
        iterator = doublylinkedlist->last;
        for (uint32_t i = 0; i < doublylinkedlist->length; i++) {
            functor(iterator->item);
            iterator = iterator->prev;  //Backward scan
        }
    }
}
