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
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <CUnit/Basic.h>


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
test_fd_set(void)
{
	int rc, fd1, fd2;
	fd_set set;

	/* Open socket */
	fd1 = socket(AF_INET, SOCK_STREAM, 0);
	CU_ASSERT(fd1 != -1);

	/* Open socket */
	fd2 = socket(AF_INET, SOCK_STREAM, 0);
	CU_ASSERT(fd2 != -1);

	/* Check zero */
	FD_ZERO(&set);
	CU_ASSERT(FD_ISSET(fd1, &set) == 0);
	CU_ASSERT(FD_ISSET(fd2, &set) == 0);

	/* Check set fd1 */
	FD_SET(fd1, &set);
	CU_ASSERT(FD_ISSET(fd1, &set) != 0);
	CU_ASSERT(FD_ISSET(fd2, &set) == 0);

	/* Check set fd2 */
	FD_SET(fd2, &set);
	CU_ASSERT(FD_ISSET(fd1, &set) != 0);
	CU_ASSERT(FD_ISSET(fd2, &set) != 0);

	/* Check clear fd1 */
	FD_CLR(fd1, &set);
	CU_ASSERT(FD_ISSET(fd1, &set) == 0);
	CU_ASSERT(FD_ISSET(fd2, &set) != 0);

	/* Check clear fd2 */
	FD_CLR(fd2, &set);
	CU_ASSERT(FD_ISSET(fd1, &set) == 0);
	CU_ASSERT(FD_ISSET(fd2, &set) == 0);

	rc = close(fd1);
	CU_ASSERT(rc == 0);

	rc = close(fd2);
	CU_ASSERT(rc == 0);
}


static void *
test_select_server(void *arg)
{
	struct sockaddr_un addr;
	int fd1 = *(int *)arg;
	socklen_t len;
	size_t nbytes;
	char buf[50];
	int rc, fd;

	/* Accept connections */
	len = sizeof(addr);
	memset(&addr, 0, sizeof(addr));
	fd = accept(fd1, (struct sockaddr *)&addr, &len);
	if (fd == -1) return 0;

	/* Receive data */
	nbytes = recv(fd, buf, sizeof(buf), 0);
	if (nbytes != 12) return 0;

	/* Send data */
	nbytes = send(fd, "Hello World!", 12, 0);
	if (nbytes != 12) return 0;

	/* Receive data */
	nbytes = recv(fd, buf, sizeof(buf), 0);
	if (nbytes != 12) return 0;

	rc = close(fd);
	if (rc != 0) return 0;

	return (void *)1;
}


