# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2020, MayaData Inc. All rights reserved.
# Copyright (c) 2020, DataCore Software Corporation. All rights reserved.

cflags = []

sources = [
	'aio.c',
	'epoll.c',
	'error.c',
	'fatal.c',
	'fcntl.c',
	'file.c',
	'getopt.c',
	'ifaddrs.c',
	'inet.c',
	'ioctl.c',
	'md5.c',
	'mman.c',
	'netdb.c',
	'poll.c',
	'pthread.c',
	'resource.c',
	'select.c',
	'semaphore.c',
	'signal.c',
	'socket.c',
	'stat.c',
	'stdio.c',
	'stdlib.c',
	'string.c',
	'strings.c',
	'syslog.c',
	'termios.c',
	'time.c',
	'uio.c',
	'unistd.c',
	'uuid.c',
	'wait.c',
	'wrap.c',
]

# build static lib
static_lib = static_library('wpdk',
		sources,
		c_args: cflags,
		include_directories: [ global_includes ],
		install: true)

# build empty libraries
foreach name:[ 'aio', 'crypto', 'numa', 'pthread', 'rt', 'uuid' ]
	null_lib = static_library(name,
			['empty.c'],
			c_args: cflags,
			include_directories: [ global_includes ],
			install: true)
endforeach
