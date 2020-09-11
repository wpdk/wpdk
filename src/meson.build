# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2020, MayaData Inc. All rights reserved.
# Copyright (c) 2020, DataCore Software Corporation. All rights reserved.

libraries = ['crypto', 'numa', 'pthread', 'rt', 'uuid']

headers = []
cflags = []

sources = [
	'epoll.c',
	'fcntl.c',
	'file.c',
	'getopt.c',
	'ifaddrs.c',
	'ioctl.c',
	'libaio.c',
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

install_headers(headers)

# build static lib
static_lib = static_library('wpdk',
		sources,
		c_args: cflags,
		include_directories: [ global_includes ],
		install: true)

# build libraries
foreach name:libraries
	sources = []
	headers = []
	cflags = []

	subdir(name)
	install_headers(headers)

	if sources.length() > 0
		# build static lib
		static_lib = static_library(name,
				sources,
				c_args: cflags,
				include_directories: [ global_includes ],
				install: true)
	endif
endforeach