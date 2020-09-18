#include <wpdk/internal.h>
#include <stdio.h>

#include <CUnit/Basic.h>

void include__mingw_h(void);
void include_arpa_inet_h(void);
void include_net_if_h(void);
void include_netinet_in_h(void);
void include_netinet_tcp_h(void);
void include_openssl_md5_h(void);
void include_sys_cdefs_h(void);
void include_sys_epoll_h(void);
void include_sys_file_h(void);
void include_sys_ioctl_h(void);
void include_sys_mman_h(void);
void include_sys_queue_h(void);
void include_sys_resource_h(void);
void include_sys_select_h(void);
void include_sys_socket_h(void);
void include_sys_stat_h(void);
void include_sys_time_h(void);
void include_sys_uio_h(void);
void include_sys_un_h(void);
void include_sys_user_h(void);
void include_sys_wait_h(void);
void include_corecrt_h(void);
void include_dirent_h(void);
void include_errno_h(void);
void include_fcntl_h(void);
void include_getopt_h(void);
void include_ifaddrs_h(void);
void include_libaio_h(void);
void include_limits_h(void);
void include_netdb_h(void);
void include_poll_h(void);
void include_pthread_h(void);
void include_regex_h(void);
void include_sched_h(void);
void include_semaphore_h(void);
void include_signal_h(void);
void include_stdio_h(void);
void include_stdlib_h(void);
void include_string_h(void);
void include_strings_h(void);
void include_syslog_h(void);
void include_termios_h(void);
void include_unistd_h(void);
void include_uuid_uuid_h(void);


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


void
add_include_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("include", null_init, null_clean);

	CU_ADD_TEST(suite, include__mingw_h);
	CU_ADD_TEST(suite, include_arpa_inet_h);
	CU_ADD_TEST(suite, include_net_if_h);
	CU_ADD_TEST(suite, include_netinet_in_h);
	CU_ADD_TEST(suite, include_netinet_tcp_h);
	CU_ADD_TEST(suite, include_openssl_md5_h);
	CU_ADD_TEST(suite, include_sys_cdefs_h);
	CU_ADD_TEST(suite, include_sys_epoll_h);
	CU_ADD_TEST(suite, include_sys_file_h);
	CU_ADD_TEST(suite, include_sys_ioctl_h);
	CU_ADD_TEST(suite, include_sys_mman_h);
	CU_ADD_TEST(suite, include_sys_queue_h);
	CU_ADD_TEST(suite, include_sys_resource_h);
	CU_ADD_TEST(suite, include_sys_select_h);
	CU_ADD_TEST(suite, include_sys_socket_h);
	CU_ADD_TEST(suite, include_sys_stat_h);
	CU_ADD_TEST(suite, include_sys_time_h);
	CU_ADD_TEST(suite, include_sys_uio_h);
	CU_ADD_TEST(suite, include_sys_un_h);
	CU_ADD_TEST(suite, include_sys_user_h);
	CU_ADD_TEST(suite, include_sys_wait_h);
	CU_ADD_TEST(suite, include_corecrt_h);
	CU_ADD_TEST(suite, include_dirent_h);
	CU_ADD_TEST(suite, include_errno_h);
	CU_ADD_TEST(suite, include_fcntl_h);
	CU_ADD_TEST(suite, include_getopt_h);
	CU_ADD_TEST(suite, include_ifaddrs_h);
	CU_ADD_TEST(suite, include_libaio_h);
	CU_ADD_TEST(suite, include_limits_h);
	CU_ADD_TEST(suite, include_netdb_h);
	CU_ADD_TEST(suite, include_poll_h);
	CU_ADD_TEST(suite, include_pthread_h);
	CU_ADD_TEST(suite, include_regex_h);
	CU_ADD_TEST(suite, include_sched_h);
	CU_ADD_TEST(suite, include_semaphore_h);
	CU_ADD_TEST(suite, include_signal_h);
	CU_ADD_TEST(suite, include_stdio_h);
	CU_ADD_TEST(suite, include_stdlib_h);
	CU_ADD_TEST(suite, include_string_h);
	CU_ADD_TEST(suite, include_strings_h);
	CU_ADD_TEST(suite, include_syslog_h);
	CU_ADD_TEST(suite, include_termios_h);
	CU_ADD_TEST(suite, include_unistd_h);
	CU_ADD_TEST(suite, include_uuid_uuid_h);
}
