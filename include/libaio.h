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
 *
 *  There seems to be a lack of definitive documentation about libaio.
 *  This implementation has been based on details found at:
 * 
 *  https://github.com/littledan/linux-aio
 *  https://www.fsl.cs.sunysb.edu/~vass/linux-aio.txt
 *  https://blog.cloudflare.com/io_submit-the-epoll-alternative-youve-never-heard-about/
 */

#ifndef _WPDK_LIBAIO_H_
#define _WPDK_LIBAIO_H_

#include <sys/cdefs.h>
#include <sys/_types.h>

_WPDK_BEGIN_C_HEADER

enum io_iocb_cmd {
	IO_CMD_PREAD = 1,
	IO_CMD_PWRITE,
	IO_CMD_PREADV,
	IO_CMD_PWRITEV
};

struct iocb {
	void		*data;
	short		aio_lio_opcode;
	short		aio_reqprio;
	int			aio_fildes;

	union {
		struct {
			void		*buf;
			size_t		nbytes;
			off_t		offset;
		} c;
	} u;
};

#define aio_buf			u.c.buf
#define aio_nbytes		u.c.nbytes
#define aio_offset		u.c.offset

struct io_event {
	void		*data;
	struct iocb	*obj;
	uint64_t	res;
	uint64_t	res2;
};

typedef struct io_context {
	uint32_t id;
	uint32_t size;
	uint32_t head;
	uint32_t tail;
	uint32_t version;
	uint32_t compat_features;
	uint32_t incompat_features;
	uint32_t header_length;	
} *io_context_t;

int io_setup(int maxevents, io_context_t *ctxp);
int io_destroy(io_context_t ctx_id);
int io_submit(io_context_t ctx_id, long nr, struct iocb *ios[]);
int io_getevents(io_context_t ctx_id, long min_nr, long nr, struct io_event *events, struct timespec *timeout);
int io_cancel(io_context_t ctx_id, struct iocb *iocb, struct io_event *evt);

static inline void io_prep_pread(struct iocb *iocb, int fd, void *buf, size_t count, off_t offset) {
	memset(iocb, 0, sizeof(struct iocb));
	iocb->aio_lio_opcode = IO_CMD_PREAD;
	iocb->aio_fildes = fd;
	iocb->u.c.buf = buf;
	iocb->u.c.nbytes = count;
	iocb->u.c.offset = offset;
}

static inline void io_prep_pwrite(struct iocb *iocb, int fd, void *buf, size_t count, off_t offset) {
	memset(iocb, 0, sizeof(struct iocb));
	iocb->aio_lio_opcode = IO_CMD_PWRITE;
	iocb->aio_fildes = fd;
	iocb->u.c.buf = buf;
	iocb->u.c.nbytes = count;
	iocb->u.c.offset = offset;
}

static inline void io_prep_preadv(struct iocb *iocb, int fd, struct iovec *iov, int iovcnt, off_t offset) {
	memset(iocb, 0, sizeof(struct iocb));
	iocb->aio_lio_opcode = IO_CMD_PREADV;
	iocb->aio_fildes = fd;
	iocb->u.c.buf = iov;
	iocb->u.c.nbytes = iovcnt;
	iocb->u.c.offset = offset;
}

static inline void io_prep_pwritev(struct iocb *iocb, int fd, struct iovec *iov, int iovcnt, off_t offset) {
	memset(iocb, 0, sizeof(struct iocb));
	iocb->aio_lio_opcode = IO_CMD_PWRITEV;
	iocb->aio_fildes = fd;
	iocb->u.c.buf = iov;
	iocb->u.c.nbytes = iovcnt;
	iocb->u.c.offset = offset;
}

_WPDK_END_C_HEADER

#endif /* _WPDK_LIBAIO_H_ */
