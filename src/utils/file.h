//
//  file.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 08/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef file_h
#define file_h

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dirent.h>

#include "error.h"
#include "string.h"

/*
	Maps a file in memory
	@param file_path: the path of the file to be mapped
	@param file_size: pointer to the size of the file
	@return: pointer to the memory area
 */
const char *map_file(const char *file_path, uint64_t *file_size);

/*
	Verifies if the path actually exists
	@param path: path of the file
	@param is_directory:
	@return: boolean
 */
int path_exists(const char *path, int *is_directory);

/*
	Returns the size of the file at a certain path
	@param file_path: the path of the file
	@return: the size of the file
 */
off_t filesize(const char *file_path);

/*
	Clears the folder at a certain path
	@param dir_path: the path of the directory to be cleared
 */
void clear_folder(const char *dir_path);

#endif /* file_h */
