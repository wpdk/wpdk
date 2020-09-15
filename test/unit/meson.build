# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2020, MayaData Inc. All rights reserved.
# Copyright (c) 2020, DataCore Software Corporation. All rights reserved.

cflags = [
	'-DCHECK_UNIMPLEMENTED',
]

sources = [
	'main.c',
	'error_ut.c',
	'fatal_ut.c',
	'string_ut.c',
	'strings_ut.c',
]

unit_tests = executable('unit_tests',
		sources,
		c_args: [ cflags, '-D_WPDK_UNIT_TESTS_' ],
		include_directories: [ global_includes ],
		link_with : static_lib,
		dependencies : cunit_dep,
		install: true
)
