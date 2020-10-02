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


int
wpdk_convert_to_posix(int err)
{
	switch (err) {
		case ERROR_SUCCESS:
			return 0;

		case ERROR_ACCESS_DENIED:
			/* Access is denied */
			return EACCES;

		case ERROR_ADAP_HDW_ERR:
			/* A network adapter hardware error occurred */
			return EIO;

		case ERROR_ALERTED:
			/* Alerted */
			return EINTR;

		case ERROR_ALREADY_EXISTS:
			/* Cannot create a file when that file already exists */
			return EEXIST;

		case ERROR_ARENA_TRASHED:
			/* The storage control blocks were destroyed */
			return EFAULT;

		case ERROR_BAD_ARGUMENTS:
			/* One or more arguments are not correct */
			return EINVAL;

		case ERROR_BAD_COMMAND:
			/* The device does not recognize the command */
			return EIO;

		case ERROR_BAD_DEVICE:
			/* The specified device name is invalid */
			return ENXIO;

#ifndef ERROR_BAD_DEVICE_PATH
#define ERROR_BAD_DEVICE_PATH 330L
#endif
		case ERROR_BAD_DEVICE_PATH:
			/* An attempt was made to send down the command via an invalid path to the target device */
			return EINVAL;

		case ERROR_BAD_DRIVER_LEVEL:
			/* The system does not support the command requested */
			return ENOSYS;

		case ERROR_BAD_ENVIRONMENT:
			/* The environment is incorrect */
			return EFAULT;

		case ERROR_BAD_EXE_FORMAT:
			/* Program is not a valid Win32 application */
			return ENOEXEC;

		case ERROR_BAD_FORMAT:
			/* An attempt was made to load a program with an incorrect format */
			return ENOEXEC;

		case ERROR_BAD_LENGTH:
			/* The program issued a command but the command length is incorrect */
			return EIO;

		case ERROR_BAD_NETPATH:
			/* The network path was not found */
			return ENOENT;

		case ERROR_BAD_NET_NAME:
			/* The network name cannot be found */
			return ENOENT;

		case ERROR_BAD_NET_RESP:
			/* The specified server cannot perform the requested operation */
			return ENOTSUP;

		case ERROR_BAD_PATHNAME:
			/* The specified path is invalid */
			return ENOENT;

		case ERROR_BAD_UNIT:
			/* The system cannot find the device specified */
			return ENODEV;

		case ERROR_BROKEN_PIPE:
			/* The pipe has been ended */
			return EPIPE;

		case ERROR_BUFFER_OVERFLOW:
			/* The file name is too long */
			return ENAMETOOLONG;

		case ERROR_BUS_RESET:
			/* The I/O bus was reset */
			return EIO;

		case ERROR_BUSY:
			/* The requested resource is in use */
			return EBUSY;

		case ERROR_CALL_NOT_IMPLEMENTED:
			/* This function is not supported on this system */
			return ENOSYS;

		case ERROR_CAN_NOT_COMPLETE:
			/* Cannot complete this function */
			return EINVAL;

		case ERROR_CANCELLED:
			/* The operation was canceled by the user */
			return ECANCELED;

		case ERROR_CANNOT_MAKE:
			/* The directory or file cannot be created */
			return EPERM;

#ifndef ERROR_CANT_TERMINATE_SELF
#define ERROR_CANT_TERMINATE_SELF 555L
#endif
		case ERROR_CANT_TERMINATE_SELF:
			/* The last thread in a process attempted to terminate itself */
			return EPERM;

#ifndef ERROR_CANT_WAIT
#define ERROR_CANT_WAIT 554L
#endif
		case ERROR_CANT_WAIT:
			/* An operation cannot continue without blocking for I/O */
			return EWOULDBLOCK;

		case ERROR_COMMITMENT_LIMIT:
			/* The paging file is too small for this operation to complete */
			return ENOMEM;

		case ERROR_CONNECTION_ABORTED:
			/* The network connection was aborted by the local system */
			return ECONNABORTED;

		case ERROR_CONNECTION_ACTIVE:
			/* An invalid operation was attempted on an active network connection */
			return EISCONN;

		case ERROR_CONNECTION_REFUSED:
			/* The remote computer refused the network connection */
			return ECONNREFUSED;

		case ERROR_CRC:
			/* Data error (cyclic redundancy check) */
			return EIO;

		case ERROR_CURRENT_DIRECTORY:
			/* The directory cannot be removed */
			return EPERM;

#ifndef ERROR_DATA_CHECKSUM_ERROR
#define ERROR_DATA_CHECKSUM_ERROR 323L
#endif
		case ERROR_DATA_CHECKSUM_ERROR:
			/* A data integrity checksum error occurred. Data in the file stream is corrupt */
			return EIO;

		case ERROR_DELETE_PENDING:
			/* The file cannot be opened because it is in the process of being deleted */
			return EBUSY;

		case ERROR_DEV_NOT_EXIST:
			/* The specified network resource or device is no longer available */
			return ENOENT;

#ifndef ERROR_DEVICE_ALREADY_ATTACHED
#define ERROR_DEVICE_ALREADY_ATTACHED 548L
#endif
		case ERROR_DEVICE_ALREADY_ATTACHED:
			/* An attempt was made to attach to a device that was already attached */
			return EBUSY;

#ifndef ERROR_DEVICE_NO_RESOURCES
#define ERROR_DEVICE_NO_RESOURCES 322L
#endif
		case ERROR_DEVICE_NO_RESOURCES:
			/* The target device has insufficient resources to complete the operation */
			return ENOMEM;

		case ERROR_DEVICE_NOT_CONNECTED:
			/* The device is not connected */
			return ENXIO;

#ifndef ERROR_DEVICE_UNREACHABLE
#define ERROR_DEVICE_UNREACHABLE 321L
#endif
		case ERROR_DEVICE_UNREACHABLE:
			/* The device is unreachable */
			return ENXIO;

		case ERROR_DIR_NOT_EMPTY:
			/* The directory is not empty */
			return ENOTEMPTY;

		case ERROR_DIR_NOT_ROOT:
			/* The directory is not a subdirectory of the root directory */
			return EINVAL;

		case ERROR_DIRECT_ACCESS_HANDLE:
			/* Attempt to use a file handle to an open disk partition for an operation other than raw disk I/O */
			return EIO;

#ifndef ERROR_DIRECTORY_NOT_SUPPORTED
#define ERROR_DIRECTORY_NOT_SUPPORTED 336L
#endif
		case ERROR_DIRECTORY_NOT_SUPPORTED:
			/* An operation is not supported on a directory */
			return EPERM;

		case ERROR_DISK_FULL:
			/* There is not enough space on the disk */
			return ENOSPC;

		case ERROR_DUP_NAME:
			/* Not connected because a duplicate name exists on the network */
			return EADDRINUSE;

#ifndef ERROR_ELEVATION_REQUIRED
#define ERROR_ELEVATION_REQUIRED 740L
#endif
		case ERROR_ELEVATION_REQUIRED:
			/* The requested operation requires elevation */
			return EPERM;

#ifndef ERROR_ERRORS_ENCOUNTERED
#define ERROR_ERRORS_ENCOUNTERED 774L
#endif
		case ERROR_ERRORS_ENCOUNTERED:
			/* One or more errors occurred while processing the request */
			return EINVAL;

		case ERROR_EXCL_SEM_ALREADY_OWNED:
			/* The exclusive semaphore is owned by another process */
			return EBUSY;

		case ERROR_FILE_EXISTS:
			/* The file exists */
			return EEXIST;

#ifndef ERROR_FILE_LEVEL_TRIM_NOT_SUPPORTED
#define ERROR_FILE_LEVEL_TRIM_NOT_SUPPORTED 326L
#endif
		case ERROR_FILE_LEVEL_TRIM_NOT_SUPPORTED:
			/* Device does not support file-level TRIM */
			return ENOSYS;

		case ERROR_FILE_NOT_FOUND:
			/* The system cannot find the file specified */
			return ENOENT;

		case ERROR_FILENAME_EXCED_RANGE:
			/* The filename or extension is too long */
			return ENAMETOOLONG;

		case ERROR_GEN_FAILURE:
			/* A device attached to the system is not functioning */
			return EIO;

		case ERROR_GRACEFUL_DISCONNECT:
			/* The network connection was gracefully closed */
			return ESHUTDOWN;

		case ERROR_INCORRECT_SIZE:
			/* Incorrect size argument */
			return EINVAL;

		case ERROR_INVALID_ACCESS:
			/* The access code is invalid */
			return EACCES;

		case ERROR_INVALID_BLOCK:
			/* The storage control block address is invalid */
			return EFAULT;

		case ERROR_INVALID_FUNCTION:
			/* Incorrect function */
			return EINVAL;

		case ERROR_INVALID_HANDLE:
			/* The handle is invalid */
			return EBADF;

#ifndef ERROR_INVALID_LOCK_RANGE
#define ERROR_INVALID_LOCK_RANGE 307L
#endif
		case ERROR_INVALID_LOCK_RANGE:
			/* A requested file lock operation cannot be processed due to an invalid byte range */
			return EINVAL;

		case ERROR_INVALID_NAME:
			/* The filename, directory name, or volume label syntax is incorrect */
			return ENOENT;

		case ERROR_INVALID_USER_BUFFER:
			/* The supplied user buffer is not valid for the requested operation */
			return EINVAL;

		case ERROR_HANDLE_DISK_FULL:
			/* The disk is full */
			return ENOSPC;

		case ERROR_HANDLE_EOF:
			/* Reached the end of the file */
			return ENODATA;

		case ERROR_HOST_DOWN:
			/* The remote system is not available */
			return EHOSTUNREACH;

		case ERROR_HOST_UNREACHABLE:
			/* The network location cannot be reached */
			return EHOSTUNREACH;

		case ERROR_INSUFFICIENT_BUFFER:
			/* The data area passed to a system call is too small */
			return EINVAL;

		case ERROR_INTERNAL_ERROR:
			/* An internal error occurred */
			return EFAULT;

		case ERROR_INVALID_ADDRESS:
			/* Attempt to access invalid address */
			return EFAULT;

		case ERROR_INVALID_DATA:
			/* The data is invalid */
			return EINVAL;

		case ERROR_INVALID_DRIVE:
			/* The system cannot find the drive specified */
			return ENODEV;

#ifndef ERROR_INVALID_EXCEPTION_HANDLER
#define ERROR_INVALID_EXCEPTION_HANDLER 310L
#endif
		case ERROR_INVALID_EXCEPTION_HANDLER:
			/* An invalid exception handler routine has been detected */
			return EFAULT;

#ifndef ERROR_INVALID_FIELD_IN_PARAMETER_LIST
#define ERROR_INVALID_FIELD_IN_PARAMETER_LIST 328L
#endif
		case ERROR_INVALID_FIELD_IN_PARAMETER_LIST:
			/* The command specified an invalid field in its parameter list */
			return EINVAL;

		case ERROR_INVALID_FLAGS:
			/* Invalid flags */
			return EINVAL;

		case ERROR_INVALID_NETNAME:
			/* The format of the specified network name is invalid */
			return EADDRNOTAVAIL;

		case ERROR_INVALID_PARAMETER:
			/* The parameter is incorrect */
			return EINVAL;

		case ERROR_IO_DEVICE:
			/* The request could not be performed because of an I/O device error */
			return EIO;

		case ERROR_IO_INCOMPLETE:
			/* Overlapped I/O event is not in a signaled state */
			return EINPROGRESS;

		case ERROR_IO_PENDING:
			/* Overlapped I/O operation is in progress */
			return EINPROGRESS;

		case ERROR_LOCK_FAILED:
			/* Unable to lock a region of a file */
			return EBUSY;

		case ERROR_LOCK_VIOLATION:
			/* The process cannot access the file because another process has locked a portion of the file */
			return EACCES;

#ifndef ERROR_LOST_WRITEBEHIND_DATA
#define ERROR_LOST_WRITEBEHIND_DATA 596L
#endif
		case ERROR_LOST_WRITEBEHIND_DATA:
			/* Windows was unable to save all the data for the file */
			return EIO;

		case ERROR_MAX_THRDS_REACHED:
			/* No more threads can be created in the system */
			return ENOMEM;

#ifndef ERROR_MEMORY_HARDWARE
#define ERROR_MEMORY_HARDWARE 779L
#endif
		case ERROR_MEMORY_HARDWARE:
			/* The hardware has reported an uncorrectable memory error */
			return EFAULT;

		case ERROR_MORE_DATA:
			/* More data is available */
			return EMSGSIZE;

#ifndef ERROR_MULTIPLE_FAULT_VIOLATION
#define ERROR_MULTIPLE_FAULT_VIOLATION 640L
#endif
		case ERROR_MULTIPLE_FAULT_VIOLATION:
			/* Multiple fault violation */
			return EFAULT;

		case ERROR_NEGATIVE_SEEK:
			/* An attempt was made to move the file pointer before the beginning of the file */
			return EINVAL;

		case ERROR_NETNAME_DELETED:
			/* The specified network name is no longer available */
			return ECONNABORTED;

		case ERROR_NETWORK_ACCESS_DENIED:
			/* Network access is denied */
			return EACCES;

		case ERROR_NETWORK_UNREACHABLE:
			/* The network location cannot be reached */
			return ENETUNREACH;

		case ERROR_NETWORK_BUSY:
			/* The network is busy */
			return EAGAIN;

		case ERROR_NO_MORE_ITEMS:
			/* No more data is available */
			return ENODATA;

		case ERROR_NOACCESS:
			/* Invalid access to memory location */
			return EFAULT;

#ifndef ERROR_NOINTERFACE
#define ERROR_NOINTERFACE 632L
#endif
		case ERROR_NOINTERFACE:
			/* The requested interface is not supported */
			return ENOSYS;

#ifndef ERROR_NOT_CAPABLE
#define ERROR_NOT_CAPABLE 775L
#endif
		case ERROR_NOT_CAPABLE:
			/* The implementation is not capable of performing the request */
			return ENOSYS;

		case ERROR_NOT_ENOUGH_MEMORY:
			/* Not enough memory resources are available to process this command */
			return ENOMEM;

		case ERROR_NOT_ENOUGH_QUOTA:
			/* Not enough quota is available to process this command */
			return ENOMEM;

		case ERROR_NOT_FOUND:
			/* Element not found */
			return ENOENT;

		case ERROR_NOT_LOCKED:
			/* The segment is already unlocked */
			return EINVAL;

		case ERROR_NOT_READY:
			/* The device is not ready */
			return ENXIO;

		case ERROR_NOT_SAME_DEVICE:
			/* The system cannot move the file to a different disk drive */
			return EXDEV;

		case ERROR_NOT_SUPPORTED:
			/* The request is not supported */
			return ENOTSUP;

		case ERROR_NO_MORE_FILES:
			/* There are no more files */
			return ENFILE;

		case ERROR_NO_MORE_USER_HANDLES:
			/* The current process has used all of its allowance of handles */
			return ENOMEM;

		case ERROR_NO_NETWORK:
			/* The network is not present or not started */
			return ENETDOWN;

		case ERROR_NO_PROC_SLOTS:
			/* The system cannot start another process at this time */
			return ENOMEM;

		case ERROR_NO_SYSTEM_RESOURCES:
			/* Insufficient system resources exist to complete the requested service */
			return ENOMEM;

		case ERROR_NONPAGED_SYSTEM_RESOURCES:
			/* Insufficient system resources exist to complete the requested service */
			return ENOMEM;

		case ERROR_NOT_DOS_DISK:
			/* The specified disk or diskette cannot be accessed */
			return ENODEV;

#ifndef ERROR_OBJECT_NAME_EXISTS
#define ERROR_OBJECT_NAME_EXISTS 698L
#endif
		case ERROR_OBJECT_NAME_EXISTS:
			/* The object name already existed */
			return EEXIST;

#ifndef ERROR_OFFSET_ALIGNMENT_VIOLATION
#define ERROR_OFFSET_ALIGNMENT_VIOLATION 327L
#endif
		case ERROR_OFFSET_ALIGNMENT_VIOLATION:
			/* The command specified a data offset that does not align to the device's granularity/alignment */
			return EINVAL;

#ifndef ERROR_OPERATION_IN_PROGRESS
#define ERROR_OPERATION_IN_PROGRESS 329L
#endif
		case ERROR_OPERATION_IN_PROGRESS:
			/* An operation is currently in progress with the device */
			return EINPROGRESS;

		case ERROR_OUT_OF_STRUCTURES:
			/* Storage to process this request is not available */
			return ENOMEM;

		case ERROR_PAGED_SYSTEM_RESOURCES:
			/* Insufficient system resources exist to complete the requested service */
			return EINVAL;

		case ERROR_POSSIBLE_DEADLOCK:
			/* A potential deadlock condition has been detected */
			return EWOULDBLOCK;

		case ERROR_PAGEFILE_QUOTA:
			/* Insufficient quota to complete the requested service */
			return ENOMEM;

		case ERROR_OPEN_FAILED:
			/* The system cannot open the device or file specified */
			return ENOENT;

		case ERROR_OPERATION_ABORTED:
			/* The I/O operation has been aborted */
			return ECANCELED;

		case ERROR_OUTOFMEMORY:
			/* Not enough storage is available to complete this operation */
			return ENOMEM;

		case ERROR_OUT_OF_PAPER:
			/* The printer is out of paper */
			return EIO;

#ifndef ERROR_PAGEFILE_QUOTA_EXCEEDED
#define ERROR_PAGEFILE_QUOTA_EXCEEDED 567L
#endif
		case ERROR_PAGEFILE_QUOTA_EXCEEDED:
			/* Page file quota was exceeded */
			return ENOMEM;

		case ERROR_PATH_NOT_FOUND:
			/* The system cannot find the path specified */
			return ENOENT;

		case ERROR_PIPE_NOT_CONNECTED:
			/* No process is on the other end of the pipe */
			return EPIPE;

		case ERROR_PROTOCOL_UNREACHABLE:
			/* The network location cannot be reached */
			return ENETUNREACH;

		case ERROR_PORT_UNREACHABLE:
			/* No service is operating at the destination network endpoint on the remote system */
			return ECONNRESET;

		case ERROR_READ_FAULT:
			/* The system cannot read from the specified device */
			return EIO;

#ifndef ERROR_RECEIVE_EXPEDITED
#define ERROR_RECEIVE_EXPEDITED 708L
#endif
		case ERROR_RECEIVE_EXPEDITED:
			/* Data was marked as expedited by the remote system */
			return EMSGSIZE;

#ifndef ERROR_RECEIVE_PARTIAL
#define ERROR_RECEIVE_PARTIAL 707L
#endif
		case ERROR_RECEIVE_PARTIAL:
			/* The network transport returned partial data to its client */
			return EMSGSIZE;

#ifndef ERROR_RECEIVE_PARTIAL_EXPEDITED
#define ERROR_RECEIVE_PARTIAL_EXPEDITED 709L
#endif
		case ERROR_RECEIVE_PARTIAL_EXPEDITED:
			/* Partial data was marked as expedited by the remote system */
			return EMSGSIZE;

		case ERROR_REM_NOT_LIST:
			/* Windows cannot find the network path */
			return ENETUNREACH;

#ifndef ERROR_REPARSE
#define ERROR_REPARSE 741L
#endif
		case ERROR_REPARSE:
			/* File resulted in a symbolic link */
			return EAGAIN;

		case ERROR_REQ_NOT_ACCEP:
			/* No more connections can be made to this remote computer at this time */
			return EAGAIN;

		case ERROR_REQUEST_ABORTED:
			/* The request was aborted */
			return ECANCELED;

		case ERROR_RETRY:
			/* The operation could not be completed. A retry should be performed */
			return EAGAIN;

		case ERROR_SECTOR_NOT_FOUND:
			/* The drive cannot find the sector requested */
			return EINVAL;

		case ERROR_SEEK:
			/* The drive cannot locate a specific area or track on the disk */
			return EINVAL;

		case ERROR_SEEK_ON_DEVICE:
			/* The file pointer cannot be set on the specified device or file */
			return EINVAL;

		case ERROR_SEM_IS_SET:
			/* The semaphore is set and cannot be closed */
			return EBUSY;

		case ERROR_SEM_NOT_FOUND:
			/* The specified system semaphore name was not found */
			return ENOENT;

		case ERROR_SEM_OWNER_DIED:
			/* The previous ownership of this semaphore has ended */
			return EOWNERDEAD;

		case ERROR_SEM_TIMEOUT:
			/* The semaphore timeout period has expired */
			return ETIMEDOUT;

		case ERROR_SEM_USER_LIMIT:
			return EAGAIN;

		case ERROR_SHARING_BUFFER_EXCEEDED:
			/* Too many files opened for sharing */
			return ENOLCK;

		case ERROR_SHARING_VIOLATION:
			/* The process cannot access the file because it is being used by another process */
			return EBUSY;

		case ERROR_TIMEOUT:
			/* This operation returned because the timeout period expired */
			return ETIMEDOUT;

#ifndef ERROR_TOO_MANY_DESCRIPTORS
#define ERROR_TOO_MANY_DESCRIPTORS 331L
#endif
		case ERROR_TOO_MANY_DESCRIPTORS:
			/* The command specified a number of descriptors that exceeded the maximum supported by the device */
			return ENOMEM;

		case ERROR_TOO_MANY_NAMES:
			/* The name limit for the local computer network adapter card was exceeded */
			return ENOSPC;

		case ERROR_TOO_MANY_OPEN_FILES:
			/* The system cannot open the file */
			return EMFILE;

		case ERROR_TOO_MANY_POSTS:
			/* Too many posts were made to a semaphore */
			return EINVAL;

		case ERROR_TOO_MANY_SEM_REQUESTS:
			/* The semaphore cannot be set again */
			return EBUSY;

		case ERROR_TOO_MANY_SEMAPHORES:
			/* Cannot create another system semaphore */
			return ENOMEM;

		case ERROR_TOO_MANY_TCBS:
			/* Cannot create another thread */
			return ENOMEM;

#ifndef ERROR_TOO_MANY_THREADS
#define ERROR_TOO_MANY_THREADS 565L
#endif
		case ERROR_TOO_MANY_THREADS:
			/* A process has too many threads to perform the requested action */
			return ENOMEM;

		case ERROR_UNEXP_NET_ERR:
			/* An unexpected network error occurred */
			return EIO;

		case ERROR_USER_MAPPED_FILE:
			/* Operation cannot be performed with a user-mapped section open */
			return EINVAL;

		case ERROR_WAIT_NO_CHILDREN:
			/* There are no child processes to wait for */
			return ECHILD;

		case ERROR_WRITE_FAULT:
			/* The system cannot write to the specified device */
			return EIO;

		case ERROR_WRITE_PROTECT:
			/* The media is write protected */
			return EACCES;

		case ERROR_WRONG_DISK:
			/* The wrong diskette is in the drive */
			return EIO;

		case ERROR_WORKING_SET_QUOTA:
			/* Insufficient quota to complete the requested service */
			return ENOMEM;

		case WAIT_TIMEOUT:
			/* The wait operation timed out */
			return ETIMEDOUT;

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

#ifndef WSA_SECURE_HOST_NOT_FOUND
#define WSA_SECURE_HOST_NOT_FOUND 11032L
#endif
		case WSA_SECURE_HOST_NOT_FOUND:
			/* No such host is known securely */
			return EHOSTDOWN;

#ifndef WSA_IPSEC_NAME_POLICY_ERROR
#define WSA_IPSEC_NAME_POLICY_ERROR 11033L
#endif
		case WSA_IPSEC_NAME_POLICY_ERROR:
			/* Name based IPSEC policy could not be added */
			return ENOSYS;

		/* case WSA_NOT_ENOUGH_MEMORY:
			ERROR_NOT_ENOUGH_MEMORY
		*/
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
