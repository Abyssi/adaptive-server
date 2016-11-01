//
//  threadpool.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 04/09/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef threadpool_h
#define threadpool_h

#include <stdio.h>
#include "pthread.h"

#include "singlylinkedlist.h"

typedef struct {
    int32_t max_size; // -1 use only main threads, 0 use infinite thread, other limit threads
    uint32_t running_threads;
    singlylinkedlist_t *queued_threads_list;
    
    pthread_mutex_t mt_safe_lock;
} threadpool_t;

typedef struct {
    pthread_t pthread;
    
    void (*function)(void *);
    void *argument;
    pthread_attr_t *flags;
} threadpoolitem_t;

typedef struct {
    threadpool_t *threadpool;
    threadpoolitem_t *threadpoolitem;
} threadpoolthread_t;

/*
 Initializes thread pool structure
 @return: pointer to the struct threadpool_t
 */
threadpool_t *threadpool_init(uint32_t max_size);

/*
 Initializes an item inside the thread_pool structure
 @param function:
 @param argument:
 @param flags:
 @return: pointer to the struct threadpoolitem_t
 */
threadpoolitem_t *threadpoolitem_init(void (*function)(void *), void *argument, pthread_attr_t *flags);

/*
 Initializes a thread inside the thread_pool structure
 @param threadpool:
 @param threadpoolitem:
 @return: pointer to the struct threadpoolthread_t
 */
threadpoolthread_t *threadpoolthread_init(threadpool_t *threadpool, threadpoolitem_t *threadpoolitem);

/*
 Adds a thread in the pool thread
 @param threadpool: pointer to the pool struct
 @param function:
 @param argument: argument for the function
 @param flags: use this for more options
 */
void threadpool_add(threadpool_t *threadpool, void (*function)(void *), void *argument, pthread_attr_t *flags);

/*
 Deletes an from the queue
 @return: pointer to the struct threadpool_t
 */
void threadpool_dequeue(threadpool_t *threadpool);

#endif /* threadpool_h */
