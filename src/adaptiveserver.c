//
//  adaptiveserver.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/04/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include <wurfl.h>
#include <pthread.h>

#include "config/config.h"

#include "http/http_server.h"
#include "http/http_request.h"
#include "http/http_response.h"
#include "http/http_custom_response.h"

#include "image/image_manipulation.h"
#include "image/image_cache.h"

#include "utils/string.h"
#include "utils/logger.h"
#include "utils/pthread.h"

static wurfl_t *wurfl;
static image_cache_t *image_cache;
static http_mimes_t *http_mimes;

const char *translate_request_path_to_local_file_path(const char *request_path)
{
    const char *base_path = shared_config->server_directory;
    char *file_path = string_by_adding_path_component(base_path, request_path);
    int is_directory;
    if (!path_exists(file_path, &is_directory)) return NULL;
    if (is_directory) file_path = string_by_adding_path_component(file_path, "index.html");
    return file_path;
}

http_response_t *http_response_with_manipulated_image(http_server_t *http_server, http_request_t *http_request, const char *image_path, const char *mime)
{
    float relative_quality_parameter = http_request_get_relative_quality_parameter(http_request, mime);
    
    image_info_t *image_info = image_info_init(image_path, relative_quality_parameter);
    
    const char *user_agent_header_value = http_headers_get(http_request->headers, "User-Agent");
    
    device_t *device = wurfl_match(wurfl, user_agent_header_value);
    
    image_info->max_width = atoi(device_capability(device, "max_image_width"));
    image_info->max_height = atoi(device_capability(device, "max_image_height"));
    image_info->colors = atoi(device_capability(device, "colors"));
    
    char *preferred_file_extension = file_path_extension(image_path);
    
    char *preferred_file_extensions[] = {preferred_file_extension, "jpg", "png", "gif", NULL};
    for (char **iterator = preferred_file_extensions; *iterator != NULL; iterator++) {
        preferred_file_extension = *iterator;
        if (streq(device_capability(device, *iterator++), "true"))
            break;
    }
    image_info->extension = preferred_file_extension;
    
    do_log_with_format(2, "Device capabilities:\n - Quality: %f\n - Max height: %d\n - Max width: %d\n - Colors: %d\n - Extension: %s", image_info->quality, image_info->max_height, image_info->max_width, image_info->colors, image_info->extension);
    
    /* FINISHED PARSING PARAMETERS FROM HEADERS AND WURFL */
    
    const char *cached_file_path = NULL;
    
    PTHREAD_RWLOCK_RDLOCK(image_cache->mt_safe_rwlock);
    if (image_cache_contains(image_cache, image_info)) {
        do_log_with_format(1, "Cache(%d elements) hit", image_cache->lrulist->doublylinkedlist->length);
        cached_file_path = image_cache_get(image_cache, image_info);
    }
    else
    {
        do_log_with_format(1, "Cache(%d elements) miss", image_cache->lrulist->doublylinkedlist->length);
        cached_file_path = image_cache_new_real_path(image_cache, image_info->extension);
        
        PTHREAD_RWLOCK_UNLOCK(image_cache->mt_safe_rwlock);
        save_image_with_image_info(image_info, cached_file_path);
        
        PTHREAD_RWLOCK_WRLOCK(image_cache->mt_safe_rwlock);
        image_cache_add(image_cache, cached_file_path, image_info);
    }
    PTHREAD_RWLOCK_UNLOCK(image_cache->mt_safe_rwlock);
    
    device_free(device);
    
    return http_response_with_file(cached_file_path, mime);
}

http_response_t *dynamic_image_manipulation_response_handler(http_server_t *http_server, http_request_t *http_request)
{
    const char *file_path = translate_request_path_to_local_file_path(http_request->path);
    if (file_path == NULL) return http_response_file_not_found();
    const char *mime = http_mime_get_from_file_path(http_mimes, file_path);
    
    if (strstr(mime, "image/") != NULL)
        return http_response_with_manipulated_image(http_server, http_request, file_path, mime);
    
    return http_response_with_file(file_path, mime);
}

void at_exit(void){
    image_cache_save_to_file_path(image_cache, shared_config->cache_path);
}

int main(int argc, char *argv[])
{
    shared_config = config_init(argc, argv);
    
    do_log(0, "Developed by Andrea Silvi");
    
    image_cache = image_cache_init_from_file_path(shared_config->cache_path, shared_config->cache_directory, shared_config->cache_size);
    wurfl = wurfl_init(shared_config->wurfl_path, NULL);
    http_mimes = http_mimes_init_from_file(shared_config->mime_path);
    
    http_server_t *server = http_server_init(shared_config->threads_number);
    server->port = shared_config->server_port;
    server->response_handler = dynamic_image_manipulation_response_handler;
    
    atexit(at_exit);
    
    start_server(server);
    
    return 0;
}
