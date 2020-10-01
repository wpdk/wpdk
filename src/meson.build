# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2020, MayaData Inc. All rights reserved.
# Copyright (c) 2020, DataCore Software Corporation. All rights reserved.

cflags = [ machine_args ]

sources = [
	'aio.c',
	'dirent.c',
	'epoll.c',
	'error.c',
	'fatal.c',
	'fcntl.c',
	'file.c',
	'fnmatch.c',
	'getopt.c',
	'ifaddrs.c',
	'inet.c',
	'invalid.c',
	'ioctl.c',
	'log.c',
	'md5.c',
	'mman.c',
	'netdb.c',
	'poll.c',
	'pthread.c',
	'pwd.c',
	'regex.c',
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

if cc.has_header('getopt.h')
	cflags += ['-DHAVE_GETOPT_H', '-DHAVE_GETOPT', '-DHAVE_GETOPT_LONG']
endif

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

# use pre-build objects to build shared lib
sources = []
objs = static_lib.extract_all_objects(recursive: false)

version_map = files('src_version.map')

def_file = custom_target('src_def',
	command: [map_to_win_cmd, '@INPUT@', '@OUTPUT@'],
	input: version_map, output: 'src_exports.def')

mingw_map = custom_target('src_mingw',
	command: [map_to_win_cmd, '@INPUT@', '@OUTPUT@'],
	input: version_map,	output: 'src_mingw.map')

if cc.get_id() == 'msvc'
	def_arg = ['/def:' + def_file.full_path()]
elif is_ms_linker
	def_arg = ['-Wl,/def:' + def_file.full_path()]
else
	def_arg = ['-Wl,--version-script=' + mingw_map.full_path()]
endif

# build shared lib
shared_lib = shared_library('wpdk',
		sources,
		objects: objs,
		c_args: cflags,
		include_directories: [ global_includes ],
		link_args: [ def_arg, '-lws2_32', '-lrpcrt4' ],
		link_depends: [ version_map, def_file, mingw_map ],
		install: true)
