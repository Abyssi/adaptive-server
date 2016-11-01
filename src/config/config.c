//
//  config.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 02/09/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "config.h"

config_t *shared_config;

config_t *config_init(int argc, char *argv[])
{
    config_t *config = malloc(sizeof(config_t));
    exit_on_error(config == NULL, "malloc: config_t");
    config_read_file(config);
    config_parse_arguments(config, argc, argv);
    return config;
}

#ifdef DEBUG
static inline void config_setup_debug_paths(config_t *config)
{
    config->server_directory = kSERVER_DIRECTORY;
    config->cache_directory = kCACHE_DIRECTORY;
    config->cache_path = kCACHE_PATH;
    config->cache_path = kCACHE_PATH;
    config->mime_path = kMIME_PATH;
    config->wurfl_path = kWURFL_PATH;
    config->log_path = kLOG_PATH;
}
#endif

int config_parse_arguments(config_t *config, int argc, char *argv[])
{
    int c;
    while ((c = getopt(argc, argv, "c:l:p:t:")) != -1)
        switch (c)
    {
        case 'c':
            config->cache_size = atoi(optarg);
            break;
        case 'l':
            config->log_level = atoi(optarg);
            break;
        case 'p':
            config->server_port = atoi(optarg);
            break;
        case 't':
            config->threads_number = atoi(optarg);
            break;
        case '?':
            fprintf(stderr, "Unrecognized option: -%c\n", optopt);
            return -1;
    }
    return 0;
}

void config_read_file(config_t *config)
{
    FILE *fp;
    
    char config_line[1024];
    char config_name[512];
    char config_value[512];
    
    exit_on_error((fp = fopen(kCONFIG_PATH, "r")) == NULL, "fopen");
    
    while(!feof(fp))
    {
        fgets(config_line, 1024, fp);
        
        if (config_line[0] == '#' || config_line[0] == '\n') continue; //Skip lines staring with these chars
        
        sscanf(config_line, "%s %s\n", config_name, config_value);
        
        if (streq(config_name, "CACHE_SIZE"))
            config->cache_size = atoi(config_value);
        else if (streq(config_name, "LOG_LEVEL"))
            config->log_level = atoi(config_value);
        else if (streq(config_name, "SERVER_PORT"))
            config->server_port = atoi(config_value);
        else if (streq(config_name, "THREADS_NUMBER"))
            config->threads_number = atoi(config_value);
        else if (streq(config_name, "UNFLUSHED_CACHE_PERCENTAGE"))
            config->unflushed_cache_percentage = atof(config_value);
        else if (streq(config_name, "SERVER_DIRECTORY"))
            config->server_directory = string_copy(config_value);
        else if (streq(config_name, "CACHE_DIRECTORY"))
            config->cache_directory = string_copy(config_value);
        else if (streq(config_name, "CACHE_PATH"))
            config->cache_path = string_copy(config_value);
        else if (streq(config_name, "MIME_PATH"))
            config->mime_path = string_copy(config_value);
        else if (streq(config_name, "WURFL_PATH"))
            config->wurfl_path = string_copy(config_value);
        else if (streq(config_name, "LOG_PATH"))
            config->log_path = string_copy(config_value);
    }
    
    fclose(fp);
    
#ifdef DEBUG
    config_setup_debug_paths(config);
#endif
}

