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
	meson_version: '>= 0.55.0'
)

cc = meson.get_compiler('c')
global_includes = include_directories('.', 'include')

cunit = subproject('CUnit')
cunit_dep = cunit.get_variable('cunit_dep')

subdir('include')
subdir('src')
subdir('test/unit')

wpdk_dep = declare_dependency(
  include_directories : global_includes,
  link_with : [ static_lib, uuid_static ]
)

meson.add_install_script('scripts/copy_libs.bat')
