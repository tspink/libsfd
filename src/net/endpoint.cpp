/**
 * src/net/endpoint.cpp
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
#include <sfd/net/ip-endpoint.h>
#include <sfd/net/unix-endpoint.h>
#include <sfd/net/l2-endpoint.h>

#include <malloc.h>
#include <netinet/in.h>
#include <sys/un.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>

using namespace sfd::net;

EndPoint::EndPoint(AddressFamily::AddressFamily af) : _family(af) { }

EndPoint::~EndPoint() {
}

/**
 * Constructs an EndPoint from a given sockaddr.
 * @param sa The sockaddr to construct the EndPoint from.
 * @return An EndPoint representing the given sockaddr.
 */
const EndPoint *EndPoint::from_sockaddr(const struct sockaddr *sa)
{
	// We can only construct endpoints from families we know about.
	if (sa->sa_family == AF_INET) {
		// Create an IPEndPoint
		const struct sockaddr_in *sa_in = (const struct sockaddr_in *)sa;
		return new IPEndPoint(IPAddress(ntohl(sa_in->sin_addr.s_addr)), ntohs(sa_in->sin_port));
	} else {
		return NULL;
	}
}

/**
 * Constructs an IPEndPoint from the given IPAddress and port.
 * @param addr The IPAddress to be associated with this endpoint.
 * @param port The port numebr to be associated with this endpoint.
 */
IPEndPoint::IPEndPoint(const IPAddress& addr, int port) : EndPoint(AddressFamily::IPv4), _addr(addr), _port(port)
{

}

IPEndPoint::~IPEndPoint()
{
}

/**
 * Creates a sockaddr from the IPEndPoint.
 * @param len The output length of the sockaddr
 * @return A pointer to a sockaddr.  This must be freed with @free_sockaddr.
 */
struct sockaddr *IPEndPoint::create_sockaddr(socklen_t& len) const
{
	// Allocate storage for the sockaddr.
	struct sockaddr_in *sa = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	if (!sa) return NULL;
	
	len = sizeof(struct sockaddr_in);
	
	// Populate the sockaddr.
	sa->sin_family = AF_INET;
	sa->sin_port = htons(_port);
	sa->sin_addr.s_addr = htonl(_addr.address());

	// Return the sockaddr.
	return (struct sockaddr *)sa;
}

/**
 * Releases the memory associated with the given sockaddr.
 * @param sa The sockaddr to free.
 */
void IPEndPoint::free_sockaddr(struct sockaddr *sa) const
{
	free(sa);
}

UnixEndPoint::UnixEndPoint(const std::string& path) : EndPoint(AddressFamily::Unix), _path(path)
{
}

UnixEndPoint::~UnixEndPoint()
{

}

struct sockaddr* UnixEndPoint::create_sockaddr(socklen_t& len) const
{
	// Allocate storage for the sockaddr.
	struct sockaddr_un *sa = (struct sockaddr_un *)malloc(sizeof(struct sockaddr_un));
	if (!sa) return NULL;
	
	len = sizeof(struct sockaddr_un);
	
	// Populate the sockaddr.
	sa->sun_family = AF_UNIX;
	strncpy(sa->sun_path, _path.c_str(), sizeof(sa->sun_path)-1);

	// Return the sockaddr.
	return (struct sockaddr *)sa;
}

void UnixEndPoint::free_sockaddr(struct sockaddr* sa) const
{
	free(sa);
}

L2EndPoint::L2EndPoint(const BluetoothAddress& addr, short psm) : EndPoint(AddressFamily::Bluetooth), _addr(addr), _psm(psm)
{

}

L2EndPoint::~L2EndPoint()
{

}

struct sockaddr* L2EndPoint::create_sockaddr(socklen_t& len) const
{
	// Allocate storage for the sockaddr.
	struct sockaddr_l2 *sa = (struct sockaddr_l2 *)malloc(sizeof(struct sockaddr_l2));
	if (!sa) return NULL;
	
	len = sizeof(struct sockaddr_l2);
	
	// Populate the sockaddr.
	bzero(sa, sizeof(*sa));
	
	sa->l2_family = AF_BLUETOOTH;
	sa->l2_bdaddr = *(bdaddr_t *)_addr.address().address;
	sa->l2_psm = htobs(_psm);
	
	// Return the sockaddr.
	return (struct sockaddr *)sa;
}

void L2EndPoint::free_sockaddr(struct sockaddr* sa) const
{
	free(sa);
}

