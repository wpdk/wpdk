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
#include "include_ut.h"

#include <CUnit/Basic.h>


#ifdef __cplusplus
#define TEST_NAME		"include_cpp"
#define ADD_TESTS()		extern "C" void add_include_cpp_tests()
#define DEF_TEST(x)		void include_cpp_ ## x(void)
#define ADD_TEST(x)		CU_ADD_TEST(suite, include_cpp_ ## x)
#else
#define TEST_NAME		"include_c"
#define ADD_TESTS()		void add_include_c_tests()
#define DEF_TEST(x)		void include_c_ ## x(void)
#define ADD_TEST(x)		CU_ADD_TEST(suite, include_c_ ## x)
#endif


DEF_TEST(_mingw_h);
DEF_TEST(_timeval_h);
DEF_TEST(arpa_inet_h);
DEF_TEST(net_if_h);
DEF_TEST(netinet_in_h);
DEF_TEST(netinet_ip_h);
DEF_TEST(netinet_ip6_h);
DEF_TEST(netinet_tcp_h);
DEF_TEST(openssl_md5_h);
DEF_TEST(sys_cdefs_h);
DEF_TEST(sys_epoll_h);
DEF_TEST(sys_file_h);
DEF_TEST(sys_ioctl_h);
DEF_TEST(sys_mman_h);
DEF_TEST(sys_queue_h);
DEF_TEST(sys_resource_h);
DEF_TEST(sys_select_h);
DEF_TEST(sys_socket_h);
DEF_TEST(sys_stat_h);
DEF_TEST(sys_time_h);
DEF_TEST(sys_uio_h);
DEF_TEST(sys_un_h);
DEF_TEST(sys_user_h);
DEF_TEST(sys_wait_h);
DEF_TEST(corecrt_h);
DEF_TEST(assert_h);
DEF_TEST(dirent_h);
DEF_TEST(errno_h);
DEF_TEST(fcntl_h);
DEF_TEST(fnmatch_h);
DEF_TEST(getopt_h);
DEF_TEST(ifaddrs_h);
DEF_TEST(inaddr_h);
DEF_TEST(libaio_h);
DEF_TEST(limits_h);
DEF_TEST(netdb_h);
DEF_TEST(poll_h);
DEF_TEST(pthread_h);
DEF_TEST(pwd_h);
DEF_TEST(regex_h);
DEF_TEST(sched_h);
DEF_TEST(semaphore_h);
DEF_TEST(setjmp_h);
DEF_TEST(signal_h);
DEF_TEST(stdio_h);
DEF_TEST(stdlib_h);
DEF_TEST(string_h);
DEF_TEST(strings_h);
DEF_TEST(syslog_h);
DEF_TEST(termios_h);
DEF_TEST(time_h);
DEF_TEST(unistd_h);
DEF_TEST(uuid_uuid_h);


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
include_all(void)
{
}


ADD_TESTS()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite(TEST_NAME, null_init, null_clean);

	CU_ADD_TEST(suite, include_all);
	ADD_TEST(_mingw_h);
	ADD_TEST(_timeval_h);
	ADD_TEST(arpa_inet_h);
	ADD_TEST(net_if_h);
	ADD_TEST(netinet_in_h);
	ADD_TEST(netinet_ip_h);
	ADD_TEST(netinet_ip6_h);
	ADD_TEST(netinet_tcp_h);
	ADD_TEST(openssl_md5_h);
	ADD_TEST(sys_cdefs_h);
	ADD_TEST(sys_epoll_h);
	ADD_TEST(sys_file_h);
	ADD_TEST(sys_ioctl_h);
	ADD_TEST(sys_mman_h);
	ADD_TEST(sys_queue_h);
	ADD_TEST(sys_resource_h);
	ADD_TEST(sys_select_h);
	ADD_TEST(sys_socket_h);
	ADD_TEST(sys_stat_h);
	ADD_TEST(sys_time_h);
	ADD_TEST(sys_uio_h);
	ADD_TEST(sys_un_h);
	ADD_TEST(sys_user_h);
	ADD_TEST(sys_wait_h);
	ADD_TEST(assert_h);
	ADD_TEST(corecrt_h);
	ADD_TEST(dirent_h);
	ADD_TEST(errno_h);
	ADD_TEST(fcntl_h);
	ADD_TEST(fnmatch_h);
	ADD_TEST(getopt_h);
	ADD_TEST(ifaddrs_h);
	ADD_TEST(inaddr_h);
	ADD_TEST(libaio_h);
	ADD_TEST(limits_h);
	ADD_TEST(netdb_h);
	ADD_TEST(poll_h);
	ADD_TEST(pthread_h);
	ADD_TEST(pwd_h);
	ADD_TEST(regex_h);
	ADD_TEST(sched_h);
	ADD_TEST(semaphore_h);
	ADD_TEST(setjmp_h);
	ADD_TEST(signal_h);
	ADD_TEST(stdio_h);
	ADD_TEST(stdlib_h);
	ADD_TEST(string_h);
	ADD_TEST(strings_h);
	ADD_TEST(syslog_h);
	ADD_TEST(termios_h);
	ADD_TEST(time_h);
	ADD_TEST(unistd_h);
	ADD_TEST(uuid_uuid_h);
}
