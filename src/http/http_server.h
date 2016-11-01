//
//  http_server.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/04/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef http_server_h
#define http_server_h

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "http_request.h"
#include "http_response.h"
#include "http_header.h"
#include "http_custom_response.h"

#include "../utils/logger.h"
#include "../utils/threadpool.h"

#define LISTENQ 1024

typedef struct http_server_t {
    uint32_t port;
    http_response_t *(*response_handler)(struct http_server_t *http_server, http_request_t *request);
    int running;
    threadpool_t *threadpool;
    pthread_mutex_t response_handler_accesses_lock;
} http_server_t;

/*
	Initializes the server and allocates the struct, assigning attributes
	@param threads_number: max number of threads for threadpool
	@return: pointer to the server struct
 */
http_server_t *http_server_init(uint32_t threads_number);

/*
	Starts the server by opening the socket and starting the accepting loop
	@param server: pointer to the server struct
 */
void start_server(http_server_t *server);

/*
	Stops the server by assigning 0 at the running attribute (last accept)
	@param server: pointer to the server struct
 
 */
void stop_server(http_server_t *server);

/*
	Deallocates the server and frees memory
	@param http_server: pointer to the server struct
 */
void http_server_destroy(http_server_t *http_server);

#endif /* http_server_h */


