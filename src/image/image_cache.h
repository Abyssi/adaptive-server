//
//  image_cache.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef image_cache_h
#define image_cache_h

#include <stdio.h>
#include "pthread.h"

#include "../utils/string.h"
#include "../utils/lrulist.h"
#include "../utils/file.h"

typedef struct
{
    const char *base_path;
    
    float quality; //From Accept
    
    int max_height; //From libwurfl
    int max_width; //From libwurfl
    int colors; //From libwurfl
    
    char *extension; //From libwurfl
    
} image_info_t;

typedef struct
{
    const char *base_path;
    
    lrulist_t *lrulist;
    
    uint32_t unflushed_operations_limit;
    uint32_t unflushed_operations;
    
    pthread_rwlock_t mt_safe_rwlock;
} image_cache_t;

typedef struct
{
    const char *real_path;
    image_info_t *image_info;
} image_cacheitem_t;

/*
 Initializes the cache with a certain path and size
 @param base_path: path for the cache, set in the config file
 @param max_size: maximum size in terms of number of images
 @return: pointer to the cache structure
 */
image_cache_t *image_cache_init(const char *base_path, uint32_t max_size);

/*
 Initializes a cache item
 @param real_path: path of the item
 @param image_info: pointer to the image_info struct
 */
image_cacheitem_t *image_cacheitem_init(const char *real_path, image_info_t *image_info);

/*
 Initializes the structure for the information about the image
 @param base_path: path for the image
 @param quality: parameter for the quality requested
 @return: pointer to the image_info structure
 */
image_info_t *image_info_init(const char *base_path, float quality);

/*
 Adds an image with a certain path to the cache
 @param image_cache: pointer to the cache
 @param real_path: path for the cache, set in the config file
 @param image_info: pointer to the image_info struct
 */
void image_cache_add(image_cache_t *image_cache, const char *real_path, image_info_t *image_info);

void image_cache_add_item(image_cache_t *image_cache, image_cacheitem_t *image_cacheitem);

/*
 Removes an image from the cache
 @param image_cache: pointer to the cache
 @param image_info: pointer to the image_info struct
 */
void image_cache_remove(image_cache_t *image_cache, image_info_t *image_info);

/*
 Verifies if an image is in the cache
 @param image_cache: pointer to the cache
 @param image_info: pointer to the image_info struct
 */
int image_cache_contains(image_cache_t *image_cache, image_info_t *image_info);

int image_cache_contains_file(image_cache_t *image_cache, const char *file_path);

/*
 Gets an image from the cache
 @param image_cache: pointer to the cache
 @param image_info: pointer to the image_info struct
 */
const char *image_cache_get(image_cache_t *image_cache, image_info_t *image_info);

uint32_t image_cache_index(image_cache_t *image_cache, image_info_t *image_info);

uint32_t image_cache_index_of_file(image_cache_t *image_cache, const char *file_path);

/*
 Verifies if an image with an information is identical to another
 @param image_info_a: pointer to the image_info struct for image A
 @param image_info_b: pointer to the image_info struct for image B
 */
int same_image_info(image_info_t *image_info_a, image_info_t *image_info_b);

char *image_cache_new_real_path(image_cache_t *image_cache, char *file_extension);

int on_remove(const void *item);

/*
 Destroys an image_info struct
 @param image_info: pointer to the image_info struct
 */
void image_info_destroy(image_info_t *image_info);

/*
 Destroys an image from the cache
 @param image_cacheitem: pointer to the image_cahceitem struct
 */
void image_cacheitem_destroy(image_cacheitem_t *image_cacheitem);

/*
 Destroys the cache
 @param image_cache: pointer to the cache
 */
void image_cache_destroy(image_cache_t *image_cache);

/*
 Saves the cache to a file (persistance)
 @param image_cache: pointer to the cache struct
 @param file_path: the path of the file
 */
void image_cache_save_to_file_path(image_cache_t *image_cache, const char *file_path);

/*
 Initializes the cache from a file (persistance)
 @param file_path: the path of the file for the cache
 @param base_path: the path in which cache has to be taken
 @param max_size: the maximum size of the cache
 @return: pointer to the cache struct
 */
image_cache_t *image_cache_init_from_file_path(const char *file_path, const char *base_path, uint32_t max_size);

/*
 Clears the exceding files from the cache
 @param image_cache: pointer to the cache struct
 */
void clear_exceding_files(image_cache_t *image_cache);

#endif /* image_cache_h */
