//
//  image_manipulation.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef image_manipulation_h
#define image_manipulation_h

#include <stdio.h>
#include <MagickWand/MagickWand.h>

#include "image_cache.h"

#include "../utils/logger.h"
#include "../utils/string.h"

/*
	Manipulates the image with magickwand functions and writes it into output path
	@param image_info: pointer to the image_info struct
	@param output: path where to write the output file
 */
int save_image_with_image_info(image_info_t *image_info, const char *output);

/*
	Finds bounds for the image (proportions, etc.)
	@param real_dimensions: vector with the dimensions of the real image
	@param bound_dimensions: vector with the dimensions bounds read from WURFL
	@param out_dimensions: vector with the dimensions found for the output
 */
void image_dimensions_with_bounds(uint64_t real_dimensions[2], uint64_t bound_dimensions[2], uint64_t out_dimensions[2]);

#endif /* image_manipulation_h */
