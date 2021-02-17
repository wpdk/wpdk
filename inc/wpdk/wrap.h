/*-
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Copyright (c) 2020, MayaData Inc. All rights reserved.
 *  Copyright (c) 2020, DataCore Software Corporation. All rights reserved.
 * 
 *  POSIX details are based on the Open Group Base Specification Issue 7,
 *  2018 edition at https://pubs.opengroup.org/onlinepubs/9699919799/
 * 
 *  Details about Linux extensions are based on the Linux man-pages project
 *  at https://www.kernel.org/doc/man-pages/
 */

#ifndef _WPDK_WRAP_H_
#define _WPDK_WRAP_H_

/*
 *  The MSVC linker doesn't support the GCC --wrap option used to
 *  implement mocking. Simulate it using /alternatename.
 */
#if defined(_MSC_VER)

#define wpdk_calloc __wrap_wpdk_calloc
#pragma comment(linker, "/alternatename:__wrap_wpdk_calloc=wpdk_calloc")
#pragma comment(linker, "/alternatename:__real_wpdk_calloc=wpdk_calloc")

#define wpdk_pthread_mutexattr_init __wrap_wpdk_pthread_mutexattr_init
#pragma comment(linker, "/alternatename:__wrap_wpdk_pthread_mutexattr_init=wpdk_pthread_mutexattr_init")
#pragma comment(linker, "/alternatename:__real_wpdk_pthread_mutexattr_init=wpdk_pthread_mutexattr_init")

#define wpdk_pthread_mutex_init __wrap_wpdk_pthread_mutex_init
#pragma comment(linker, "/alternatename:__wrap_wpdk_pthread_mutex_init=wpdk_pthread_mutex_init")
#pragma comment(linker, "/alternatename:__real_wpdk_pthread_mutex_init=wpdk_pthread_mutex_init")

#define wpdk_recvmsg __wrap_wpdk_recvmsg
#pragma comment(linker, "/alternatename:__wrap_wpdk_recvmsg=wpdk_recvmsg")
#pragma comment(linker, "/alternatename:__real_wpdk_recvmsg=wpdk_recvmsg")

#define wpdk_sendmsg __wrap_wpdk_sendmsg
#pragma comment(linker, "/alternatename:__wrap_wpdk_sendmsg=wpdk_sendmsg")
#pragma comment(linker, "/alternatename:__real_wpdk_sendmsg=wpdk_sendmsg")

#define wpdk_unlink __wrap_wpdk_unlink
#pragma comment(linker, "/alternatename:__wrap_wpdk_unlink=wpdk_unlink")
#pragma comment(linker, "/alternatename:__real_wpdk_unlink=wpdk_unlink")

#define wpdk_writev __wrap_wpdk_writev
#pragma comment(linker, "/alternatename:__wrap_wpdk_writev=wpdk_writev")
#pragma comment(linker, "/alternatename:__real_wpdk_writev=wpdk_writev")

#endif /* _MSC_VER */

/*
 *  Redefine references to __wrap_ and __real_ functions to include
 *  wpdk in the function name.
 */
#define __wrap_calloc	__wrap_wpdk_calloc
#define __real_calloc	__real_wpdk_calloc

#define __wrap_pthread_mutexattr_init	__wrap_wpdk_pthread_mutexattr_init
#define __real_pthread_mutexattr_init	__real_wpdk_pthread_mutexattr_init

#define __wrap_pthread_mutex_init		__wrap_wpdk_pthread_mutex_init
#define __real_pthread_mutex_init		__real_wpdk_pthread_mutex_init

#define __wrap_recvmsg	__wrap_wpdk_recvmsg
#define __real_recvmsg	__real_wpdk_recvmsg

#define __wrap_sendmsg	__wrap_wpdk_sendmsg
#define __real_sendmsg	__real_wpdk_sendmsg

#define __wrap_unlink	__wrap_wpdk_unlink
#define __real_unlink	__real_wpdk_unlink

#define __wrap_writev	__wrap_wpdk_writev
#define __real_writev	__real_wpdk_writev

#endif /* _WPDK_WRAP_H_ */
