//
//  pthread.h
//  AdaptiveServer
//
//  Created by Andrea Silvi on 07/09/16.
//  Copyright © 2016 Andrea Silvi. All rights reserved.
//

#ifndef pthread_h
#define pthread_h

#include <pthread.h>

#include "error.h"

/*
	These functions are defined to be made different from the others
	These are synchronization functions
 */
#define PTHREAD_MUTEX_INIT(mutex) exit_on_error(pthread_mutex_init(&mutex, NULL) != 0, "pthread_mutex_init")
#define PTHREAD_MUTEX_DESTROY(mutex) exit_on_error(pthread_mutex_destroy(&mutex) != 0, "pthread_mutex_init");
#define PTHREAD_MUTEX_LOCK(mutex) exit_on_error(pthread_mutex_lock(&mutex) != 0, "pthread_mutex_init");
#define PTHREAD_MUTEX_UNLOCK(mutex) exit_on_error(pthread_mutex_unlock(&mutex) != 0, "pthread_mutex_init");

#define PTHREAD_RWLOCK_INIT(rwlock) exit_on_error(pthread_rwlock_init(&rwlock, NULL) != 0, "pthread_rwlock_init")
#define PTHREAD_RWLOCK_DESTROY(rwlock) exit_on_error(pthread_rwlock_destroy(&rwlock) != 0, "pthread_mutex_init");
#define PTHREAD_RWLOCK_RDLOCK(rwlock) exit_on_error(pthread_rwlock_rdlock(&rwlock) != 0, "pthread_mutex_init");
#define PTHREAD_RWLOCK_WRLOCK(rwlock) exit_on_error(pthread_rwlock_wrlock(&rwlock) != 0, "pthread_mutex_init");
#define PTHREAD_RWLOCK_UNLOCK(rwlock) exit_on_error(pthread_rwlock_unlock(&rwlock) != 0, "pthread_mutex_init");

#endif /* pthread_h */
