#include <wpdklib.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <libaio.h>

/*
 *  There seems to be a lack of definitive documentation about libaio.
 *  This implementation has been based on details found at:
 * 
 *  https://github.com/littledan/linux-aio
 *  https://www.fsl.cs.sunysb.edu/~vass/linux-aio.txt
 *  https://blog.cloudflare.com/io_submit-the-epoll-alternative-youve-never-heard-about/
 */


struct io_header;

struct io_task {
	union {
		OVERLAPPED			io;
		struct io_iov {
			long	active;
			long	iovcnt;
		} iov;
	};
	union {
		struct iocb			*iocb;
		void				*type;
	};
	union {
		struct io_header	*hdr;
		struct io_task		*task;
	};
	union {
		void				*data;
		long long			result;
	};
};

struct io_header {
	struct io_context	context;
	struct io_event		*events;
	long				iocount;
	uint32_t			next;
};


int io_setup(int maxevents, io_context_t *ctxp)
{
	struct io_header *hdr;

	if (!ctxp || *ctxp || maxevents < 1) {
		_set_errno(EINVAL);
		return -EINVAL;
	}

	size_t len = sizeof(struct io_header) + sizeof(struct io_event) * maxevents;
	hdr = (struct io_header *)malloc(len);

	if (!hdr) {
		_set_errno(ENOMEM);
		return -ENOMEM;
	}

	memset(hdr, 0, len);
	hdr->events = (struct io_event *)(hdr + 1);
	hdr->context.size = maxevents;
	hdr->context.header_length = (uint32_t)((char *)hdr->events - (char *)&hdr->context);

	*ctxp = &hdr->context;
	return 0;
}


int io_destroy(io_context_t ctx_id)
{
	struct io_header *hdr = (struct io_header *)ctx_id;

	// HACK - io_destroy - cancel / wait for all operations
	// HACK - wait for iocount to drop to zero

	if (!ctx_id) {
		_set_errno(EINVAL);
		return -EINVAL;
	}

	free(hdr);
	return 0;
}


static int wpdk_aio_increment_iocount (struct io_header *hdr)
{
	long count;

	do {
		count = hdr->iocount;
		if (count >= (long)hdr->context.size) {
			_set_errno(EAGAIN);
			return 0;
		}
	} while (InterlockedCompareExchange(&hdr->iocount, count + 1, count) != count);

	return 1;
}


static void wpdk_aio_decrement_iocount (struct io_header *hdr)
{
	InterlockedDecrement(&hdr->iocount);
}


static struct io_task *wpdk_aio_allocate(io_context_t ctx_id, struct iocb *iocb, int iovcnt)
{
	struct io_header *hdr = (struct io_header *)ctx_id;
	struct io_task *task;
	int i;

	task = (struct io_task *)calloc((iovcnt > 1) ? iovcnt + 1 : 1,
				sizeof(struct io_task));

	if (!task || !wpdk_aio_increment_iocount(hdr)) {
		free(task);
		_set_errno(EAGAIN);
		return NULL;
	}

	task->hdr = hdr;
	task->data = iocb->data;
	task->iocb = iocb;

	if (iovcnt > 1) {
		task->iov.active = iovcnt + 1;
		task->iov.iovcnt = iovcnt;

		for (i = 1; i <= iovcnt; i++) {
			task[i].task = task;
			task[i].type = &task[i].iov;
		}
	}

	return task;
}


static void wpdk_aio_free(struct io_task *task)
{
	if (task) {
		wpdk_aio_decrement_iocount(task->hdr);
		free(task);
	}
}


static void wpdk_aio_task_done (struct io_task *task, long long res)
{
	struct io_header *hdr;
	struct io_event *next;

	// HACK - need locking and use barrier before updating tail
	hdr = task->hdr;
	next = &hdr->events[hdr->context.tail];
	next->data = task->data;
	next->obj = task->iocb;
	next->res = res;
	next->res2 = 0;

	hdr->context.tail = (hdr->context.tail + 1) % hdr->context.size;
	wpdk_aio_free(task);
}


