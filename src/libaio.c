#include <wpdklib.h>
#include <stdlib.h>
#include <unistd.h>
#include <libaio.h>

/*
 *  There seems to be a lack of definitive documentation about libaio.
 *  The implementation has been based on details found at:
 * 
 *  https://github.com/littledan/linux-aio
 *  https://blog.cloudflare.com/io_submit-the-epoll-alternative-youve-never-heard-about/
 *  https://www.fsl.cs.sunysb.edu/~vass/linux-aio.txt
 */


int io_setup(int maxevents, io_context_t *ctxp)
{
	io_context_t ctx_id;

	if (!ctxp || *ctxp || maxevents < 1) {
		_set_errno(EINVAL);
		return -1;
	}

	size_t len = sizeof(io_context_t) + sizeof(struct io_event) * maxevents;
	ctx_id = (io_context_t)malloc(len);

	if (!ctx_id) {
		_set_errno(ENOMEM);
		return -1;
	}

	memset(ctx_id, 0, len);
	ctx_id->size = maxevents;
	ctx_id->header_length = sizeof(io_context_t);

	*ctxp = ctx_id;
	return 0;
}


int io_destroy(io_context_t ctx_id)
{
	// HACK - io_destroy - cancel / wait for all operations

	if (!ctx_id) {
		_set_errno(EINVAL);
		return -1;
	}

	free(ctx_id);
	return 0;
}


int wpdk_aio_reserve(io_context_t ctx_id)
{
	if ((ctx_id->tail + 1) % ctx_id->size == ctx_id->head)
		return 0;
	
	// HACK - needs to reserve in some way?
//	InterlockedIncrement(&ctx_id->iocount);
	return 1;
}


void wpdk_aio_release(io_context_t ctx_id)
{
	// HACK - no need to release until reserve done
	// InterlockedDecrement(&ctx_id->iocount);
}


static void wpdk_aio_complete(io_context_t ctx_id, void *data, struct iocb *obj, long long res)
{
	struct io_event *events = (struct io_event *)(ctx_id + 1);
	struct io_event *next = &events[ctx_id->tail];

	// HACK - need to serialise ?
	next->data = data;
	next->obj = obj;
	next->res = res;
	next->res2 = 0;

	ctx_id->tail = (ctx_id->tail + 1) % ctx_id->size;
}


static int wpdk_aio_read (HANDLE h, io_context_t ctx_id, struct iocb *iocb, void *buf, size_t len)
{
	LARGE_INTEGER offset;
	OVERLAPPED io = {0};	
	DWORD nbytes;

	offset.QuadPart = iocb->aio_offset;
	io.Offset = offset.LowPart;
	io.OffsetHigh = offset.HighPart;
	
	if (ReadFile(h, buf, len, &nbytes, &io) == FALSE)
		return -(wpdk_windows_seterrno(GetLastError()));

	wpdk_aio_complete(ctx_id, iocb->data, iocb, nbytes);
	return 0;
}


static int wpdk_aio_write (HANDLE h, io_context_t ctx_id, struct iocb *iocb, void *buf, size_t len)
{
	LARGE_INTEGER offset;
	OVERLAPPED io = {0};
	DWORD nbytes;

	offset.QuadPart = iocb->aio_offset;
	io.Offset = offset.LowPart;
	io.OffsetHigh = offset.HighPart;
	
	if (WriteFile(h, buf, len, &nbytes, &io) == FALSE)
		return -(wpdk_windows_seterrno(GetLastError()));

	wpdk_aio_complete(ctx_id, iocb->data, iocb, nbytes);
	return 0;
}


static int wpdk_aio_readv (HANDLE h, io_context_t ctx_id, struct iocb *iocb, struct iovec *iov, int iovcnt)
{
	LARGE_INTEGER offset;
	OVERLAPPED io = {0};	
	DWORD nbytes;
	int i;

	if (iovcnt == 1)
		return wpdk_aio_read(h, ctx_id, iocb, iov->iov_base, iov->iov_len);

	offset.QuadPart = iocb->aio_offset;

	for (i = 0; i < iovcnt; i++) {
		io.Offset = offset.LowPart;
		io.OffsetHigh = offset.HighPart;

		if (ReadFile(h, iov[i].iov_base, (DWORD)iov[i].iov_len, &nbytes, &io) == FALSE)
			return -(wpdk_windows_seterrno(GetLastError()));

		offset.QuadPart += nbytes;
		if (nbytes != iov[i].iov_len) break;
	}

	wpdk_aio_complete(ctx_id, iocb->data, iocb, offset.QuadPart - iocb->aio_offset);
	return 0;
}


