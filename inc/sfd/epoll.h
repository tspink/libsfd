/**
 * inc/sfd/epoll.h
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
#include <sfd/exception.h>
#include <vector>

namespace sfd {
	namespace EpollEventType {

		enum EpollEventType {
			IN = 0x001,
			PRI = 0x002,
			OUT = 0x004,
			RDNORM = 0x040,
			RDBAND = 0x080,
			WRNORM = 0x100,
			WRBAND = 0x200,
			MSG = 0x400,
			ERR = 0x008,
			HUP = 0x010,
			RDHUP = 0x2000,
			WAKEUP = 1u << 29,
			ONESHOT = 1u << 30,
			ET = 1u << 31
		};

		static inline EpollEventType operator|(EpollEventType& l, EpollEventType& r) {
			return (EpollEventType) ((unsigned long) l | (unsigned long) r);
		}
	}

	struct EpollEvent {
		FileDescriptor *fd;
		EpollEventType::EpollEventType event_type;

		inline bool in() const {
			return event_type & EpollEventType::IN;
		}

		inline bool out() const {
			return event_type & EpollEventType::OUT;
		}

		inline bool err() const {
			return event_type & EpollEventType::ERR;
		}

		inline bool hup() const {
			return event_type & EpollEventType::HUP;
		}
	};

	/**
	 * Represents a managed epollfd object.
	 */
	class Epoll : public FileDescriptor {
	public:
		Epoll();
		
		void add(FileDescriptor *fd, EpollEventType::EpollEventType event_types);
		void remove(FileDescriptor *fd);
		bool wait(std::vector<EpollEvent>& events, int max_events = 24, int timeout = -1);
	};

	class EpollException : public Exception {
	public:

		EpollException(const std::string& msg) : Exception(msg) {
		}
	};
}
