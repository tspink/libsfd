/**
 * inc/sfd/net/socket.h
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

#include <sfd/fd.h>
#include <sfd/net/types.h>
#include <sfd/net/endpoint.h>
#include <sfd/exception.h>

namespace sfd {
	namespace net {

		class Socket : public FileDescriptor {
		public:
			Socket(AddressFamily::AddressFamily family, SocketType::SocketType type, ProtocolType::ProtocolType protocol);

			void bind(const EndPoint& ep);
			void listen(int max_pending);
			Socket *accept();

			void connect(const EndPoint& ep);

			const EndPoint *remote_endpoint() const {
				return _remote_endpoint;
			}

		private:
			Socket(int fd, AddressFamily::AddressFamily family, SocketType::SocketType type, ProtocolType::ProtocolType protocol, const EndPoint *rep);
			
			AddressFamily::AddressFamily _family;
			SocketType::SocketType _type;
			ProtocolType::ProtocolType _protocol;
			const EndPoint *_remote_endpoint;
		};

		class SocketException : public Exception {
		public:

			SocketException(const std::string& msg) : Exception(msg) {
			}
		};
	}
}
