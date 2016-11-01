//
//  threadpool.c
//  AdaptiveServer
//
//  Created by Andrea Silvi on 04/09/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#include "threadpool.h"

static void *threadpool_thread(void *argument);

threadpool_t *threadpool_init(uint32_t max_size)
{
    threadpool_t *threadpool = malloc(sizeof(threadpool_t));
    exit_on_error(threadpool == NULL, "malloc: threadpool_t");
    
    threadpool->max_size = max_size;//max_size;
    threadpool->running_threads = 0;
    threadpool->queued_threads_list = singlylinkedlist_init();
    PTHREAD_MUTEX_INIT(threadpool->mt_safe_lock);
    return threadpool;
}

threadpoolitem_t *threadpoolitem_init(void (*function)(void *), void *argument, pthread_attr_t *flags)
{
    threadpoolitem_t *threadpoolitem = malloc(sizeof(threadpoolitem_t));
    exit_on_error(threadpoolitem == NULL, "malloc: threadpoolitem_t");
    
    threadpoolitem->function = function;
    threadpoolitem->argument = argument;
    threadpoolitem->flags = flags;
    return threadpoolitem;
}

threadpoolthread_t *threadpoolthread_init(threadpool_t *threadpool, threadpoolitem_t *threadpoolitem)
{
    threadpoolthread_t *threadpoolthread = malloc(sizeof(threadpoolthread_t));
    exit_on_error(threadpoolthread == NULL, "malloc: threadpoolthread_t");
    
    threadpoolthread->threadpool = threadpool;
    threadpoolthread->threadpoolitem = threadpoolitem;
    return threadpoolthread;
}

void threadpool_add(threadpool_t *threadpool, void (*function)(void *), void *argument, pthread_attr_t *flags)
{
    PTHREAD_MUTEX_LOCK(threadpool->mt_safe_lock)
    threadpoolitem_t *threadpoolitem = threadpoolitem_init(function, argument, flags);
    
    if (threadpool->max_size < -1) return;
    else if (threadpool->max_size == -1)
    {
        threadpool->running_threads++;
        threadpoolthread_t threadpoolthread = {threadpool, threadpoolitem};
        threadpool_thread(&threadpoolthread);
    }
    else if (threadpool->max_size == 0 || threadpool->running_threads < threadpool->max_size) {
        threadpool->running_threads++;
        threadpoolthread_t *threadpoolthread = threadpoolthread_init(threadpool, threadpoolitem);
        pthread_create(&threadpoolitem->pthread, flags, threadpool_thread, threadpoolthread);
    }
    else
    {
        PTHREAD_RWLOCK_WRLOCK(threadpool->queued_threads_list->mt_safe_rwlock)
        singlylinkedlist_add(threadpool->queued_threads_list, threadpoolitem);
        PTHREAD_RWLOCK_UNLOCK(threadpool->queued_threads_list->mt_safe_rwlock)
    }
    PTHREAD_MUTEX_UNLOCK(threadpool->mt_safe_lock)
}

void threadpool_dequeue(threadpool_t *threadpool)
{
    if (threadpool->queued_threads_list->length > 0) {
        PTHREAD_RWLOCK_WRLOCK(threadpool->queued_threads_list->mt_safe_rwlock)
        singlylinkedlistitem_t *singlylinkedlistitem = singlylinkedlist_unlink(threadpool->queued_threads_list, 0);
        threadpoolitem_t *threadpoolitem = (threadpoolitem_t *)singlylinkedlistitem->item;
        PTHREAD_RWLOCK_UNLOCK(threadpool->queued_threads_list->mt_safe_rwlock)
        threadpool_add(threadpool, threadpoolitem->function, threadpoolitem->argument, threadpoolitem->flags);
        singlylinkedlistitem_destroy(singlylinkedlistitem);
    }
}

static void *threadpool_thread(void *argument)
{
    threadpoolthread_t *threadpoolthread = argument;
    threadpool_t *threadpool = threadpoolthread->threadpool;
    threadpoolitem_t *threadpoolitem = threadpoolthread->threadpoolitem;
    
    threadpoolitem->function(threadpoolitem->argument);
    if (threadpool->max_size > -1) PTHREAD_MUTEX_LOCK(threadpool->mt_safe_lock)
        threadpool->running_threads--;
    if (threadpool->max_size > -1) PTHREAD_MUTEX_UNLOCK(threadpool->mt_safe_lock)
        
        threadpool_dequeue(threadpool);
    
    return NULL;
    
}
