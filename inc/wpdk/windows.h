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

#ifndef _WPDK_WINDOWS_H_
#define _WPDK_WINDOWS_H_

#include <wpdk/header.h>

/*
 * Include the smallest set of Windows definitions possible to
 * minimise the potential for conflicts.
 */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
// enable for setupapi.h
// #define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
// enable for setupapi.h
// #define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
// enable for setupapi.h
// #define NOUSER
#define NONLS
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#if defined(__MINGW32__) && !defined(_WPDK_BUILD_LIB_)
#define NOMSG
#endif
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

/*
 * Avoid a collision between the clang headers and the Platform SDK
 */
#ifdef __clang__
#define _m_prefetchw __m_prefetchw
#endif

#include <windows.h>
#undef _m_prefetchw

#endif /* _WPDK_WINDOWS_H_ */
