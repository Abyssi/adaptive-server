//
//  logger.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 27/08/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef logger_h
#define logger_h

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>

#include "string.h"
#include "pthread.h"
#include "../config/config.h"

/*
	Writes a log message with options to the Standard Output
	@param level: the level of detail of the log message
	@param write_to_file: integer to reveal if the message has to be written in the log file
	@param message: the message to be written
 */
void do_log_with_options(uint32_t level, int write_to_file, char *message);

/*
	Writes a log message to the Standard Output
	@param level: the level of detail of the log message
	@param message: the message to be written
 */
void do_log(uint32_t level, char *message);

/*
	Writes a log message with format and options to the Standard Output
	@param level: the level of detail of the log message
	@param write_to_file: integer to reveal if the message has to be written in the log file
	@param format: format wanted for the meassage
	@param message: the message to be written
 */
void do_log_with_format_with_options(uint32_t level, int write_to_file, const char* format, ...);

/*
	Writes a log message with format to the Standard Output
	@param level: the level of detail of the log message
	@param format: format wanted for the meassage
	@param message: the message to be written
 */
void do_log_with_format(uint32_t level, const char* format, ...);

/*
	Writes a log error message with options to the Standard Output
	@param level: the level of detail of the log message
	@param write_to_file: integer to reveal if the message has to be written in the log file
	@param message: the message to be written
 */
void do_error_log_with_options(uint32_t level, int write_to_file, char *message);

/*
	Writes a log error message with to the Standard Output
	@param level: the level of detail of the log message
	@param message: the message to be written
 */
void do_error_log(uint32_t level, char *message);

/*
	Writes a log error message with format and options to the Standard Output
	@param level: the level of detail of the log message
	@param write_to_file: integer to reveal if the message has to be written in the log file
	@param format: format wanted for the meassage
	@param message: the message to be written
 */
void do_error_log_with_format_with_options(uint32_t level, int write_to_file, const char* format, ...);

/*
	Writes a log error message with format to the Standard Output
	@param level: the level of detail of the log message
	@param format: format wanted for the meassage
	@param message: the message to be written
 */
void do_error_log_with_format(uint32_t level, const char* format, ...);

/*
 Writes the output into log file
 @param message: the message to be written in the log file
 */
void do_log_to_file(char *message);

/*
 Clears the log file
 */
void clear_log_file();

#endif /* logger_h */
