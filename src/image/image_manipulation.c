//
//  image_manipulation.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "image_manipulation.h"

#define MAX(a, b) (a >= b ? a : b)
#define MIN(a, b) (a < b ? a : b)

void ThrowWandException(MagickWand *wand)
{
    char *description;
    ExceptionType severity;
    
    description = MagickGetException(wand, &severity);
    fprintf(stderr,"%s %s %lu %s\n", GetMagickModule(), description);
    description = (char *)MagickRelinquishMemory(description);
}

int save_image_with_image_info(image_info_t *image_info, const char *output)
{
    MagickWand *m_wand = NewMagickWand();
    MagickResetIterator(m_wand);
    MagickReadImage(m_wand, string_copy(image_info->base_path));
    
    float quality = image_info->quality;
    
    uint64_t real_width;
    if ((real_width = MagickGetImageWidth(m_wand)) == MagickFalse)
        ThrowWandException(m_wand);
    
    uint64_t real_height;
    if ((real_height = MagickGetImageHeight(m_wand)) == MagickFalse)
        ThrowWandException(m_wand);
    
    uint64_t real_dimensions[] = {real_width, real_height};
    uint64_t bound_dimensions[] = {image_info->max_width, image_info->max_height};
    uint64_t final_dimensions[2];
    
    image_dimensions_with_bounds(real_dimensions, bound_dimensions, final_dimensions);
    uint64_t width = final_dimensions[0];
    uint64_t height = final_dimensions[1];
    
    int colors = image_info->colors;
    
    if (quality != 0 && MagickSetImageCompressionQuality(m_wand, quality * 100) == MagickFalse)
        ThrowWandException(m_wand);
    
    if (width != 0 && height != 0 && MagickResizeImage(m_wand, width, height, LanczosFilter) == MagickFalse)
        ThrowWandException(m_wand);
    
    if (colors != 0 && MagickQuantizeImage(m_wand, colors, RGBColorspace, 0, 4, 1) == MagickFalse)
        ThrowWandException(m_wand);
    
    if (MagickWriteImage(m_wand, output) == MagickFalse)
        ThrowWandException(m_wand);
    
    MagickWandTerminus();
    
    return 0;
}

void image_dimensions_with_bounds(uint64_t real_dimensions[2], uint64_t bound_dimensions[2], uint64_t out_dimensions[2])
{
    uint64_t real_width = real_dimensions[0];
    uint64_t real_height = real_dimensions[1];
    float real_ratio = (float)real_width / (float)real_height;
    
    uint64_t bound_width = bound_dimensions[0];
    uint64_t bound_height = bound_dimensions[1];
    
    uint64_t width, height;
    if ((height = (width = MIN(real_width, bound_width)) / real_ratio) < bound_height) {}
    else if ((width = (height = MIN(real_height, bound_height)) * real_ratio) < bound_width) {}
    
    out_dimensions[0] = width;
    out_dimensions[1] = height;
}