static long wpdk_aio_iov_done (struct io_task *task, long count)
{
	long long result = 0;
	int i;

	if (InterlockedAdd(&task->iov.active, -count) == 0) {
		for (i = 1; result >= 0 && i <= task->iov.iovcnt; i++) {
			if (task[i].result < 0) result = task[i].result;
			else result += task[i].result;

			/*
			 * Windows doesn't support scatter/gather I/O with arbitrary
			 * alignment, so readv/writev are implemented with multiple I/Os.
			 * If one of the transfers was shorter than requested, stop
			 * calculating the length and disregard the subsequent I/Os.
			 */

			// HACK - alternative is to issue I/Os serially which is slower
			// and still not posix conformant which stipulates an atomic transfer

			if (task[i].type == &task[i].result) break;
		}

		wpdk_aio_task_done(task, result);
		return 0;
	}

	return task->iov.active;
}


static void wpdk_aio_done (struct io_task *io, long long res)
{
	if (io->type == &io->iov) {
		// Record that the transfer was shorter than requested
		if (res != io->result)
			io->type = &io->result;

		io->result = res;
		wpdk_aio_iov_done(io->task, 1);
		return;
	}

	wpdk_aio_task_done(io, res);
}


static int wpdk_aio_iov_abort (struct io_task *io)
{
	int rc = wpdk_windows_seterrno(GetLastError());
	struct io_task *task = io->task;
	int count, i = (int)(io - task);

	io->result = (-rc);
	count = wpdk_aio_iov_done(task, task->iov.iovcnt - i + 1);

	if (count == 1) {
		wpdk_aio_free(task);
		return (_set_errno(rc), -rc);
	}

	wpdk_aio_iov_done(task, 1);
	return EINTR;
}


static int wpdk_aio_abort (struct io_task *task)
{
	int rc = wpdk_windows_seterrno(GetLastError());

	wpdk_aio_free(task);
	return (_set_errno(rc), -rc);
}


static int wpdk_aio_read (HANDLE h, io_context_t ctx_id, struct iocb *iocb, void *buf, size_t len)
{
	struct io_task *task = wpdk_aio_allocate(ctx_id, iocb, 1);
	LARGE_INTEGER offset;
	DWORD nbytes = 0;

	if (!task) return -EAGAIN;

	offset.QuadPart = iocb->aio_offset;
	task->io.Offset = offset.LowPart;
	task->io.OffsetHigh = offset.HighPart;

	// HACK - check that len < max DWORD size

	if (!len || ReadFile(h, buf, (DWORD)len, &nbytes, &task->io) == TRUE)
		wpdk_aio_done(task, nbytes);

	else if (GetLastError() != ERROR_IO_PENDING)
		return wpdk_aio_abort(task);

	return 0;
}


static int wpdk_aio_write (HANDLE h, io_context_t ctx_id, struct iocb *iocb, void *buf, size_t len)
{
	struct io_task *task = wpdk_aio_allocate(ctx_id, iocb, 1);
	LARGE_INTEGER offset;
	DWORD nbytes = 0;

	if (!task) return -EAGAIN;

	offset.QuadPart = iocb->aio_offset;
	task->io.Offset = offset.LowPart;
	task->io.OffsetHigh = offset.HighPart;

	// HACK - check that len < max DWORD size
		
	if (!len || WriteFile(h, buf, (DWORD)len, &nbytes, &task->io) == TRUE)
		wpdk_aio_done(task, nbytes);

	else if (GetLastError() != ERROR_IO_PENDING)
		return wpdk_aio_abort(task);

	return 0;
}


static int wpdk_aio_readv (HANDLE h, io_context_t ctx_id, struct iocb *iocb, struct iovec *iov, int iovcnt)
{
	struct io_task *task = wpdk_aio_allocate(ctx_id, iocb, iovcnt);
	LARGE_INTEGER offset;
	DWORD nbytes;
	int i;

	if (!task) return -EAGAIN;

	offset.QuadPart = iocb->aio_offset;

	for (i = 0; i < iovcnt; i++) {
		task[i+1].io.Offset = offset.LowPart;
		task[i+1].io.OffsetHigh = offset.HighPart;
		task[i+1].result = iov[i].iov_len;
		nbytes = 0;

		if (!iov[i].iov_len || ReadFile(h, iov[i].iov_base,
					(DWORD)iov[i].iov_len, &nbytes, &task[i+1].io) == TRUE) {
			wpdk_aio_done(&task[i+1], nbytes);

			/*
			 * If the transfer was shorter than requested,
			 * don't start any more I/Os.
			 */
			if (nbytes != iov[i].iov_len) {
				wpdk_aio_iov_done(task, iovcnt - i);
				return 0;
			}
		}

		else if (GetLastError() != ERROR_IO_PENDING)
			return wpdk_aio_iov_abort(&task[i+1]);

		offset.QuadPart += iov[i].iov_len;
	}

	wpdk_aio_iov_done(task, 1);
	return 0;
}


