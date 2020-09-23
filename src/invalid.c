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

#include <wpdk/internal.h>
#include <stdlib.h>
#include <crtdbg.h>


long _wpdk_invalid_handler_done;

static INIT_ONCE wpdk_invalid_once = INIT_ONCE_STATIC_INIT;


static void
wpdk_invalid_parameter_handler(const wchar_t* expr, const wchar_t* func,
	const wchar_t* file, unsigned int line, uintptr_t pReserved)
{
	UNREFERENCED_PARAMETER(expr);
	UNREFERENCED_PARAMETER(func);
	UNREFERENCED_PARAMETER(file);
	UNREFERENCED_PARAMETER(line);
	UNREFERENCED_PARAMETER(pReserved);
	return;
}


static BOOL CALLBACK
wpdk_invalid_callback(INIT_ONCE *pInit, void *arg, void **pContext)
{
	UNREFERENCED_PARAMETER(pInit);
	UNREFERENCED_PARAMETER(arg);

	/*
	 *  Report invalid parameter errors as EINVAL rather than by
	 *  terminating the program.
	 */
	_set_invalid_parameter_handler(wpdk_invalid_parameter_handler);
	_CrtSetReportMode(_CRT_ASSERT, 0);

	InterlockedExchange(&_wpdk_invalid_handler_done, 1);
	
	*pContext = NULL;
	return TRUE;
}


void
_wpdk_set_invalid_handler()
{
	void *context = NULL;

	/*
	 *  The call should never fail since the callback doesn't return
	 *  FALSE. If it does fail, the best option is to let execution
	 *  continue with the default invalid parameter handler action of
	 *  terminating the program.
	 * 
	 *  NOTE: The documentation appears to incorrectly state the order
	 *  of the arguments as (..., Context, Parameter).  According to
	 *  the function prototype and the example code, the correct order
	 *  is (..., Parameter, Context).
	 */
	InitOnceExecuteOnce(&wpdk_invalid_once, wpdk_invalid_callback,
			NULL, &context);
}
