/*-
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Copyright (c) 2020, MayaData Inc. All rights reserved.
 *  Copyright (c) 2020, DataCore Software Corporation. All rights reserved.
 * 
 *  POSIX details are based on the Open Group Base Specification Issue 7,
 *  2018 edition at https://pubs.opengroup.org/onlinepubs/9699919799/
 * 
 *  Details about Linux extensions are based on the Linux man-pages project
 *  at https://www.kernel.org/doc/man-pages/
 */

#include <wpdk/internal.h>
#include <sys/cdefs.h>
#include <sys/epoll.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/queue.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <openssl/md5.h>
#include <uuid/uuid.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <getopt.h>
#include <ifaddrs.h>
#include <libaio.h>
#include <limits.h>
#include <netdb.h>
#include <poll.h>
#include <pthread.h>
#include <regex.h>
#include <sched.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <syslog.h>
#include <termios.h>
#include <unistd.h>

// Check setupapi.h (DPDK)
#include <windows.h>
#include <setupapi.h>

#include <CUnit/Basic.h>

void include_cpp__mingw_h(void);
void include_cpp_arpa_inet_h(void);
void include_cpp_net_if_h(void);
void include_cpp_netinet_in_h(void);
void include_cpp_netinet_ip_h(void);
void include_cpp_netinet_tcp_h(void);
void include_cpp_openssl_md5_h(void);
void include_cpp_sys_cdefs_h(void);
void include_cpp_sys_epoll_h(void);
void include_cpp_sys_file_h(void);
void include_cpp_sys_ioctl_h(void);
void include_cpp_sys_mman_h(void);
void include_cpp_sys_queue_h(void);
void include_cpp_sys_resource_h(void);
void include_cpp_sys_select_h(void);
void include_cpp_sys_socket_h(void);
void include_cpp_sys_stat_h(void);
void include_cpp_sys_time_h(void);
void include_cpp_sys_uio_h(void);
void include_cpp_sys_un_h(void);
void include_cpp_sys_user_h(void);
void include_cpp_sys_wait_h(void);
void include_cpp_corecrt_h(void);
void include_cpp_dirent_h(void);
void include_cpp_errno_h(void);
void include_cpp_fcntl_h(void);
void include_cpp_fnmatch_h(void);
void include_cpp_getopt_h(void);
void include_cpp_ifaddrs_h(void);
void include_cpp_libaio_h(void);
void include_cpp_limits_h(void);
void include_cpp_netdb_h(void);
void include_cpp_poll_h(void);
void include_cpp_pthread_h(void);
void include_cpp_regex_h(void);
void include_cpp_sched_h(void);
void include_cpp_semaphore_h(void);
void include_cpp_signal_h(void);
void include_cpp_stdio_h(void);
void include_cpp_stdlib_h(void);
void include_cpp_string_h(void);
void include_cpp_strings_h(void);
void include_cpp_syslog_h(void);
void include_cpp_termios_h(void);
void include_cpp_unistd_h(void);
void include_cpp_uuid_uuid_h(void);


static int
null_init(void)
{
	return 0;
}


static int
null_clean(void)
{
	return 0;
}


static void
include_cpp_all_h(void)
{
}


extern "C" void
add_include_cpp_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("include_cpp", null_init, null_clean);

	CU_ADD_TEST(suite, include_cpp_all_h);
	CU_ADD_TEST(suite, include_cpp__mingw_h);
	CU_ADD_TEST(suite, include_cpp_arpa_inet_h);
	CU_ADD_TEST(suite, include_cpp_net_if_h);
	CU_ADD_TEST(suite, include_cpp_netinet_in_h);
	CU_ADD_TEST(suite, include_cpp_netinet_ip_h);
	CU_ADD_TEST(suite, include_cpp_netinet_tcp_h);
	CU_ADD_TEST(suite, include_cpp_openssl_md5_h);
	CU_ADD_TEST(suite, include_cpp_sys_cdefs_h);
	CU_ADD_TEST(suite, include_cpp_sys_epoll_h);
	CU_ADD_TEST(suite, include_cpp_sys_file_h);
	CU_ADD_TEST(suite, include_cpp_sys_ioctl_h);
	CU_ADD_TEST(suite, include_cpp_sys_mman_h);
	CU_ADD_TEST(suite, include_cpp_sys_queue_h);
	CU_ADD_TEST(suite, include_cpp_sys_resource_h);
	CU_ADD_TEST(suite, include_cpp_sys_select_h);
	CU_ADD_TEST(suite, include_cpp_sys_socket_h);
	CU_ADD_TEST(suite, include_cpp_sys_stat_h);
	CU_ADD_TEST(suite, include_cpp_sys_time_h);
	CU_ADD_TEST(suite, include_cpp_sys_uio_h);
	CU_ADD_TEST(suite, include_cpp_sys_un_h);
	CU_ADD_TEST(suite, include_cpp_sys_user_h);
	CU_ADD_TEST(suite, include_cpp_sys_wait_h);
	CU_ADD_TEST(suite, include_cpp_corecrt_h);
	CU_ADD_TEST(suite, include_cpp_dirent_h);
	CU_ADD_TEST(suite, include_cpp_errno_h);
	CU_ADD_TEST(suite, include_cpp_fcntl_h);
	CU_ADD_TEST(suite, include_cpp_fnmatch_h);
	CU_ADD_TEST(suite, include_cpp_getopt_h);
	CU_ADD_TEST(suite, include_cpp_ifaddrs_h);
	CU_ADD_TEST(suite, include_cpp_libaio_h);
	CU_ADD_TEST(suite, include_cpp_limits_h);
	CU_ADD_TEST(suite, include_cpp_netdb_h);
	CU_ADD_TEST(suite, include_cpp_poll_h);
	CU_ADD_TEST(suite, include_cpp_pthread_h);
	CU_ADD_TEST(suite, include_cpp_regex_h);
	CU_ADD_TEST(suite, include_cpp_sched_h);
	CU_ADD_TEST(suite, include_cpp_semaphore_h);
	CU_ADD_TEST(suite, include_cpp_signal_h);
	CU_ADD_TEST(suite, include_cpp_stdio_h);
	CU_ADD_TEST(suite, include_cpp_stdlib_h);
	CU_ADD_TEST(suite, include_cpp_string_h);
	CU_ADD_TEST(suite, include_cpp_strings_h);
	CU_ADD_TEST(suite, include_cpp_syslog_h);
	CU_ADD_TEST(suite, include_cpp_termios_h);
	CU_ADD_TEST(suite, include_cpp_unistd_h);
	CU_ADD_TEST(suite, include_cpp_uuid_uuid_h);
}