static void
test_select(void)
{
	const char *path = "testsocket";
	struct timeval delay = { 0 };
	struct sockaddr_un un;
	fd_set rd, wr, ex;
	pthread_t thread;
	int rc, fd1, fd2;
	size_t nbytes;
	void *result;

	unlink(path);

	/* Open server socket */
	fd1 = socket(AF_UNIX, SOCK_STREAM, 0);
	CU_ASSERT(fd1 != -1);

	/* Bind server socket */
	un.sun_family = AF_UNIX;
	strncpy(un.sun_path, path, sizeof(un.sun_path));
	rc = bind(fd1, (struct sockaddr *)&un, sizeof(un));
	CU_ASSERT(rc == 0);

	rc = listen(fd1, 5);
	CU_ASSERT(rc == 0);

	/* Create server */
	rc = pthread_create(&thread, NULL, test_select_server, (void *)&fd1);
	CU_ASSERT(rc == 0);

	/* Open client socket */
	fd2 = socket(AF_UNIX, SOCK_STREAM, 0);
	CU_ASSERT(fd2 != -1);

	/* Connect client */
	rc = connect(fd2, (struct sockaddr *)&un, sizeof(un));
	CU_ASSERT(rc == 0);

	/* Check read select */
	FD_ZERO(&rd);
	FD_SET(fd2, &rd);
	delay.tv_sec = 0;
	delay.tv_usec = 500000;
	rc = select(FD_SETSIZE, &rd, NULL, NULL, &delay);
	CU_ASSERT(rc == 0);
	CU_ASSERT(FD_ISSET(fd2, &rd) == 0);

	/* Check write select */
	FD_ZERO(&wr);
	FD_SET(fd2, &wr);
	delay.tv_sec = 0;
	delay.tv_usec = 0;
	rc = select(FD_SETSIZE, NULL, &wr, NULL, &delay);
	CU_ASSERT(rc == 1);
	CU_ASSERT(FD_ISSET(fd2, &wr) == 1);

	/* Check except select */
	FD_ZERO(&ex);
	FD_SET(fd2, &ex);
	delay.tv_sec = 0;
	delay.tv_usec = 0;
	rc = select(FD_SETSIZE, NULL, NULL, &ex, &delay);
	CU_ASSERT(rc == 0);
	CU_ASSERT(FD_ISSET(fd2, &ex) == 0);

	/* Check select */
	FD_ZERO(&rd);
	FD_SET(fd2, &rd);
	FD_ZERO(&wr);
	FD_SET(fd2, &wr);
	FD_ZERO(&ex);
	FD_SET(fd2, &ex);
	delay.tv_sec = 0;
	delay.tv_usec = 0;
	rc = select(FD_SETSIZE, &rd, &wr, &ex, &delay);
	CU_ASSERT(rc == 1);
	CU_ASSERT(FD_ISSET(fd2, &rd) == 0);
	CU_ASSERT(FD_ISSET(fd2, &wr) == 1);
	CU_ASSERT(FD_ISSET(fd2, &ex) == 0);

	/* Send data */
	nbytes = send(fd2, "Hello World!", 12, 0);
	CU_ASSERT(nbytes == 12);

	/* Check read select */
	FD_ZERO(&rd);
	FD_SET(fd2, &rd);
	delay.tv_sec = 1;
	delay.tv_usec = 0;
	rc = select(FD_SETSIZE, &rd, NULL, NULL, &delay);
	CU_ASSERT(rc == 1);
	CU_ASSERT(FD_ISSET(fd2, &rd) == 1);

	/* Check write select */
	FD_ZERO(&wr);
	FD_SET(fd2, &wr);
	delay.tv_sec = 0;
	delay.tv_usec = 0;
	rc = select(FD_SETSIZE, NULL, &wr, NULL, &delay);
	CU_ASSERT(rc == 1);
	CU_ASSERT(FD_ISSET(fd2, &wr) == 1);

	/* Check except select */
	FD_ZERO(&ex);
	FD_SET(fd2, &ex);
	delay.tv_sec = 0;
	delay.tv_usec = 0;
	rc = select(FD_SETSIZE, NULL, NULL, &ex, &delay);
	CU_ASSERT(rc == 0);
	CU_ASSERT(FD_ISSET(fd2, &ex) == 0);

	/* Check select */
	FD_ZERO(&rd);
	FD_SET(fd2, &rd);
	FD_ZERO(&wr);
	FD_SET(fd2, &wr);
	FD_ZERO(&ex);
	FD_SET(fd2, &ex);
	delay.tv_sec = 0;
	delay.tv_usec = 0;
	rc = select(FD_SETSIZE, &rd, &wr, &ex, &delay);
	CU_ASSERT(rc == 2);
	CU_ASSERT(FD_ISSET(fd2, &rd) == 1);
	CU_ASSERT(FD_ISSET(fd2, &wr) == 1);
	CU_ASSERT(FD_ISSET(fd2, &ex) == 0);

	/* Send data */
	nbytes = send(fd2, "Hello World!", 12, 0);
	CU_ASSERT(nbytes == 12);

	/* Collect server result */
	rc = pthread_join(thread, &result);
	CU_ASSERT(rc == 0);
	CU_ASSERT(result == (void *)1);

	rc = close(fd1);
	CU_ASSERT(rc == 0);

	rc = close(fd2);
	CU_ASSERT(rc == 0);

	unlink(path);
}


void add_select_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("select", null_init, null_clean);

	CU_ADD_TEST(suite, test_fd_set);
	CU_ADD_TEST(suite, test_select);
}
