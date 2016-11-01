//
//  error.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/04/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "error.h"

void exit_on_error(int condition, char *message)
{
    if (condition) do_exit(message);
}

void do_exit(char *message)
{
    do_error_log_with_format(0, message);
    exit(EXIT_FAILURE);
}

void log_on_error(int condition, char *message)
{
    if (condition) do_error_log_with_format(0, message);
}






