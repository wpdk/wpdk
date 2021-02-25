# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) 2017-2019 Intel Corporation

# check the OS is supported, rather than going any further
if host_machine.system() != 'windows'
	error('The Windows Platform Development Kit is only supported on Windows')
endif

# MS linker requires special treatment.
is_ms_linker = (cc.get_id() == 'clang')

# set the machine type and cflags for it
if meson.is_cross_build()
	machine = host_machine.cpu()
else
	machine = get_option('machine')
endif

# machine type 'default' is special, it defaults to the per arch agreed common
# minimal baseline needed for DPDK.
# That might not be the most optimized, but the most portable version while
# still being able to support the CPU features required for DPDK.
# This can be bumped up by the DPDK project, but it can never be an
# invariant like 'native'
if machine == 'default'
	if host_machine.cpu_family().startswith('x86')
		machine = 'corei7'
	elif host_machine.cpu_family().startswith('aarch')
		machine = 'default'
	endif
endif

if cc.get_id() != 'msvc'
	machine_args = '-march=' + machine
endif

if get_option('b_lto')
	if cc.has_argument('-ffat-lto-objects')
		add_project_arguments('-ffat-lto-objects', language: 'c')
	else
		error('compiler does not support fat LTO objects - please turn LTO off')
	endif
	# workaround for gcc bug 81440
	if cc.get_id() == 'gcc' and cc.version().version_compare('<8.0')
		add_project_arguments('-Wno-lto-type-mismatch', language: 'c')
		add_project_link_arguments('-Wno-lto-type-mismatch', language: 'c')
	endif
endif

# workaround for gcc bug 86832
if cc.get_id() == 'gcc' and cc.version().version_compare('<8.3')
	add_project_arguments('-mstack-protector-guard=global', language: 'c')
endif
if cpp.get_id() == 'gcc' and cpp.version().version_compare('<8.3')
	add_project_arguments('-mstack-protector-guard=global', language: 'cpp')
endif
