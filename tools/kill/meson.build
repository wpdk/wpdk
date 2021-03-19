# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2020, MayaData Inc. All rights reserved.
# Copyright (c) 2020, DataCore Software Corporation. All rights reserved.

cflags = []

sources = [
	'kill.c',
	wrap_header,
]

wpdk_terminate = executable('wpdk_terminate',
		sources,
		c_args: cflags,
		include_directories: [ global_includes ],
		link_with : [ static_lib ],
		link_args : [ link_libs, link_static_libs ],
		install: true
)

wpdk_kill = configure_file(input : 'kill.sh', output : 'wpdk_kill',
		copy: true, install: true, install_dir: 'bin',
		install_mode: 'rwxr-xr-x')
