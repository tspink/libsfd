/**
 * inc/sfd/net/ip-address.h
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

#include <cstdint>
#include <string>

namespace sfd {
	namespace net {

		class IPAddress {
		public:
			typedef uint32_t AddressStorageType;
			
			IPAddress(AddressStorageType address) : _address(address) {
			}

			AddressStorageType address() const {
				return _address;
			}

			inline void to_octets(uint8_t& a, uint8_t& b, uint8_t& c, uint8_t& d) const {
				a = (_address >> 24) & 0xff;
				b = (_address >> 16) & 0xff;
				c = (_address >> 8) & 0xff;
				d = (_address >> 0) & 0xff;
			}

			std::string to_string() const {
				uint8_t a, b, c, d;
				to_octets(a, b, c, d);

				return std::to_string(a) + "." + std::to_string(b) + "." + std::to_string(c) + "." + std::to_string(d);
			}

			static inline IPAddress from_octets(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
				return IPAddress((uint32_t) a << 24 | (uint32_t) b << 16 | (uint32_t) c << 8 | (uint32_t) d);
			}

			static inline IPAddress any() {
				return IPAddress(0);
			}

			static inline IPAddress broadcast() {
				return IPAddress(0xffffffff);
			}

			static inline IPAddress localhost() {
				return from_octets(127, 0, 0, 1);
			}

		private:
			AddressStorageType _address;
		};
	}
}
