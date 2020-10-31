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


struct server {
	const char	*path;
	int			server;
	int			client;
	pthread_t	thread;
	void		*result;
};


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


static void *
server_thread(void *arg)
{
	struct server *s = (struct server *)arg;
	size_t inbytes, outbytes;
	struct sockaddr_un addr;
	socklen_t len;
	char buf[50];
	int rc, fd;

	/* Listen */
	rc = listen(s->server, 5);

	if (rc == -1) {
		fprintf(stderr, "server: listen failed (%s)\n", strerror(errno));
		return 0;
	}

	/* Accept connections */
	len = sizeof(addr);
	memset(&addr, 0, sizeof(addr));

	fd = accept(s->server, (struct sockaddr *)&addr, &len);

	if (rc == -1) {
		fprintf(stderr, "server: accept failed (%s)\n", strerror(errno));
		return 0;
	}

	while (1) {
		/* Receive data */
		inbytes = recv(fd, buf, sizeof(buf), 0);

		if (inbytes == (size_t)-1) {
			fprintf(stderr, "server: recv failed (%s)\n", strerror(errno));
			return 0;
		}

		if (inbytes == 1 && buf[0] == '.') break;

		/* Send data */
		outbytes = send(fd, buf, inbytes, 0);

		if (outbytes != inbytes) {
			fprintf(stderr, "server: send failed (outbytes %lld, %s)\n",
				outbytes, strerror(errno));
			return 0;
		}
	}

	rc = close(fd);

	if (rc == -1) {
		fprintf(stderr, "server: close failed (%s)\n", strerror(errno));
		return 0;
	}

	return (void *)1;
}


static void
start_server(struct server *s, const char *path)
{
	struct sockaddr_un un;
	int rc;

	s->path = path;
	unlink(s->path);

	/* Open server socket */
	s->server = socket(AF_UNIX, SOCK_STREAM, 0);
	CU_ASSERT(s->server != -1);

	/* Bind server socket */
	un.sun_family = AF_UNIX;
	strncpy(un.sun_path, s->path, sizeof(un.sun_path));
	rc = bind(s->server, (struct sockaddr *)&un, sizeof(un));
	CU_ASSERT(rc == 0);

	/* Create server */
	rc = pthread_create(&s->thread, NULL, server_thread, (void *)s);
	CU_ASSERT(rc == 0);

	/* Open client socket */
	s->client = socket(AF_UNIX, SOCK_STREAM, 0);
	CU_ASSERT(s->client != -1);

	/* Connect client */
	rc = connect(s->client, (struct sockaddr *)&un, sizeof(un));
	CU_ASSERT(rc == 0);
}


static void
stop_server(struct server *s)
{
	size_t nbytes;
	DWORD rc;

	/* Send data */
	nbytes = send(s->client, ".", 1, 0);
	CU_ASSERT(nbytes == 1);

	/* Collect server result */
	rc = pthread_join(s->thread, &s->result);
	CU_ASSERT(rc == 0);
	CU_ASSERT(s->result == (void *)1);

	rc = close(s->server);
	CU_ASSERT(rc == 0);

	rc = close(s->client);
	CU_ASSERT(rc == 0);

	unlink(s->path);
}


static void
test_send(void)
{
	struct server s;
	size_t nbytes;
	char buf[50];	

	start_server(&s, "testsocket");

	/* Send data */
	nbytes = send(s.client, "Hello World!", 12, 0);
	CU_ASSERT(nbytes == 12);

	/* Receive data */
	nbytes = recv(s.client, buf, sizeof(buf), 0);
	CU_ASSERT(nbytes == 12);
	CU_ASSERT(memcmp(buf, "Hello World!", 12) == 0);

	stop_server(&s);
}


void add_socket_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("socket", null_init, null_clean);

	CU_ADD_TEST(suite, test_send);
}
