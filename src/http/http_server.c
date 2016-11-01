//
//  http_server.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 21/04/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "http_server.h"

typedef struct
{
    http_server_t *server;
    int socket;
} handle_connection_thread_args;

http_server_t *http_server_init(uint32_t threads_number)
{
    http_server_t *http_server = malloc(sizeof(http_server_t));
    exit_on_error(http_server == NULL, "malloc: http_server_t");
    
    http_server->port = 0;
    http_server->response_handler = NULL;
    http_server->running = 0;
    http_server->threadpool = threadpool_init(threads_number);
    PTHREAD_MUTEX_INIT(http_server->response_handler_accesses_lock);
    return http_server;
}

void handle_connection(http_server_t *server, int socket)
{
    char *buffer;
    ssize_t len;
    http_request_t *http_request = NULL;
    
    for (int i = 0; (len = fd_getline(socket, &buffer)) > 0; i++) {
        
        if (buffer[0] == '\r') //End of headers
            break;
        if (i == 0) {
            http_request = http_request_init_with_request_line(buffer);
        }
        else
        {
            char *parsed_http_header_key;
            char *parsed_http_header_value;
            parse_header_line(buffer, &parsed_http_header_key, &parsed_http_header_value);
            
            http_headers_add(http_request->headers, parsed_http_header_key, parsed_http_header_value);
            
            free(parsed_http_header_key);
            free(parsed_http_header_value);
        }
        
        free(buffer);
    }
    if (http_request == NULL) return;
    
    do_log_with_format(1, "Did receive request: \"%s %s\"", http_request_method_string(http_request), http_request->path);
    
    http_response_t *http_response = http_request_is_valid(http_request) ? server->response_handler(server, http_request) : http_response_bad_request();
    
    http_response->http_version = http_request->http_version;
    if (http_request->method == HEAD) http_response->body = http_response->path = NULL;
    http_response_write(http_response, socket);
    
    do_log_with_format(1, "Did send response: \"%s %u bytes\"", http_response_status_string(http_response), http_response->body_lenght);
    
    http_request_destroy(http_request);
    http_response_destroy(http_response);
}

void handle_connection_thread_job(void *args)
{
    handle_connection_thread_args *thread_args = (handle_connection_thread_args *)args;
    
    handle_connection(thread_args->server, thread_args->socket);
    
    if (close(thread_args->socket) < 0)
        do_exit("Error closing connection socket in parent.");
    
    do_log(0, "Did close connection on socket");
}


void start_server(http_server_t *server)
{
    int listener;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len;
    
    /*  Create socket  */
    
    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        do_exit("Couldn't create listening socket.");
    
    int on = 1;
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        do_exit("Couldn't make reusable socket.");
    
    if (setsockopt(listener, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on)) < 0)
        do_exit("Couldn't make keepalive socket.");
    
    /*  Populate socket address structure  */
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(server->port);
    
    /*  Assign socket address to socket  */
    
    if (bind(listener, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        do_exit("Couldn't bind listening socket.");
    
    /*  Make socket a listening socket  */
    
    if (listen(listener, LISTENQ) < 0)
        do_exit("Call to listen failed.");
    
    cli_len = sizeof(cli_addr);
    
    server->running = 1;
    
    char serv_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &serv_addr.sin_addr, serv_ip, INET_ADDRSTRLEN);
    
    do_log_with_format(1, "Started on port %d and reachable at http://%s:%d", server->port, serv_ip, server->port);
    
    /*  Loop infinitely to accept and service connections  */
    
    while (server->running) {
        /*  Wait for connection  */
        int conn;
        
        if ( (conn = accept(listener, (struct sockaddr *) &cli_addr, &cli_len)) < 0 )
            do_exit("Error calling accept().");
        
        if (conn > 0){
            char cli_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &cli_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
            do_log_with_format(1, "Did open connection on socket from IP: %s", cli_ip);
        }
        
        handle_connection_thread_args *args = malloc(sizeof(handle_connection_thread_args));
        args->server = server;
        args->socket = conn;
        
        threadpool_add(server->threadpool, handle_connection_thread_job, args, NULL);
    }
}

void stop_server(http_server_t *server)
{
    /* This tells the server to accept the last request and then close it */
    server->running = 0;
}

void http_server_destroy(http_server_t *http_server)
{
    PTHREAD_MUTEX_DESTROY(http_server->response_handler_accesses_lock);
    free(http_server);
}
