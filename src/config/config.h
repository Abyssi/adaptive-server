//
//  config.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 02/09/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef config_h
#define config_h

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../utils/error.h"
#include "../utils/string.h"

#ifdef DEBUG
#define kSERVER_DIRECTORY "/Users/andreasilvi/Documents/Developing/adaptiveserver/AdaptiveServer/www/html/"
#define kCACHE_DIRECTORY "/Users/andreasilvi/Documents/Developing/adaptiveserver/AdaptiveServer/cache/"
#define kCACHE_PATH "/Users/andreasilvi/Documents/Developing/adaptiveserver/AdaptiveServer/cache/adaptiveserver.cache"
#define kMIME_PATH "/Users/andreasilvi/Documents/Developing/adaptiveserver/AdaptiveServer/etc/mime.types"
#define kWURFL_PATH "/Users/andreasilvi/Documents/Developing/adaptiveserver/AdaptiveServer/etc/wurfl.xml"
#define kLOG_PATH "/Users/andreasilvi/Documents/Developing/adaptiveserver/AdaptiveServer/log/log.txt"
#define kCONFIG_PATH "/Users/andreasilvi/Documents/Developing/adaptiveserver/AdaptiveServer/etc/adaptiveserver.conf"
#else
#define kCONFIG_PATH "../etc/adaptiveserver.conf"
#endif

typedef struct {
    uint32_t cache_size;
    uint32_t log_level;
    uint32_t server_port;
    uint32_t threads_number;
    float unflushed_cache_percentage;
    const char *server_directory;
    const char *cache_directory;
    const char *cache_path;
    const char *mime_path;
    const char *wurfl_path;
    const char *log_path;
} config_t;

extern config_t *shared_config;

/*
 Initializes the configuration file
 */
config_t *config_init(int argc, char *argv[]);

/*
 Parses the argumets written in argv
 */
int config_parse_arguments(config_t *config, int argc, char *argv[]);

/*
 Reads the config file
 */
void config_read_file(config_t *config);

#endif /* config_h */