static int wpdk_aio_writev (HANDLE h, io_context_t ctx_id, struct iocb *iocb, struct iovec *iov, int iovcnt)
{
	struct io_task *task = wpdk_aio_allocate(ctx_id, iocb, iovcnt);
	LARGE_INTEGER offset;
	DWORD nbytes;
	int i;

	if (!task) return -EAGAIN;

	offset.QuadPart = iocb->aio_offset;

	for (i = 0; i < iovcnt; i++) {
		task[i+1].io.Offset = offset.LowPart;
		task[i+1].io.OffsetHigh = offset.HighPart;
		task[i+1].result = iov[i].iov_len;
		nbytes = 0;	

		if (!iov[i].iov_len || WriteFile(h, iov[i].iov_base,
					(DWORD)iov[i].iov_len, &nbytes, &task[i+1].io) == TRUE) {
			wpdk_aio_done(&task[i+1], nbytes);

			/*
			 * If the transfer was shorter than requested,
			 * don't start any more I/Os.
			 */
			if (nbytes != iov[i].iov_len) {
				wpdk_aio_iov_done(task, iovcnt - i);
				return 0;
			}
		}

		else if (GetLastError() != ERROR_IO_PENDING)
			return wpdk_aio_iov_abort(&task[i+1]);

		offset.QuadPart += iov[i].iov_len;
	}

	wpdk_aio_iov_done(task, 1);
	return 0;
}


static int wpdk_aio_validate_iovec (struct iovec *iov, size_t iovcnt)
{
	ssize_t length = 0;
	int i;

	// HACK - unsigned / signed issue
	if (iovcnt < 1) {
		_set_errno(EINVAL);
		return 0;
	}

	// HACK - validate should check size_t is in range
	// HACK - is there system imposed maximum size?

	for (i = 0; i < (int)iovcnt; i++) {
		if (iov[i].iov_len > (size_t)(SSIZE_MAX - length)) {
			_set_errno(EINVAL);
			return 0;
		}
		length += iov[i].iov_len;
	}

	return 1;
}


