# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2020, MayaData Inc. All rights reserved.
# Copyright (c) 2020, DataCore Software Corporation. All rights reserved.

pkg = import('pkgconfig')

pkg.generate(static_lib, name: 'WPDK', filebase: 'libwpdk',
	version: meson.project_version(),
	description: 'Windows Platform Development Kit (WPDK)',
	subdirs: 'wpdk',
)
