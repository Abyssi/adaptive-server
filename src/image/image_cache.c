//
//  image_cache.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "image_cache.h"

image_cache_t *image_cache_init(const char *base_path, uint32_t max_size)
{
    image_cache_t *image_cache = malloc(sizeof(image_cache_t));
    exit_on_error(image_cache == NULL, "malloc: image_cache_t");
    
    image_cache->base_path = string_copy(base_path);
    image_cache->lrulist = lrulist_init(max_size, on_remove);
    image_cache->unflushed_operations_limit = max_size * shared_config->unflushed_cache_percentage;
    image_cache->unflushed_operations = 0;
    PTHREAD_RWLOCK_INIT(image_cache->mt_safe_rwlock);
    return image_cache;
}

image_cacheitem_t *image_cacheitem_init(const char *real_path, image_info_t *image_info)
{
    image_cacheitem_t *image_cacheitem = malloc(sizeof(image_cacheitem_t));
    exit_on_error(image_cacheitem == NULL, "malloc: image_cacheitem_t");
    
    image_cacheitem->real_path = string_copy(real_path);
    image_cacheitem->image_info = image_info;
    return image_cacheitem;
}

image_info_t *image_info_init(const char *base_path, float quality)
{
    image_info_t *image_info = malloc(sizeof(image_info_t));
    exit_on_error(image_info == NULL, "malloc: image_info_t");
    
    image_info->base_path = string_copy(base_path);
    image_info->quality = quality;
    return image_info;
}

void image_cache_add(image_cache_t *image_cache, const char *real_path, image_info_t *image_info)
{
    image_cache_add_item(image_cache, image_cacheitem_init(real_path, image_info));
}

void image_cache_add_item(image_cache_t *image_cache, image_cacheitem_t *image_cacheitem)
{
    lrulist_push(image_cache->lrulist, image_cacheitem);
    if (image_cache->unflushed_operations_limit != 0 && image_cache->unflushed_operations++ >= image_cache->unflushed_operations_limit - 1)
    {
        image_cache_save_to_file_path(image_cache, shared_config->cache_path);
        image_cache->unflushed_operations = 0;
    }
}

void image_cache_remove(image_cache_t *image_cache, image_info_t *image_info)
{
    uint32_t index = image_cache_index(image_cache, image_info);
    if (index == UINT32_MAX) return;
    lrulist_remove(image_cache->lrulist, index);
    if (image_cache->unflushed_operations_limit != 0 && image_cache->unflushed_operations++ >= image_cache->unflushed_operations_limit - 1)
    {
        image_cache_save_to_file_path(image_cache, shared_config->cache_path);
        image_cache->unflushed_operations = 0;
    }
}

int image_cache_contains(image_cache_t *image_cache, image_info_t *image_info)
{
    uint32_t index = image_cache_index(image_cache, image_info);
    return index != UINT32_MAX;
}

int image_cache_contains_file(image_cache_t *image_cache, const char *file_path)
{
    uint32_t index = image_cache_index_of_file(image_cache, file_path);
    return index != UINT32_MAX;
}

const char *image_cache_get(image_cache_t *image_cache, image_info_t *image_info)
{
    uint32_t index = image_cache_index(image_cache, image_info);
    if (index == UINT32_MAX) return NULL;
    image_cacheitem_t *image_cacheitem = ((image_cacheitem_t *)lrulist_get(image_cache->lrulist, index));
    lrulist_set_used(image_cache->lrulist, index);
    if (image_cache->unflushed_operations_limit != 0 && image_cache->unflushed_operations++ >= image_cache->unflushed_operations_limit - 1)
    {
        image_cache_save_to_file_path(image_cache, shared_config->cache_path);
        image_cache->unflushed_operations = 0;
    }
    return image_cacheitem->real_path;
}

uint32_t image_cache_index(image_cache_t *image_cache, image_info_t *image_info)
{
    if (image_cache->lrulist->doublylinkedlist->length == 0) return UINT32_MAX;
    lrulistitem_t *iterator = image_cache->lrulist->doublylinkedlist->first;
    for (int i = 0; i < image_cache->lrulist->doublylinkedlist->length; i++) {
        
        image_cacheitem_t *image_cacheitem = (image_cacheitem_t *)iterator->item;
        if (same_image_info(image_cacheitem->image_info, image_info)) {
            
            return i;
        }
        iterator = iterator->next;
    }
    return UINT32_MAX;
}