// HACK - io_submit is synchronous at the moment
int io_submit(io_context_t ctx_id, long nr, struct iocb *ios[])
{
	struct iovec *iov;
	struct iocb *iocb;
	HANDLE h;
	int rc;
	long i;

	if (!ios) {
		_set_errno(EINVAL);
		return -EINVAL;
	}

	for (i = 0; i < nr; i++) {
		iocb = ios[i];

		if (!iocb) {
			_set_errno(EINVAL);
			return (i) ? i : -EINVAL;
		}

		h = (HANDLE)_get_osfhandle(iocb->aio_fildes);

		if (h == INVALID_HANDLE_VALUE) {
			_set_errno(EINVAL);
			return (i) ? i : -EINVAL;
		}

		switch (iocb->aio_lio_opcode) {
			case IO_CMD_PREAD:
				rc = wpdk_aio_read(h, ctx_id, iocb, iocb->aio_buf, iocb->aio_nbytes);
				break;

			case IO_CMD_PWRITE:
				rc = wpdk_aio_write(h, ctx_id, iocb, iocb->aio_buf, iocb->aio_nbytes);
				break;

			case IO_CMD_PREADV:
				iov = (struct iovec *)iocb->aio_buf;

				if (iocb->aio_nbytes == 1) {
					// HACK - test out the iovec logic
					struct iocb io = *iocb;
					struct iocb *iovs = &io;
					struct iovec v[4];

					v[0].iov_base = iov->iov_base;
					v[0].iov_len = iov->iov_len / 4;
					v[1].iov_base = (char *)v[0].iov_base + v[0].iov_len;
					v[1].iov_len = iov->iov_len / 4;
					v[2].iov_base = (char *)v[1].iov_base + v[1].iov_len;
					v[2].iov_len = iov->iov_len / 4;
					v[3].iov_base = (char *)v[2].iov_base + v[2].iov_len;
					v[3].iov_len = iov->iov_len - (iov->iov_len / 4) * 3;

					io.aio_buf = v;
					io.aio_nbytes = 4;
					rc = io_submit(ctx_id, 1, &iovs);
					break;
				}

				if (!wpdk_aio_validate_iovec(iov, iocb->aio_nbytes))
					rc = -EINVAL;
				else if (iocb->aio_nbytes == 1)
					rc = wpdk_aio_read(h, ctx_id, iocb, iov->iov_base, iov->iov_len);
				else
					rc = wpdk_aio_readv(h, ctx_id, iocb, iov, (int)iocb->aio_nbytes);
				break;

			case IO_CMD_PWRITEV:
				iov = (struct iovec *)iocb->aio_buf;

				if (iocb->aio_nbytes == 1) {
					// HACK - test out the iovec logic
					struct iocb io = *iocb;
					struct iocb *iovs = &io;
					struct iovec v[4];

					v[0].iov_base = iov->iov_base;
					v[0].iov_len = iov->iov_len / 4;
					v[1].iov_base = (char *)v[0].iov_base + v[0].iov_len;
					v[1].iov_len = iov->iov_len / 4;
					v[2].iov_base = (char *)v[1].iov_base + v[1].iov_len;
					v[2].iov_len = iov->iov_len / 4;
					v[3].iov_base = (char *)v[2].iov_base + v[2].iov_len;
					v[3].iov_len = iov->iov_len - (iov->iov_len / 4) * 3;

					io.aio_buf = v;
					io.aio_nbytes = 4;
					rc = io_submit(ctx_id, 1, &iovs);
					break;
				}

				if (!wpdk_aio_validate_iovec(iov, iocb->aio_nbytes))
					rc = -EINVAL;
				else if (iocb->aio_nbytes == 1)
					rc = wpdk_aio_write(h, ctx_id, iocb, iov->iov_base, iov->iov_len);
				else
					rc = wpdk_aio_writev(h, ctx_id, iocb, iov, (int)iocb->aio_nbytes);
				break;

			default:
				_set_errno(EINVAL);
				rc = -EINVAL;
				break;
		}

		/*
		 * If a readv/writev has failed, but is still partially active, then
		 * report it as started and stop processing the rest of the iocbs.
		 */
		if (rc == EINTR)
			return i + 1;

		if (rc != 0)
			return (i) ? i : rc;
	}

	return i;
}


int io_cancel(io_context_t ctx_id, struct iocb *iocb, struct io_event *evt)
{
	// HACK - not implemented
	UNREFERENCED_PARAMETER(ctx_id);
	UNREFERENCED_PARAMETER(iocb);
	UNREFERENCED_PARAMETER(evt);
	return -EINVAL;
}


int io_getevents(io_context_t ctx_id, long min_nr, long nr, struct io_event *events, struct timespec *timeout)
{
	struct io_header *hdr = (struct io_header *)ctx_id;
	struct io_event *ctx_events;
	int i;

	UNREFERENCED_PARAMETER(min_nr);
	UNREFERENCED_PARAMETER(timeout);

	if (!ctx_id) {
		_set_errno(EINVAL);
		return -EINVAL;
	}

	ctx_events = hdr->events;

	for (i = 0; i < nr; i++) {
		if (ctx_id->head == ctx_id->tail) break;
		events[i] = ctx_events[ctx_id->head];
		ctx_id->head = (ctx_id->head + 1) % ctx_id->size;
	}

	// HACK - no synchronisation - need barriers
	// HACK - io_getevents not implemented
	return i;
}


// HACK - more extensive list
int wpdk_windows_seterrno (DWORD err)
{
	int error = EINVAL;

	UNREFERENCED_PARAMETER(err);

	_set_errno(error);
	return error;
}


int wpdk_windows_error ()
{
	wpdk_windows_seterrno(GetLastError());
	return -1;
}
