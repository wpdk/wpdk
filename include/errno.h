/*-
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Copyright (c) 2020, MayaData Inc. All rights reserved.
 *  Copyright (c) 2020, DataCore Software Corporation. All rights reserved.
 *
 */

#ifndef _WPDK_ERRNO_H_
#define _WPDK_ERRNO_H_

#include <sys/platform.h>
#include <../ucrt/errno.h>

_WPDK_BEGIN_C_HEADER

#define ESHUTDOWN		200
#define ETOOMANYREFS	201
#define EHOSTDOWN		202
#define EUSERS			203
#define EDQUOT			204
#define ESTALE			205
#define EREMOTE			206
#define EPROCLIM		207

_WPDK_END_C_HEADER

#endif /* _WPDK_ERRNO_H_ */
