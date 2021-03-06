/**
 * inc/sfd/net/ip-endpoint.h
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
#pragma once

#include <sfd/net/endpoint.h>
#include <sfd/net/ip-address.h>

namespace sfd {
	namespace net {

		class IPEndPoint : public EndPoint {
		public:
			IPEndPoint(const IPAddress& addr, int port);
			~IPEndPoint();

			IPEndPoint(const IPEndPoint& o) : EndPoint(o.family()), _addr(o._addr), _port(o._port) { }
			IPEndPoint(IPEndPoint&& o) noexcept : EndPoint(o.family()), _addr(o._addr), _port(o._port) { }

			IPEndPoint& operator=(const IPEndPoint& o)
			{
				IPEndPoint tmp(o);
				*this = std::move(tmp);
				return *this;
			}

			IPEndPoint& operator=(IPEndPoint&& o) noexcept
			{
				if (this == &o) {
					return *this;
				}

				_addr = o._addr;
				_port = o._port;
				return *this;
			}

			const IPAddress& address() const {
				return _addr;
			}

			int port() const {
				return _port;
			}

			virtual struct sockaddr *create_sockaddr(socklen_t& len) const override;
			virtual void free_sockaddr(struct sockaddr *sa) const override;

		private:
			IPAddress _addr;
			int _port;
		};
	}
}
