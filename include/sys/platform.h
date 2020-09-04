#ifndef _WPDK_SYS_PLATFORM_H_
#define	_WPDK_SYS_PLATFORM_H_

/*
 * Include the smallest set of Windows definitions possible to
 * minimise the potential for conflicts.
 */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOWINMESSAGES
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
#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
#define NOUSER
#define NONLS
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
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
 * Disable Winsock warnings
 */
//#define _WINSOCK_DEPRECATED_NO_WARNINGS

/*
 * Avoid a collision between the clang headers and the Platform SDK
 */
#ifdef __clang__
#define _m_prefetchw __m_prefetchw
#endif

#include <windows.h>
#undef _m_prefetchw

#include <sys/cdefs.h>
#include <sys/_types.h>

#endif /* !_WPDK_SYS_PLATFORM_H_ */