uint32_t image_cache_index_of_file(image_cache_t *image_cache, const char *file_path)
{
    if (image_cache->lrulist->doublylinkedlist->length == 0) return UINT32_MAX;
    lrulistitem_t *iterator = image_cache->lrulist->doublylinkedlist->first;
    for (int i = 0; i < image_cache->lrulist->doublylinkedlist->length; i++) {
        
        image_cacheitem_t *image_cacheitem = (image_cacheitem_t *)iterator->item;
        if (streq(image_cacheitem->real_path, file_path)) {
            
            return i;
        }
        iterator = iterator->next;
    }
    return UINT32_MAX;
}

int same_image_info(image_info_t *image_info_a, image_info_t *image_info_b)
{
    int same_path = streq(image_info_a->base_path, image_info_b->base_path);
    int same_quality = image_info_a->quality == image_info_b->quality;
    int same_max_height = image_info_a->max_height == image_info_b->max_height;
    int same_max_width = image_info_a->max_width == image_info_b->max_width;
    int same_colors = image_info_a->colors == image_info_b->colors;
    int same_extension = streq(image_info_a->extension, image_info_b->extension);
    
    return same_path && same_quality && same_max_height && same_max_width && same_colors && same_extension;
}

char *image_cache_new_real_path(image_cache_t *image_cache, char *file_extension)
{
    uint32_t filename_size = 20;
    uint32_t tot_filename_size = filename_size  + sizeof file_extension - 1;
    
    char *file_path;
    char random_string[filename_size + 1];
    char filename[tot_filename_size + 1];
    
    do {
        rand_string(random_string, filename_size);
        snprintf(filename, tot_filename_size, "%s.%s", random_string, file_extension);
        file_path = string_by_adding_path_component(image_cache->base_path, filename);
    } while (path_exists(file_path, NULL));
    
    return file_path;
}

int on_remove(const void *item)
{
    image_cacheitem_t *image_cacheitem = (image_cacheitem_t *)item;
    const char *file_path = image_cacheitem->real_path;
    
    int rc = remove(file_path);
    if(rc != 0) printf("Cache: File can not be deleted!\n");
    return rc == 0;
}

void image_info_destroy(image_info_t *image_info)
{
    free((void *)image_info->base_path);
    free(image_info->extension);
    free(image_info);
}

void image_cacheitem_destroy(image_cacheitem_t *image_cacheitem)
{
    free((void *)image_cacheitem->real_path);
    image_info_destroy(image_cacheitem->image_info);
    free(image_cacheitem);
}

void image_cache_destroy(image_cache_t *image_cache)
{
    free((void *)image_cache->base_path);
    lrulist_destroy(image_cache->lrulist);
    free(image_cache);
    PTHREAD_RWLOCK_DESTROY(image_cache->mt_safe_rwlock);
}

static inline void string_write_to_file(const char *string, FILE *fp)
{
    size_t length = strlen(string) + 1;
    fwrite(&length, sizeof(length), 1, fp);
    fwrite(string, 1, length, fp);
}

static inline void image_info_write_to_file(image_info_t *image_info, FILE *fp)
{
    string_write_to_file(image_info->base_path, fp);
    fwrite(&(image_info->quality), sizeof(image_info->quality), 1, fp);
    fwrite(&(image_info->max_height), sizeof(image_info->max_height), 1, fp);
    fwrite(&(image_info->max_width), sizeof(image_info->max_width), 1, fp);
    fwrite(&(image_info->colors), sizeof(image_info->colors), 1, fp);
    string_write_to_file(image_info->extension, fp);
}

static inline void image_cacheitem_write_to_file(image_cacheitem_t *image_cacheitem, FILE *fp)
{
    string_write_to_file(image_cacheitem->real_path, fp);
    image_info_write_to_file(image_cacheitem->image_info, fp);
}

