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
	'include_ut.c',
	'string_ut.c',
	'strings_ut.c',
]

include_sources = [
	'include/arpa_inet_h.c',
	'include/net_if_h.c',
	'include/netinet_in_h.c',
	'include/netinet_tcp_h.c',
	'include/openssl_md5_h.c',
	'include/sys_cdefs_h.c',
	'include/sys_epoll_h.c',
	'include/sys_file_h.c',
	'include/sys_ioctl_h.c',
	'include/sys_mman_h.c',
	'include/sys_queue_h.c',
	'include/sys_resource_h.c',
	'include/sys_select_h.c',
	'include/sys_socket_h.c',
	'include/sys_stat_h.c',
	'include/sys_time_h.c',
	'include/sys_uio_h.c',
	'include/sys_un_h.c',
	'include/sys_user_h.c',
	'include/sys_wait_h.c',
	'include/corecrt_h.c',
	'include/dirent_h.c',
	'include/errno_h.c',
	'include/fcntl_h.c',
	'include/getopt_h.c',
	'include/ifaddrs_h.c',
	'include/libaio_h.c',
	'include/limits_h.c',
	'include/netdb_h.c',
	'include/poll_h.c',
	'include/pthread_h.c',
	'include/regex_h.c',
	'include/sched_h.c',
	'include/semaphore_h.c',
	'include/signal_h.c',
	'include/stdio_h.c',
	'include/stdlib_h.c',
	'include/string_h.c',
	'include/strings_h.c',
	'include/syslog_h.c',
	'include/termios_h.c',
	'include/unistd_h.c',
	'include/uuid_uuid_h.c',
]

unit_tests = executable('unit_tests',
		[ sources, include_sources ],
		c_args: [ cflags, '-D_WPDK_UNIT_TESTS_' ],
		include_directories: [ global_includes ],
		link_with : static_lib,
		dependencies : cunit_dep,
		install: true
)
