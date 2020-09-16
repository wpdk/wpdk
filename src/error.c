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
 *  Details about possible windows error codes are drawn from Microsoft
 *  documentation at
 *  https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes
 */

#include <wpdk/internal.h>
#include <sys/socket.h>
#include <errno.h>


// HACK - add non WSA errors
int
wpdk_convert_to_posix(int err)
{
	switch (err) {
		case ERROR_SUCCESS:
			return 0;

		case ERROR_FILE_NOT_FOUND:
			/* The system cannot find the file specified */
			return ENOENT;

		case WSAEINTR:
			/* A blocking operation was interrupted by a call to WSACancelBlockingCall */
			return EINTR;

		case WSAEBADF:
			/* The file handle supplied is not valid */
			return EBADF;

		case WSAEACCES:
			/* An attempt was made to access a socket in a way forbidden by its access permissions */
			return EACCES;

		case WSAEFAULT:
			/* The system detected an invalid address in a pointer argument in a call */
			return EFAULT;

		case WSAEINVAL:
			/* An invalid argument was supplied */
			return EINVAL;

		case WSAEMFILE:
			/* Too many open sockets */
			return EMFILE;

		case WSAEWOULDBLOCK:
			/* A non-blocking socket operation could not be completed immediately */
			return EWOULDBLOCK;

		case WSAEINPROGRESS:
			/* A blocking operation is currently executing */
			return EINPROGRESS;

		case WSAEALREADY:
			/* operation on a non-blocking socket that already had an operation in progress */
			return EALREADY;

		case WSAENOTSOCK:
			/* An operation was attempted on something that is not a socket */
			return ENOTSOCK;

		case WSAEDESTADDRREQ:
			/* A required address was omitted from an operation on a socket */
			return EDESTADDRREQ;

		case WSAEMSGSIZE:
			/* A message sent on a datagram socket was larger than the internal message buffer */
			return EMSGSIZE;

		case WSAEPROTOTYPE:
			/* A protocol was specified that does not support the socket type requested */
			return EPROTOTYPE;

		case WSAENOPROTOOPT:
			/* An incorrect option or level was specified in a getsockopt or setsockopt call */
			return ENOPROTOOPT;

		case WSAEPROTONOSUPPORT:
			/* The protocol has not been configured into the system */
			return EPROTONOSUPPORT;

		case WSAESOCKTNOSUPPORT:
			/* The specified socket type does not exist in this address family */
			return EAFNOSUPPORT;

		case WSAEOPNOTSUPP:
			/* The attempted operation is not supported for the type of object referenced */
			return ENOTSUP;

		case WSAEPFNOSUPPORT:
			/* The protocol family has not been configured into the system */
			return EPROTONOSUPPORT;

		case WSAEAFNOSUPPORT:
			/* An address incompatible with the requested protocol was used */
			return EAFNOSUPPORT;

		case WSAEADDRINUSE:
			/* Only one usage of each socket address (protocol/network address/port) */
			return EADDRINUSE;

		case WSAEADDRNOTAVAIL:
			/* The requested address is not valid in its context */
			return EADDRNOTAVAIL;

		case WSAENETDOWN:
			/* A socket operation encountered a dead network */
			return ENETDOWN;

		case WSAENETUNREACH:
			/* A socket operation was attempted to an unreachable network */
			return ENETUNREACH;

		case WSAENETRESET:
			/* The connection has been broken due to keep-alive detecting a failure */
			return ENETRESET;

		case WSAECONNABORTED:
			/* An established connection was aborted by the software in your host machine */
			return ECONNABORTED;

		case WSAECONNRESET:
			/* An existing connection was forcibly closed by the remote host */
			return ECONNRESET;

		case WSAENOBUFS:
			/* The system lacked sufficient buffer space or a queue was full */
			return ENOBUFS;

		case WSAEISCONN:
			/* A connect request was made on an already connected socket */
			return EISCONN;

		case WSAENOTCONN:
			/* The socket is not connected and no address was supplied */
			return ENOTCONN;

		case WSAESHUTDOWN:
			/* The socket has been shut down with a previous shutdown call */
			return ESHUTDOWN;

		case WSAETOOMANYREFS:
			/* Too many references to some kernel object */
			return ETOOMANYREFS;

		case WSAETIMEDOUT:
			/* Connection attempt failed because the connected party failed to respond */
			return ETIMEDOUT;

		case WSAECONNREFUSED:
			/* No connection could be made because the target machine actively refused it */
			return ECONNREFUSED;

		case WSAELOOP:
			/* Cannot translate name */
			return ELOOP;

		case WSAENAMETOOLONG:
			/* Name component or name was too long */
			return ENAMETOOLONG;

		case WSAEHOSTDOWN:
			/* A socket operation failed because the destination host was down */
			return EHOSTDOWN;

		case WSAEHOSTUNREACH:
			/* A socket operation was attempted to an unreachable host */
			return EHOSTUNREACH;

		case WSAENOTEMPTY:
			/* Cannot remove a directory that is not empty */
			return ENOTEMPTY;

		case WSAEPROCLIM:
			/* Limit on the number of applications using sockets simultaneously */
			return EPROCLIM;

		case WSAEUSERS:
			/* Ran out of quota */
			return EUSERS;

		case WSAEDQUOT:
			/* Ran out of disk quota */
			return EDQUOT;

		case WSAESTALE:
			/* File handle reference is no longer available */
			return ESTALE;

		case WSAEREMOTE:
			/* Item is not available locally */
			return EREMOTE;

		case WSASYSNOTREADY:
			/* Underlying system providing network services is currently unavailable */
			return EAGAIN;

		case WSAVERNOTSUPPORTED:
			/* The Windows Sockets version requested is not supported */
			return ENOSYS;

		case WSANOTINITIALISED:
			/* Application has not called WSAStartup, or WSAStartup failed */
			return ENOSYS;

		case WSAEDISCON:
			/* Indicates the remote party has initiated a graceful shutdown sequence */
			return ECONNRESET;

		case WSAENOMORE:
			/* No more results can be returned by WSALookupServiceNext */
			return ENOSYS;

		case WSAECANCELLED:
			/* The call has been canceled */
			return ECANCELED;

		case WSAEINVALIDPROCTABLE:
			/* The procedure call table is invalid */
			return EINVAL;

		case WSAEINVALIDPROVIDER:
			/* The requested service provider is invalid */
			return EINVAL;

		case WSAEPROVIDERFAILEDINIT:
			/* The requested service provider could not be loaded or initialized */
			return ENOSYS;

		case WSASYSCALLFAILURE:
			/* A system call has failed */
			return ENOSYS;

		case WSASERVICE_NOT_FOUND:
			/* No such service is known */
			return ENOSYS;

		case WSATYPE_NOT_FOUND:
			/* The specified class was not found */
			return ENOSYS;

		case WSA_E_NO_MORE:
			/* No more results can be returned by WSALookupServiceNext */
			return EINVAL;

		case WSA_E_CANCELLED:
			/* WSALookupServiceEnd was called while this call was still processing */
			return ECANCELED;

		case WSAEREFUSED:
			/* A database query failed because it was actively refused */
			return EINVAL;

		case WSAHOST_NOT_FOUND:
			/* No such host is known */
			return EHOSTDOWN;

		case WSATRY_AGAIN:
			/* The local server did not receive a response from an authoritative server */
			return EAGAIN;

		case WSANO_RECOVERY:
			/* A non-recoverable error occurred during a database lookup */
			return ENOSYS;

		case WSANO_DATA:
			/* The requested name is valid, but no data of the requested type was found */
			return ENOSYS;

		case WSA_SECURE_HOST_NOT_FOUND:
			/* No such host is known securely */
			return EHOSTDOWN;

		case WSA_IPSEC_NAME_POLICY_ERROR:
			/* Name based IPSEC policy could not be added */
			return ENOSYS;

		case WSA_NOT_ENOUGH_MEMORY:
			/* Not enough memory resources are available */
			return ENOMEM;
	}

	// Default to EINVAL as a generic failure
	return EINVAL;
}


int
wpdk_last_error()
{
	return wpdk_windows_error(GetLastError());
}


int
wpdk_last_wsa_error()
{
	return wpdk_windows_error(WSAGetLastError());
}


int
wpdk_windows_error(int error)
{
	_set_errno(wpdk_convert_to_posix(error));
	return (-1);
}


int
wpdk_posix_error(int error)
{
	_set_errno(error);
	return (-1);
}
