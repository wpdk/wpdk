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
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

#include <CUnit/Basic.h>


struct server {
	const char	*path;
	int			server;
	int			client;
	pthread_t	thread;
	void		*result;
	int			stop;
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

	if (fd == -1) {
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
			fprintf(stderr, "server: send failed (outbytes %zd, %s)\n",
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
	strncpy_s(un.sun_path, sizeof(un.sun_path), s->path, sizeof(un.sun_path) - 1);
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
test_socket(void)
{
	int rc, fd;

	/* Check invalid domain */
	fd = socket(-1, SOCK_STREAM, 0);
	CU_ASSERT(fd == -1 && errno == EAFNOSUPPORT);

	/* Check UNIX domain */
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	CU_ASSERT(fd != -1);
	rc = close(fd);
	CU_ASSERT(rc == 0);

	/* Check INET domain */
	fd = socket(AF_INET, SOCK_STREAM, 0);
	CU_ASSERT(fd != -1);

	/* Check blocking mode */
	rc = fcntl(fd, F_GETFL, 0);
	CU_ASSERT(rc == 0);
	rc = close(fd);
	CU_ASSERT(rc == 0);

	/* Check non-blocking mode */
	fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	CU_ASSERT(fd != -1);
	rc = fcntl(fd, F_GETFL, 0);
	CU_ASSERT(rc == O_NONBLOCK);
	rc = close(fd);
	CU_ASSERT(rc == 0);
}


static void
test_bind(void)
{
	const char *path = "testsocket";
	struct sockaddr_un un;
	struct sockaddr_in in;
	int rc, fd, fd2;

	unlink(path);

	/* Open socket */
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	CU_ASSERT(fd != -1);

	/* Check invalid family */
	un.sun_family = (ADDRESS_FAMILY)-1;
	strncpy(un.sun_path, path, sizeof(un.sun_path));
	rc = bind(fd, (struct sockaddr *)&un, sizeof(un));
	CU_ASSERT(rc == -1 && errno == EAFNOSUPPORT);

	/* Bind socket */
	un.sun_family = AF_UNIX;
	strncpy(un.sun_path, path, sizeof(un.sun_path));
	rc = bind(fd, (struct sockaddr *)&un, sizeof(un));
	CU_ASSERT(rc == 0);

	/* Check socket exists */
	rc = mknod(path, S_IFREG|S_IRWXU|S_IRWXG|S_IRWXO, 0);
	CU_ASSERT(rc == -1);

	/* Open second socket */
	fd2 = socket(AF_UNIX, SOCK_STREAM, 0);
	CU_ASSERT(fd2 != -1);

	/* Check invalid bind */
	un.sun_family = AF_UNIX;
	strncpy(un.sun_path, path, sizeof(un.sun_path));
	rc = bind(fd, (struct sockaddr *)&un, sizeof(un));
	CU_ASSERT(rc == -1);

	rc = close(fd);
	CU_ASSERT(rc == 0);

	rc = close(fd2);
	CU_ASSERT(rc == 0);

	/* Open INET socket */
	fd = socket(AF_INET, SOCK_STREAM, 0);
	CU_ASSERT(fd != -1);

	/* Setup address */
	in.sin_family = AF_INET;
	in.sin_port = 0;
	rc = inet_pton(AF_INET, "127.0.0.1", &in.sin_addr);
	CU_ASSERT(rc == 1);

	/* Bind socket */
	rc = bind(fd, (struct sockaddr *)&in, sizeof(in));
	CU_ASSERT(rc == 0);

	rc = close(fd);
	CU_ASSERT(rc == 0);

	unlink(path);
}


static void *
client_thread(void *arg)
{
	struct server *s = (struct server *)arg;
	struct sockaddr_un un;
	int rc;

	un.sun_family = AF_UNIX;
	strncpy_s(un.sun_path, sizeof(un.sun_path), s->path, sizeof(un.sun_path) - 1);

	/* Connect client */
	rc = connect(s->client, (struct sockaddr *)&un, sizeof(un));

	if (rc != 0) {
		fprintf(stderr, "client: connect failed (%s)\n", strerror(errno));
		return 0;
	}

	while (!s->stop)
		Sleep(1);

	return (void *)1;
}


static void
start_client(struct server *s, const char *path)
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
	strncpy_s(un.sun_path, sizeof(un.sun_path), s->path, sizeof(un.sun_path) - 1);
	rc = bind(s->server, (struct sockaddr *)&un, sizeof(un));
	CU_ASSERT(rc == 0);

	/* Open client socket */
	s->client = socket(AF_UNIX, SOCK_STREAM, 0);
	CU_ASSERT(s->client != -1);

	/* Listen on server */
	rc = listen(s->server, 5);
	CU_ASSERT(rc == 0);

	s->stop = 0;

	/* Create client */
	rc = pthread_create(&s->thread, NULL, client_thread, (void *)s);
	CU_ASSERT(rc == 0);
}


static void
stop_client(struct server *s)
{
	DWORD rc;

	s->stop = 1;

	/* Collect client result */
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
test_accept(void)
{
	struct sockaddr_un addr;
	struct server s;
	socklen_t len;
	int fd, rc;

	/* Check blocking accept */
	start_client(&s, "testsocket");

	len = sizeof(addr);
	fd = accept(s.server, (struct sockaddr *)&addr, &len);
	CU_ASSERT(fd != -1);

	rc = fcntl(fd, F_GETFL, 0);
	CU_ASSERT(rc == 0);

	rc = close(fd);
	CU_ASSERT(rc == 0);
	stop_client(&s);

	/* Check non-blocking accept */
	start_client(&s, "testsocket");

	rc = fcntl(s.server, F_SETFL, O_NONBLOCK);
	CU_ASSERT(rc == 0);

	for (; 1; Sleep(1)) {
		len = sizeof(addr);
		fd = accept(s.server, (struct sockaddr *)&addr, &len);
		if (fd != -1 || errno != EWOULDBLOCK) break;
	}
	CU_ASSERT(fd != -1);

	rc = fcntl(fd, F_GETFL, 0);
	CU_ASSERT(rc == O_NONBLOCK);

	rc = close(fd);
	CU_ASSERT(rc == 0);
	stop_client(&s);
}


static void
test_accept4(void)
{
	struct sockaddr_un addr;
	struct server s;
	socklen_t len;
	int fd, rc;

	/* Check blocking accept */
	start_client(&s, "testsocket");

	len = sizeof(addr);
	fd = accept4(s.server, (struct sockaddr *)&addr, &len, 0);
	CU_ASSERT(fd != -1);

	rc = fcntl(fd, F_GETFL, 0);
	CU_ASSERT(rc == 0);

	rc = close(fd);
	CU_ASSERT(rc == 0);
	stop_client(&s);

	/* Check non-blocking accept */
	start_client(&s, "testsocket");

	len = sizeof(addr);
	fd = accept4(s.server, (struct sockaddr *)&addr, &len, SOCK_NONBLOCK);
	CU_ASSERT(fd != -1);

	rc = fcntl(fd, F_GETFL, 0);
	CU_ASSERT(rc == O_NONBLOCK);

	rc = close(fd);
	CU_ASSERT(rc == 0);

	/* Check blocking accept on non-blocking socket*/
	start_client(&s, "testsocket");

	rc = fcntl(s.server, F_SETFL, O_NONBLOCK);
	CU_ASSERT(rc == 0);

	for (; 1; Sleep(1)) {
		len = sizeof(addr);
		fd = accept4(s.server, (struct sockaddr *)&addr, &len, 0);
		if (fd != -1 || errno != EWOULDBLOCK) break;
	}
	CU_ASSERT(fd != -1);

	rc = fcntl(fd, F_GETFL, 0);
	CU_ASSERT(rc == 0);

	rc = close(fd);
	CU_ASSERT(rc == 0);
	stop_client(&s);

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

	CU_ADD_TEST(suite, test_socket);
	CU_ADD_TEST(suite, test_bind);
	CU_ADD_TEST(suite, test_accept);
	CU_ADD_TEST(suite, test_accept4);
	CU_ADD_TEST(suite, test_send);
}
