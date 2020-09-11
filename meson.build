# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2020, MayaData Inc. All rights reserved.
# Copyright (c) 2020, DataCore Software Corporation. All rights reserved.

project('WPDK', 'C',
	version: '0.1.0',
	license: 'BSD',
	default_options: [
		'buildtype=release',
		'default_library=static',
		'b_vscrt=none',
		'warning_level=3'
		],
	meson_version: '>= 0.47.1'
)

cc = meson.get_compiler('c')
global_includes = include_directories('.', 'include')

subdir('include')
subdir('src')

meson.add_install_script('scripts/copy_libs.bat')
