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

/**
 * Constructs a new managed socket object, with the given address family, socket type and protocol type.
 * @param family The address family to associate with the new socket.
 * @param type The type of socket being created.
 * @param protocol The protocol of the new socket.
 */
Socket::Socket(AddressFamily::AddressFamily family, SocketType::SocketType type, ProtocolType::ProtocolType protocol)
	: FileDescriptor(::socket(family, type, protocol)),
		_family(family),
		_type(type),
		_protocol(protocol),
		_remote_endpoint(NULL)
{
	// Ensure that the created file-descriptor is valid.
	if (!valid()) {
		throw SocketException("Error whilst creating socket");
	}
}

/**
 * Constructs a managed socket object for the given pre-existing socketfd.  This constructor should only
 * be used when constructing the corresponding managed socket object, when accepting a connection on a
 * listening socket.
 * @param fd The native file-descriptor of the socket.
 * @param family The address family of the socket.
 * @param type The type of the socket.
 * @param protocol The protocol of the socket.
 * @param rep The associated remote-endpoint of the socket.
 */
Socket::Socket(FileDescriptor::NativeFD fd, AddressFamily::AddressFamily family, SocketType::SocketType type, ProtocolType::ProtocolType protocol, const EndPoint *rep)
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
 * Binds the socket to a particular endpoint.
 * @param ep The endpoint with which to bind the socket.
 */
void Socket::bind(const EndPoint& ep)
{
	// Make sure the endpoint family matches the family of the socket.
	if (ep.family() != _family)
		throw SocketException("Endpoint not of the correct family");

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

	// Create the associated remote endpoint.
	const EndPoint *rep = EndPoint::from_sockaddr(sa);
	free(sa);

	// Return a new Socket object that represents the accepted connection.
	return new Socket(new_fd, rep->family(), _type, _protocol, rep);
}

/**
 * Connects to the given remote endpoint.
 * @param ep The endpoint describing where to connect.
 */
void Socket::connect(const EndPoint& ep)
{
	// Make sure the endpoint family matches the family of the socket.
	if (ep.family() != _family)
		throw SocketException("Endpoint not of the correct family");

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

void Socket::shutdown(ShutdownModes::ShutdownModes mode)
{
	int how;
	
	switch (mode) {
	case ShutdownModes::Read:
		how = SHUT_RD;
		break;
	case ShutdownModes::Write:
		how = SHUT_WR;
		break;
	case ShutdownModes::Both:
		how = SHUT_RDWR;
		break;
	default:
		throw SocketException("Invalid shutdown mode");
	}
	
	if (::shutdown(fd(), how) < 0) {
		throw SocketException("Unable to shutdown socket");
	}
}

size_t Socket::send_to(const void* message, size_t length, const EndPoint& rep)
{
	// Make sure the remote endpoint family matches the family of this socket.
	if (rep.family() != _family)
		throw SocketException("Endpoint not of the correct family");

	// Obtain the sockaddr from the endpoint.
	socklen_t sa_len;
	struct sockaddr *sa = rep.create_sockaddr(sa_len);
	if (!sa) {
		throw SocketException("Unable to create sockaddr from endpoint");
	}
	
	ssize_t rc = ::sendto(fd(), message, length, 0, sa, sa_len);
	rep.free_sockaddr(sa);
	
	if (rc < 0) {
		throw SocketException("Unable to send message");
	}
	
	return (size_t)rc;
}

size_t Socket::recv_from(void* buffer, size_t length, EndPoint* rep)
{
	throw Exception::NotImplementedException;
}


void Socket::set_option_raw(int level, int setting, const void *value, size_t value_size)
{
	if (::setsockopt(fd(), level, setting, value, (socklen_t)value_size) < 0) {
		throw SocketException("Unable to set socket option");
	}
}

void Socket::get_option_raw(int level, int setting, void* value, size_t* value_size) const
{
	socklen_t native_value_size;
	
	if (value_size) native_value_size = *value_size;
	
	if (::getsockopt(fd(), level, setting, value, &native_value_size) < 0) {
		throw SocketException("Unable to get socket option");
	}
	
	if (value_size) *value_size = native_value_size;
}

bool Socket::debug() const
{
	return get_option<bool>(SOL_SOCKET, SO_DEBUG);
}

void Socket::debug(bool enable)
{
	try {
		set_option<bool>(SOL_SOCKET, SO_DEBUG, enable);
	} catch (const SocketException& ex) {
		throw SocketException("Unable to set debug socket option", ex);
	}
}

bool Socket::reuse_address() const
{
	return get_option<bool>(SOL_SOCKET, SO_REUSEADDR);
}

void Socket::reuse_address(bool enable)
{
	try {
		set_option<bool>(SOL_SOCKET, SO_REUSEADDR, enable);
	} catch (const SocketException& ex) {
		throw SocketException("Unable to set reuse address socket option.", ex);
	}
}

bool Socket::broadcast() const
{
		return get_option<bool>(SOL_SOCKET, SO_BROADCAST);
}

void Socket::broadcast(bool enable)
{
	try {
		set_option<bool>(SOL_SOCKET, SO_BROADCAST, enable);
	} catch (const SocketException& ex) {
		throw SocketException("Unable to set broadcast socket option.", ex);
	}
}
