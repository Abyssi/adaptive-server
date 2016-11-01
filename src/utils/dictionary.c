//
//  dictionary.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "dictionary.h"

dictionary_t *dictionary_init()
{
    return singlylinkedlist_init();
}

dictionaryitem_t *dictionaryitem_init(const char *key, const void *value)
{
    dictionaryitem_t *dictionaryitem = malloc(sizeof(dictionaryitem_t));
    exit_on_error(dictionaryitem == NULL, "malloc: dictionaryitem_t");
    
    dictionaryitem->key = string_copy(key);
    dictionaryitem->value = string_copy(value);
    return dictionaryitem;
}

void dictionary_add(dictionary_t *dictionary, const char *key, const void *value)
{
    singlylinkedlist_add(dictionary, dictionaryitem_init(key, value));
}

void dictionary_remove(dictionary_t *dictionary, const char *key)
{
    uint32_t index = dictionary_index(dictionary, key);
    if (index == UINT32_MAX) return;
    singlylinkedlist_remove(dictionary, index);
}

int dictionary_contains(dictionary_t *dictionary, const char *key)
{
    uint32_t index = dictionary_index(dictionary, key);
    return index != UINT32_MAX;
}

const void *dictionary_get(dictionary_t *dictionary, const char *key)
{
    uint32_t index = dictionary_index(dictionary, key);
    if (index == UINT32_MAX) return NULL;
    return ((dictionaryitem_t *)singlylinkedlist_get(dictionary, index)->item)->value;
}

uint32_t dictionary_index(dictionary_t *dictionary, const char *key)
{
    if (dictionary->length == 0) return UINT32_MAX;
    
    singlylinkedlistitem_t *iterator = dictionary->first;
    for (int i = 0; i < dictionary->length; i++) {
        dictionaryitem_t *dictionaryitem = (dictionaryitem_t *)iterator->item;
        if (streq((char *)dictionaryitem->key, (char *)key)) {
            return i;
        }
        iterator = iterator->next;
    }
    return UINT32_MAX;
}

void dictionaryitem_destroy(dictionaryitem_t *item)
{
    free((void *)item->key);
    free((void *)item->value);
    free(item);
}

void dictionary_destroy(dictionary_t *dictionary)
{
    singlylinkedlist_destroy(dictionary);
}