static int wpdk_aio_writev (HANDLE h, io_context_t ctx_id, struct iocb *iocb, struct iovec *iov, int iovcnt)
{
	LARGE_INTEGER offset;
	OVERLAPPED io = {0};	
	DWORD nbytes;
	int i;

	if (iovcnt == 1)
		return wpdk_aio_write(h, ctx_id, iocb, iov->iov_base, iov->iov_len);
	
	offset.QuadPart = iocb->aio_offset;

	for (i = 0; i < iovcnt; i++) {
		io.Offset = offset.LowPart;
		io.OffsetHigh = offset.HighPart;

		if (WriteFile(h, iov[i].iov_base, (DWORD)iov[i].iov_len, &nbytes, &io) == FALSE)
			return -(wpdk_windows_seterrno(GetLastError()));

		offset.QuadPart += nbytes;
		if (nbytes != iov[i].iov_len) break;
	}

	wpdk_aio_complete(ctx_id, iocb->data, iocb, offset.QuadPart - iocb->aio_offset);
	return 0;
}


// HACK - io_submit is synchronous at the moment
int io_submit(io_context_t ctx_id, long nr, struct iocb *ios[])
{
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
		h = (HANDLE)_get_osfhandle(iocb->aio_fildes);

		if (h == INVALID_HANDLE_VALUE) {
			_set_errno(EINVAL);
			return (i) ? i : -EINVAL;
		}

		if (!wpdk_aio_reserve(ctx_id)) {
			_set_errno(EAGAIN);
			return (i) ? i : -EAGAIN;
		}

		switch (iocb->aio_lio_opcode) {
			case IO_CMD_PREAD:
				rc = wpdk_aio_read(h, ctx_id, iocb, iocb->aio_buf, iocb->aio_nbytes);
				break;

			case IO_CMD_PWRITE:
				rc = wpdk_aio_write(h, ctx_id, iocb, iocb->aio_buf, iocb->aio_nbytes);
				break;

			case IO_CMD_PREADV:
				rc = wpdk_aio_readv(h, ctx_id, iocb, (struct iovec *)iocb->aio_buf, (int)iocb->aio_nbytes);
				break;

			case IO_CMD_PWRITEV:
				rc = wpdk_aio_writev(h, ctx_id, iocb, (struct iovec *)iocb->aio_buf, (int)iocb->aio_nbytes);
				break;

			default:
				_set_errno(EINVAL);
				rc = -EINVAL;
				break;
		}

		if (rc != 0) {
			wpdk_aio_release(ctx_id);
			return (i) ? i : rc;
		}
	}

	return 0;
}


int io_cancel(io_context_t ctx_id, struct iocb *iocb, struct io_event *evt)
{
	// HACK - not implemented
	return -1;
}


int io_getevents(io_context_t ctx_id, long min_nr, long nr, struct io_event *events, struct timespec *timeout)
{
	struct io_event *ctx_events = (struct io_event *)(ctx_id + 1);
	int i;

	if (!ctx_id) {
		_set_errno(EINVAL);
		return -EINVAL;
	}

	for (i = 0; i < nr; i++) {
		if (ctx_id->head == ctx_id->tail) break;
		events[i] = ctx_events[ctx_id->head];
		ctx_id->head = (ctx_id->head + 1) % ctx_id->size;
	}

	// HACK - no synchronisation - need barriers
	// HACK - io_getevents not implemented
	return i;
}


int wpdk_windows_error ()
{
	wpdk_socket_seterrno(GetLastError());
	return -1;
}


// HACK - more extensive list
int wpdk_windows_seterrno (DWORD err)
{
	int error = EINVAL;

	_set_errno(error);
	return error;
}
