/**
 * inc/sfd/net/bt-address.h
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

#include <cstdint>
#include <string>

namespace sfd {
	namespace net {
		struct RawAddress {
			typedef unsigned char AddressStorageType[6];
			
			AddressStorageType address;
		};
		
		class BluetoothAddress {
		public:
			BluetoothAddress(const RawAddress& address) : _address(address) {
			}

			RawAddress address() const {
				return _address;
			}

			std::string to_string() const {
				char buffer[32] = {0};
				sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x", 
						_address.address[0],
						_address.address[1],
						_address.address[2],
						_address.address[3],
						_address.address[4],
						_address.address[5]);
				
				return std::string(buffer);
			}
			
			static inline BluetoothAddress any() {
				RawAddress addr;
				addr.address[0] = 0x00;
				addr.address[1] = 0x00;
				addr.address[2] = 0x00;
				addr.address[3] = 0x00;
				addr.address[4] = 0x00;
				addr.address[5] = 0x00;
				
				return BluetoothAddress(addr);
			}

			static inline BluetoothAddress all() {
				RawAddress addr;
				addr.address[0] = 0xff;
				addr.address[1] = 0xff;
				addr.address[2] = 0xff;
				addr.address[3] = 0xff;
				addr.address[4] = 0xff;
				addr.address[5] = 0xff;
				
				return BluetoothAddress(addr);
			}

			static inline BluetoothAddress local() {
				RawAddress addr;
				addr.address[0] = 0x00;
				addr.address[1] = 0x00;
				addr.address[2] = 0x00;
				addr.address[3] = 0xff;
				addr.address[4] = 0xff;
				addr.address[5] = 0xff;
				
				return BluetoothAddress(addr);
			}

		private:
			RawAddress _address;
		};
	}
}
