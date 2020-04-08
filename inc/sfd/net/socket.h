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
#include <string>

namespace sfd {
	namespace net {
		
		namespace ShutdownModes
		{
			enum ShutdownModes
			{
				Read = 1,
				Write = 2,
				Both = 3
			};
		}

		class Socket : public FileDescriptor {
		public:
			Socket(AddressFamily::AddressFamily family, SocketType::SocketType type, ProtocolType::ProtocolType protocol);

			void bind(const EndPoint& ep);
			void listen(int max_pending);
			Socket *accept();

			void connect(const EndPoint& ep);
			void shutdown(ShutdownModes::ShutdownModes mode = ShutdownModes::Both);
			
			size_t send_to(const void *message, size_t length, const EndPoint& rep);
			size_t recv_from(void *buffer, size_t length, EndPoint *rep);

			const EndPoint *remote_endpoint() const {
				return _remote_endpoint;
			}
			
			bool debug() const;
			void debug(bool enable);

			bool reuse_address() const;
			void reuse_address(bool enable);
			
			bool broadcast() const;
			void broadcast(bool enable);

			void bind_to_device(const std::string& device_name);
			
		protected:
			void set_option_raw(int level, int setting, const void *value, size_t value_size);
			void get_option_raw(int level, int setting, void *value, size_t *value_size) const;
			
			template<typename T>
			void set_option(int level, int setting, T value)
			{
				set_option_raw(level, setting, &value, sizeof(value));
			}

			template<typename T>
			T get_option(int level, int setting) const
			{
				T value;
				size_t value_size = sizeof(value);
				
				get_option_raw(level, setting, &value, &value_size);
				return value;
			}

		private:
			Socket(FileDescriptor::NativeFD fd, AddressFamily::AddressFamily family, SocketType::SocketType type, ProtocolType::ProtocolType protocol, const EndPoint *rep);
						
			AddressFamily::AddressFamily _family;
			SocketType::SocketType _type;
			ProtocolType::ProtocolType _protocol;
			const EndPoint *_remote_endpoint;
		};

		class SocketException : public Exception {
		public:

			SocketException(const std::string& msg, const Exception& inner) : Exception(msg, inner) {
			}

			SocketException(const std::string& msg) : Exception(msg) {
			}
		};
		
		class IPSocket : public Socket {
		public:
			IPSocket(SocketType::SocketType type, ProtocolType::ProtocolType protocol);
			
			void multicast_loopback(bool enable);
		};
	}
}
