//
//  error.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/04/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef error_h
#define error_h

#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

/*
	Exits with a message
	@param message: char pointer for the message wanted to be written
 */
void do_exit(char *message);

/*
	Exits if a condition in verified
	@param condition: the condition to be analyzed
	@param message: the message to be written
 */
void exit_on_error(int condition, char *message);

/*
	Writes in the log if an error occurs
	@param condition: the condition to be analyzed
	@param message: the message to be written
 */
void log_on_error(int condition, char *message);

#endif /* error_h */
