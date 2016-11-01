//
//  logger.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 27/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "logger.h"

static pthread_mutex_t log_file_op_mutex = PTHREAD_MUTEX_INITIALIZER;

void do_log_with_options(uint32_t level, int write_to_file, char *message)
{
    if (level > shared_config->log_level) return;
    
    char formatted_time_string[20];
    formatted_time(formatted_time_string, sizeof(formatted_time_string), "%Y-%m-%d %H:%M:%S");
    
    char final_log_string[1024];
    snprintf(final_log_string, sizeof(final_log_string), "[%s] [LOG] AdaptiveServer: %s\n", formatted_time_string, message);
    
    fprintf(stdout, "%s", final_log_string);
    
    if (write_to_file) do_log_to_file(final_log_string);
}

void do_log(uint32_t level, char *message)
{
    do_log_with_options(1, level, message);
}

void do_log_with_format_with_options(uint32_t level, int write_to_file, const char* format, ...)
{
    char buffer[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (buffer, sizeof(buffer), format, args);
    do_log_with_options(level, write_to_file, buffer);
    va_end (args);
}

void do_log_with_format(uint32_t level, const char* format, ...)
{
    char buffer[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (buffer, sizeof(buffer), format, args);
    do_log_with_options(level, 1, buffer);
    va_end (args);
}

void do_log_to_file(char *message) {
    PTHREAD_MUTEX_LOCK(log_file_op_mutex);
    static FILE *log_file = 0;
    
    if (log_file == 0) {
        log_file = fopen(shared_config->log_path, "at");
        if (!log_file) log_file = fopen(shared_config->log_path, "wt");
        if (!log_file) do_log_with_options(0, 0, "Opening log file error");
        if (log_file) fprintf(log_file, "\n\n"); //clear_log_file();
    }
    
    fprintf(log_file, "%s", message);
    
    fflush(log_file);
    PTHREAD_MUTEX_UNLOCK(log_file_op_mutex);
}

void do_error_log_with_options(uint32_t level, int write_to_file, char *message)
{
    if (level > shared_config->log_level) return;
    
    char formatted_time_string[20];
    formatted_time(formatted_time_string, sizeof(formatted_time_string), "%Y-%m-%d %H:%M:%S");
    
    char final_log_string[1024];
    snprintf(final_log_string, sizeof(final_log_string), "[%s] [ERROR] AdaptiveServer: %s\n", formatted_time_string, message);
    
    fprintf(stderr, "%s", final_log_string);
    
    if (write_to_file) do_log_to_file(final_log_string);
}

void do_error_log(uint32_t level, char *message)
{
    do_error_log_with_options(1, level, message);
}

void do_error_log_with_format_with_options(uint32_t level, int write_to_file, const char* format, ...)
{
    char buffer[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (buffer, sizeof(buffer), format, args);
    do_error_log_with_options(level, write_to_file, buffer);
    va_end (args);
}

void do_error_log_with_format(uint32_t level, const char* format, ...)
{
    char buffer[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (buffer, sizeof(buffer), format, args);
    do_error_log_with_options(level, 1, buffer);
    va_end (args);
}

void clear_log_file()
{
    if (truncate(shared_config->log_path, 0) == -1) do_log_with_options(0, 0, "Clearing log file error");
}
