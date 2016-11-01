//
//  file.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 08/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "file.h"

const char *map_file(const char *file_path, uint64_t *file_size)
{
    const char *memblock;
    int fd;
    struct stat sb;
    
    fd = open(file_path, O_RDONLY);
    fstat(fd, &sb);
    *file_size = (uint64_t)sb.st_size;
    
    memblock = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    exit_on_error(memblock == MAP_FAILED, "mmap");
    
    close(fd);
    
    return memblock;
}

int path_exists(const char *path, int *is_directory)
{
    struct stat path_stat;
    int rc = stat(path, &path_stat);
    if (is_directory != NULL) *is_directory = S_ISDIR(path_stat.st_mode);
    return rc != -1;
}

off_t filesize(const char *file_path) {
    struct stat st;
    
    if (stat(file_path, &st) == 0)
        return st.st_size;
    
    return -1;
}

void clear_folder(const char *dir_path)
{
    DIR *theFolder = opendir(dir_path);
    struct dirent *next_file;
    char *filepath;
    
    while ( (next_file = readdir(theFolder)) != NULL )
    {
        filepath = string_by_adding_path_component(dir_path, next_file->d_name);
        remove(filepath);
    }
    closedir(theFolder);
}
