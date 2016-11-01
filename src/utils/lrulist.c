//
//  lrulist.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "lrulist.h"

lrulist_t *lrulist_init(uint32_t max_size, int (*on_remove)())
{
    lrulist_t *lrulist = malloc(sizeof(lrulist_t));
    exit_on_error(lrulist == NULL, "malloc: lrulist_t");
    
    lrulist->max_size = max_size;
    lrulist->on_remove = on_remove;
    lrulist->doublylinkedlist = doublylinkedlist_init();
    return lrulist;
}

lrulistitem_t *lrulistitem_init(const void *item)
{
    return doublylinkedlistitem_init(item);
}

void lrulist_push(lrulist_t *lrulist, const void *item)
{
    while (lrulist->doublylinkedlist->length >= lrulist->max_size) {
        if (!lrulist_remove(lrulist, lrulist->doublylinkedlist->length - 1))
            break;
    }
    doublylinkedlist_push(lrulist->doublylinkedlist, item);
}

int lrulist_remove(lrulist_t *lrulist, uint32_t index)
{
    int r = lrulist->on_remove(lrulist_get(lrulist, index));
    if (r) doublylinkedlist_remove(lrulist->doublylinkedlist, index);
    return r;
}

const void *lrulist_get(lrulist_t *lrulist, uint32_t index)
{
    return doublylinkedlist_get_item(lrulist->doublylinkedlist, index);
}

void lrulist_set_used(lrulist_t *lrulist, uint32_t index)
{
    doublylinkedlist_move(lrulist->doublylinkedlist, index, 0);
}

void lrulistitem_destroy(lrulistitem_t *lrulistitem)
{
    doublylinkedlistitem_destroy(lrulistitem);
}

void lrulist_destroy(lrulist_t *dictionary)
{
    doublylinkedlist_destroy(dictionary->doublylinkedlist);
}

