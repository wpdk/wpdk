# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2020, MayaData Inc. All rights reserved.
# Copyright (c) 2020, DataCore Software Corporation. All rights reserved.

# To add support for --wrap on all functions, generate wrap.h
# from version.map. For now, use wrap.map which only contains
# functions that are wrapped by SPDK.

wrap_map = files('../../src/wrap.map')

wrap_header = custom_target('wrap.h',
	command: [map_to_wrap_cmd, '@INPUT@', '@OUTPUT@'],
	input: wrap_map, output: 'wrap.h',
	install: true,
	install_dir: join_paths(get_option('includedir'), include_prefix, 'wpdk')
	)