void image_cache_save_to_file_path(image_cache_t *image_cache, const char *file_path)
{
    FILE *fp;
    
    exit_on_error((fp = fopen(shared_config->cache_path, "w")) == NULL, "fopen");
    
    fwrite(&(image_cache->lrulist->doublylinkedlist->length), sizeof(image_cache->lrulist->doublylinkedlist->length), 1, fp);
    
    lrulistitem_t *iterator = image_cache->lrulist->doublylinkedlist->first;
    for (int i = 0; i < image_cache->lrulist->doublylinkedlist->length; i++) {
        image_cacheitem_t *image_cacheitem = (image_cacheitem_t *)iterator->item;
        image_cacheitem_write_to_file(image_cacheitem, fp);
        iterator = iterator->next;
    }
    
    fclose(fp);
}

static inline const char *string_read_from_file(FILE *fp)
{
    size_t length;
    fread(&length, sizeof(length), 1, fp);
    const char *string = malloc(length);
    fread((void *)string, 1, length, fp);
    return string;
}

static inline image_info_t *image_info_read_from_file(FILE *fp)
{
    const char *base_path = string_read_from_file(fp);
    float quality;
    fread(&quality, sizeof(quality), 1, fp);
    
    image_info_t *image_info = image_info_init(base_path, quality);
    
    fread(&(image_info->max_height), sizeof(image_info->max_height), 1, fp);
    fread(&(image_info->max_width), sizeof(image_info->max_width), 1, fp);
    fread(&(image_info->colors), sizeof(image_info->colors), 1, fp);
    image_info->extension = (char *)string_read_from_file(fp);
    
    return image_info;
}

static inline image_cacheitem_t *image_cacheitem_read_from_file(FILE *fp)
{
    const char *real_path = string_read_from_file(fp);
    image_info_t *image_info = image_info_read_from_file(fp);
    image_cacheitem_t *image_cacheitem = image_cacheitem_init(real_path, image_info);
    return image_cacheitem;
}

image_cache_t *image_cache_init_from_file_path(const char *file_path, const char *base_path, uint32_t max_size)
{
    image_cache_t *image_cache = image_cache_init(base_path, max_size);
    image_cache->unflushed_operations_limit = 0; //disable temporanely flush to file
    
    int is_directory;
    if (!path_exists(file_path, &is_directory)) goto end;
    
    
    FILE *fp;
    
    exit_on_error((fp = fopen(shared_config->cache_path, "r")) == NULL, "fopen");
    
    uint32_t saved = 0;
    fread(&saved, sizeof(saved), 1, fp);
    
    uint32_t i = 0;
    while (i < saved && i < max_size && !feof(fp)) {
        image_cacheitem_t *image_cacheitem = image_cacheitem_read_from_file(fp);
        
        image_cache_add_item(image_cache, image_cacheitem);
        i++;
    }
    
    fclose(fp);
end:
    clear_exceding_files(image_cache);
    
    image_cache->unflushed_operations_limit = image_cache->lrulist->max_size * shared_config->unflushed_cache_percentage; //enable flush to file
    
    return image_cache;
}

void clear_exceding_files(image_cache_t *image_cache)
{
    DIR *theFolder = opendir(image_cache->base_path);
    struct dirent *next_file;
    char *filepath;
    
    while ((next_file = readdir(theFolder)) != NULL)
    {
        filepath = string_by_adding_path_component(image_cache->base_path, next_file->d_name);
        if (streq(filepath, shared_config->cache_path)) continue;
        if (!image_cache_contains_file(image_cache, filepath))
            remove(filepath);
        
    }
    
    closedir(theFolder);
    lrulistitem_t *iterator = image_cache->lrulist->doublylinkedlist->first;
    for (int i = 0; i < image_cache->lrulist->doublylinkedlist->length; i++) {
        image_cacheitem_t *image_cacheitem = (image_cacheitem_t *)iterator->item;
        int is_directory = 0;
        if (!path_exists(image_cacheitem->real_path, &is_directory) || is_directory) remove(image_cacheitem->real_path);
        iterator = iterator->next;
    }
    
}

