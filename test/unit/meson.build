# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2020, MayaData Inc. All rights reserved.
# Copyright (c) 2020, DataCore Software Corporation. All rights reserved.

cflags = [
	'-DCHECK_UNIMPLEMENTED',
]

cppflags = [ ]

sources = [
	'main.c',
	'error_ut.c',
	'dirent_ut.c',
	'fatal_ut.c',
	'include_c_ut.c',
	'include_cpp_ut.cpp',
	'netdb_ut.c',
	'stat_ut.c',
	'stdio_ut.c',
	'string_ut.c',
	'strings_ut.c',
	'uio_ut.c',
	'uuid_ut.c',
	'unistd_ut.c',
]

subdir('include_tests')

# Disable g++ warnings for #include_next
if cpp.get_id() == 'gcc' and cpp.has_argument('-Wno-pedantic')
  cppflags += [ '-Wno-pedantic' ]
endif

unit_tests = executable('wpdk_unit',
		[ sources, include_sources, ],
		c_args: [ cflags, '-D_WPDK_UNIT_TESTS_' ],
		cpp_args: [ cflags, cppflags, '-D_WPDK_UNIT_TESTS_' ],
		include_directories: [ global_includes ],
		link_with : [ static_lib ],
		dependencies : cunit_dep,
		link_args : [ '-lws2_32', '-lrpcrt4' ],
		install: true
)
