/**
 * inc/sfd/net/types.h
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

namespace sfd {
	namespace net {
		namespace AddressFamily {

			enum AddressFamily {
				Unix = 1,
				IPv4 = 2,
				IPv6 = 10,
				Bluetooth = 31,
				None = 255
			};
		}

		namespace SocketType {

			enum SocketType {
				Stream = 1,
				Datagram = 2,
				Raw = 3,
				SeqPacket = 5,
			};
		}

		namespace ProtocolType {

			enum ProtocolType {
				None = 0,
				
				// Well-defined IP Protocols
				IP   = 0,
				ICMP = 1,
				IGMP = 2,
				TCP = 6,
				UDP = 17,
				RAW = 255,
				
				// Bluetooth
				BT_L2CAP = 0,
				BT_HCI = 1,
				BT_SCO = 2,
				BT_RFCOMM = 3,
				BT_BNEP = 4,
				BT_CMTP = 5,
				BT_HIDP = 6,
				BT_AVDTP = 7,
			};
		}
	}
}
