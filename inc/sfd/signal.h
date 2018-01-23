/**
 * inc/sfd/signal.h
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

#include <set>

#include <sfd/fd.h>
#include <sfd/exception.h>

#include <sys/signalfd.h>

namespace sfd {
	namespace Signals {
		enum Signals {
			HUP = 1,
			INT = 2,
			QUIT = 3,
			FPE = 8,
			KILL = 9,
			ALRM = 14,
			TERM = 15,
		};
	}

	/**
	 * Represents a managed signalfd object.
	 */
	class Signal : public FileDescriptor {
	public:
		typedef std::set<Signals::Signals> SignalSet;
		
		Signal(const SignalSet& signals);
		virtual ~Signal();
		
	private:
		static int create_signalfd(const SignalSet& signals);
		static void signals_to_sigset(sigset_t *raw_sigset, const SignalSet& signals);
	};

	class SignalException : public Exception {
	public:

		SignalException(const std::string& msg) : Exception(msg) {
		}
	};
}
