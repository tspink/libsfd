/**
 * src/net/socket.cpp
 *
 * Copyright (c) 2016 Tom Spink <tspink@gmail.com>

 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of
 * the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <sfd/net/socket.h>
#include <sys/socket.h>

using namespace sfd;
using namespace sfd::net;

Socket::Socket(AddressFamily::AddressFamily family, SocketType::SocketType type, ProtocolType::ProtocolType protocol)
	: FileDescriptor(::socket(family, type, protocol)),
		_family(family),
		_type(type),
		_protocol(protocol),
		_remote_endpoint(NULL)
{
	if (!valid()) {
		throw SocketException("Error whilst creating socket");
	}
}

Socket::Socket(int fd, AddressFamily::AddressFamily family, SocketType::SocketType type, ProtocolType::ProtocolType protocol, const EndPoint *rep)
	: FileDescriptor(fd),
		_family(family),
		_type(type),
		_protocol(protocol),
		_remote_endpoint(rep)
{
	if (!valid()) {
		throw SocketException("Error whilst creating socket");
	}
}

/**
 * Binds the socket to a particular end-point.
 * @param ep The end-point with which to bind the socket.
 */
void Socket::bind(const EndPoint& ep)
{
	// Make sure the end-point family matches the family of the socket.
	if (ep.family() != _family)
		throw SocketException("End-point not of the correct family");

	// Obtain the sockaddr from the endpoint.
	socklen_t sa_len;
	struct sockaddr *sa = ep.create_sockaddr(sa_len);
	if (!sa) {
		throw SocketException("Unable to create sockaddr from endpoint");
	}

	// Bind the socket, and release the sockaddr memory.
	int rc = ::bind(fd(), sa, sa_len);
	ep.free_sockaddr(sa);

	// Check for any errors during the bind.
	if (rc < 0) {
		throw SocketException("Unable to bind to socket");
	}
}

/**
 * Starts a socket listening for connections.
 * @param max_pending The maximum number of pending connections in the accept queue.
 */
void Socket::listen(int max_pending)
{
	// Instruct the socket to being listening.
	if (::listen(fd(), max_pending) < 0) {
		throw SocketException("Unable to listen on socket");
	}
}

/**
 * Accepts a new connection pending on this socket, in the form of a new
 * socket object representing the client connection.
 * @return A new socket representing the client connection.
 */
Socket* Socket::accept()
{
	// Allocate an arbitrary amount of storage for the remote socaddr.
	struct sockaddr *sa = (struct sockaddr *)malloc(256);
	socklen_t sa_len = 256;

	// Accept the new socket.
	int new_fd = ::accept(fd(), sa, &sa_len);
	if (new_fd < 0) {
		free(sa);
		return NULL;
	}

	// Create the associated remote end-point.
	const EndPoint *rep = EndPoint::from_sockaddr(sa);
	free(sa);

	// Return a new Socket object that represents the accepted connection.
	return new Socket(new_fd, rep->family(), _type, _protocol, rep);
}

/**
 * Uses this socket to connect to a remote endpoint.
 */
void Socket::connect(const EndPoint& ep)
{
	// Make sure the end-point family matches the family of the socket.
	if (ep.family() != _family)
		throw SocketException("End-point not of the correct family");

	// Obtain the sockaddr from the endpoint.
	socklen_t sa_len;
	struct sockaddr *sa = ep.create_sockaddr(sa_len);
	if (!sa) {
		throw SocketException("Unable to create sockaddr from endpoint");
	}

	// Connect, and release the sockaddr memory.
	int rc = ::connect(fd(), sa, sa_len);
	ep.free_sockaddr(sa);

	// Check for any errors during the bind.
	if (rc < 0) {
		throw SocketException("Unable to connect");
	}
}
