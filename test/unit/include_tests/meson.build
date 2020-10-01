# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2020, MayaData Inc. All rights reserved.
# Copyright (c) 2020, DataCore Software Corporation. All rights reserved.

c_files = [
	'_mingw_h.c',
	'arpa_inet_h.c',
	'net_if_h.c',
	'netinet_in_h.c',
	'netinet_ip_h.c',
	'netinet_ip6_h.c',
	'netinet_tcp_h.c',
	'openssl_md5_h.c',
	'sys_cdefs_h.c',
	'sys_epoll_h.c',
	'sys_file_h.c',
	'sys_ioctl_h.c',
	'sys_mman_h.c',
	'sys_queue_h.c',
	'sys_resource_h.c',
	'sys_select_h.c',
	'sys_socket_h.c',
	'sys_stat_h.c',
	'sys_time_h.c',
	'sys_types_h.c',
	'sys_uio_h.c',
	'sys_un_h.c',
	'sys_user_h.c',
	'sys_wait_h.c',
	'corecrt_h.c',
	'dirent_h.c',
	'errno_h.c',
	'fcntl_h.c',
	'fnmatch_h.c',	
	'getopt_h.c',
	'ifaddrs_h.c',
	'inaddr_h.c',
	'libaio_h.c',
	'limits_h.c',
	'netdb_h.c',
	'poll_h.c',
	'pthread_h.c',
	'pwd_h.c',
	'regex_h.c',
	'sched_h.c',
	'semaphore_h.c',
	'setjmp_h.c',
	'signal_h.c',
	'stdio_h.c',
	'stdlib_h.c',
	'string_h.c',
	'strings_h.c',
	'syslog_h.c',
	'termios_h.c',
	'unistd_h.c',
	'uuid_uuid_h.c',
]

include_sources = files(c_files)

foreach i : c_files
	include_sources += configure_file(copy: true, input: i, output: i + 'pp')
endforeach
