/**
 * inc/sfd/net/l2-endpoint.h
 *
 * Copyright (c) 2018 Tom Spink <tspink@gmail.com>

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
#include <sfd/net/bt-address.h>

namespace sfd {
	namespace net {

		class L2EndPoint : public EndPoint {
		public:
			L2EndPoint(const BluetoothAddress& addr, short psm);
			~L2EndPoint();

			const BluetoothAddress& address() const {
				return _addr;
			}

			short psm() const {
				return _psm;
			}

			virtual struct sockaddr *create_sockaddr(socklen_t& len) const override;
			virtual void free_sockaddr(struct sockaddr *sa) const override;
			
		private:
			const BluetoothAddress _addr;
			short _psm;
		};
	}
}
