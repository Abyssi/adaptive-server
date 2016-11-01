//
//  singlylinkedlist.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 28/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "singlylinkedlist.h"

singlylinkedlist_t *singlylinkedlist_init()
{
    singlylinkedlist_t *singlylinkedlist = malloc(sizeof(singlylinkedlist_t));
    exit_on_error(singlylinkedlist == NULL, "malloc: singlylinkedlist_t");
    
    singlylinkedlist->length = 0;
    singlylinkedlist->first = NULL;
    PTHREAD_RWLOCK_INIT(singlylinkedlist->mt_safe_rwlock);
    return singlylinkedlist;
}

singlylinkedlistitem_t *singlylinkedlistitem_init(const void *item)
{
    singlylinkedlistitem_t *singlylinkedlistitem = malloc(sizeof(singlylinkedlistitem_t));
    exit_on_error(singlylinkedlistitem == NULL, "malloc: singlylinkedlistitem_t");
    
    singlylinkedlistitem->next = NULL;
    singlylinkedlistitem->item = item;
    return singlylinkedlistitem;
}

singlylinkedlistitem_t *singlylinkedlist_get(singlylinkedlist_t *singlylinkedlist, uint32_t index)
{
    if (index >= singlylinkedlist->length) return NULL; // Out of range
    
    singlylinkedlistitem_t *iterator = singlylinkedlist->first;
    for (uint32_t i = 0; i < index; i++) {
        iterator = iterator->next;  //Forward scan
    }
    return iterator;
}

singlylinkedlistitem_t *singlylinkedlist_unlink(singlylinkedlist_t *singlylinkedlist, uint32_t index) {
    if(index >= singlylinkedlist->length) return NULL;
    
    singlylinkedlistitem_t *current_item = singlylinkedlist_get(singlylinkedlist, index);
    
    singlylinkedlistitem_t *prev_item = index > 0 ? singlylinkedlist_get(singlylinkedlist, index - 1) : NULL;
    singlylinkedlistitem_t *next_item = index < singlylinkedlist->length ? current_item->next : NULL;
    
    if (prev_item != NULL) prev_item->next = next_item ?: NULL;
    else if (index == 0) singlylinkedlist->first = next_item;
    
    singlylinkedlist->length--;
    
    return current_item;
}

const void *singlylinkedlist_get_item(singlylinkedlist_t *singlylinkedlist, uint32_t index)
{
    return singlylinkedlist_get(singlylinkedlist, index)->item;
}

void singlylinkedlist_insert(singlylinkedlist_t *singlylinkedlist, uint32_t index, const void *item) {
    if(index > 0 && index > singlylinkedlist->length) return;
    
    if (index == 0) {
        singlylinkedlist_push(singlylinkedlist, item);
    }
    else
    {
        singlylinkedlistitem_t *new_item = singlylinkedlistitem_init(item);
        
        singlylinkedlistitem_t *prev_item = singlylinkedlist_get(singlylinkedlist, index - 1);
        singlylinkedlistitem_t *next_item = prev_item->next;
        
        prev_item->next = new_item;
        new_item->next = next_item;
        
        singlylinkedlist->length++;
    }
    
}

void singlylinkedlist_add(singlylinkedlist_t *singlylinkedlist, const void *item)
{
    singlylinkedlist_insert(singlylinkedlist, singlylinkedlist->length, item);
}

void singlylinkedlist_push(singlylinkedlist_t *singlylinkedlist, const void *item) {
    singlylinkedlistitem_t *new_item = singlylinkedlistitem_init(item);
    
    if (singlylinkedlist->first == NULL)
    {
        singlylinkedlist->first = new_item;
    }
    else
    {
        new_item->next = singlylinkedlist->first;
        singlylinkedlist->first = new_item;
    }
    
    singlylinkedlist->length++;
}

void singlylinkedlist_remove(singlylinkedlist_t *singlylinkedlist, uint32_t index) {
    singlylinkedlistitem_t *current_item = singlylinkedlist_unlink(singlylinkedlist, index);
    if (current_item) singlylinkedlistitem_destroy(current_item);
}

void singlylinkedlist_move(singlylinkedlist_t *singlylinkedlist, uint32_t before_index, uint32_t after_index) {
    if (before_index == after_index) return; /// Nothing to do
    if (before_index >= singlylinkedlist->length) return; // Out of range
    if (after_index >= singlylinkedlist->length) return; // Out of range
    
    const void *item = singlylinkedlist_get_item(singlylinkedlist, before_index);
    singlylinkedlist_insert(singlylinkedlist, after_index, item);
    
    singlylinkedlist_remove(singlylinkedlist, before_index);
}

void singlylinkedlistitem_destroy(singlylinkedlistitem_t *item)
{
    free(item);
}

void singlylinkedlist_destroy(singlylinkedlist_t *singlylinkedlist)
{
    singlylinkedlistitem_t *iterator = singlylinkedlist->first;
    for (int i = 0; i < singlylinkedlist->length; i++) {
        singlylinkedlistitem_t *tmp = iterator;
        iterator = iterator->next;
        singlylinkedlistitem_destroy(tmp);
    }
    PTHREAD_RWLOCK_DESTROY(singlylinkedlist->mt_safe_rwlock);
}
