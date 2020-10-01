# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2020, MayaData Inc. All rights reserved.
# Copyright (c) 2020, DataCore Software Corporation. All rights reserved.

include_prefix = 'wpdk/'

install_headers(
	'_mingw.h',
	'corecrt.h',
	'dirent.h',
	'errno.h',
	'fcntl.h',
	'fnmatch.h',
	'getopt.h',
	'ifaddrs.h',
	'inaddr.h',
	'libaio.h',
	'limits.h',
	'netdb.h',
	'poll.h',
	'pthread.h',
	'pwd.h',
	'regex.h',
	'sched.h',
	'semaphore.h',
	'signal.h',
	'stdio.h',
	'stdlib.h',
	'string.h',
	'strings.h',
	'syslog.h',
	'termios.h',
	'unistd.h',
	'next/setjmp.h',
	subdir: include_prefix
)

install_headers(
	'sys/cdefs.h',
	'sys/epoll.h',
	'sys/file.h',
	'sys/ioctl.h',
	'sys/mman.h',
	'sys/queue.h',
	'sys/resource.h',
	'sys/select.h',
	'sys/socket.h',
	'sys/stat.h',
	'sys/time.h',
	'sys/uio.h',
	'sys/un.h',
	'sys/user.h',
	'sys/wait.h',
	subdir: include_prefix + 'sys'
)

install_headers(
	'wpdk/header.h',
	'wpdk/types.h',
	'wpdk/windows.h',
	subdir: include_prefix + 'wpdk'
)

install_headers(
	'arpa/inet.h',
	subdir: include_prefix + 'arpa'
)

install_headers(
	'net/if.h',
	subdir: include_prefix + 'net'
)

install_headers(
	'netinet/in.h',
	'netinet/ip.h',
	'netinet/ip6.h',
	'netinet/tcp.h',
	subdir: include_prefix + 'netinet'
)

install_headers(
	'openssl/md5.h',
	subdir: include_prefix + 'openssl'
)

install_headers(
	'sec_api/stdlib_s.h',
	'sec_api/string_s.h',
	subdir : include_prefix + 'sec_api'
)

install_headers(
	'uuid/uuid.h',
	subdir: include_prefix + 'uuid'
)
