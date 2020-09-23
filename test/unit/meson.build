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
	'uio_ut.c',
	'uuid_ut.c',
	'unistd_ut.c',
]

include_sources = [
	'include_tests/_mingw_h.c',
	'include_tests/arpa_inet_h.c',
	'include_tests/net_if_h.c',
	'include_tests/netinet_in_h.c',
	'include_tests/netinet_tcp_h.c',
	'include_tests/openssl_md5_h.c',
	'include_tests/sys_cdefs_h.c',
	'include_tests/sys_epoll_h.c',
	'include_tests/sys_file_h.c',
	'include_tests/sys_ioctl_h.c',
	'include_tests/sys_mman_h.c',
	'include_tests/sys_queue_h.c',
	'include_tests/sys_resource_h.c',
	'include_tests/sys_select_h.c',
	'include_tests/sys_socket_h.c',
	'include_tests/sys_stat_h.c',
	'include_tests/sys_time_h.c',
	'include_tests/sys_uio_h.c',
	'include_tests/sys_un_h.c',
	'include_tests/sys_user_h.c',
	'include_tests/sys_wait_h.c',
	'include_tests/corecrt_h.c',
	'include_tests/dirent_h.c',
	'include_tests/errno_h.c',
	'include_tests/fcntl_h.c',
	'include_tests/getopt_h.c',
	'include_tests/ifaddrs_h.c',
	'include_tests/libaio_h.c',
	'include_tests/limits_h.c',
	'include_tests/netdb_h.c',
	'include_tests/poll_h.c',
	'include_tests/pthread_h.c',
	'include_tests/regex_h.c',
	'include_tests/sched_h.c',
	'include_tests/semaphore_h.c',
	'include_tests/signal_h.c',
	'include_tests/stdio_h.c',
	'include_tests/stdlib_h.c',
	'include_tests/string_h.c',
	'include_tests/strings_h.c',
	'include_tests/syslog_h.c',
	'include_tests/termios_h.c',
	'include_tests/unistd_h.c',
	'include_tests/uuid_uuid_h.c',
]

unit_tests = executable('unit_tests',
		[ sources, include_sources ],
		c_args: [ cflags, '-D_WPDK_UNIT_TESTS_' ],
		include_directories: [ global_includes ],
		link_with : [ static_lib ],
		dependencies : cunit_dep,
		link_args : [ '-lws2_32', '-lrpcrt4' ],
		install: true
)
