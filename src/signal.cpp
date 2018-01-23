/**
 * src/signal.cpp
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
#include <sfd/signal.h>

#include <sys/signal.h>
#include <unistd.h>

using namespace sfd;

/**
 * Creates a new managed signalfd object
 */
Signal::Signal(const SignalSet& signals) : FileDescriptor(create_signalfd(signals))
{
	if (!valid()) {
		throw SignalException("Error whilst creating signalfd");
	}	
}

int Signal::create_signalfd(const SignalSet& signals)
{
	sigset_t raw_sigset;
	signals_to_sigset(&raw_sigset, signals);
	
	int fd = ::signalfd(-1, &raw_sigset, SFD_NONBLOCK | SFD_CLOEXEC);
	
	if (fd < 0) {
		return fd;
	}
	
	sigprocmask(SIG_BLOCK, &raw_sigset, nullptr);
	return fd;
}

void Signal::signals_to_sigset(sigset_t* raw_sigset, const SignalSet& signals)
{
	sigemptyset(raw_sigset);
	
	for (Signals::Signals signal : signals) {
		sigaddset(raw_sigset, (int)signal);
	}
}

Signal::~Signal()
{

}
