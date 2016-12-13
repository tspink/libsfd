/**
 * src/epoll.cpp
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
#include <sfd/epoll.h>
#include <sys/epoll.h>

using namespace sfd;

/**
 * Constructs a new managed epollfd.
 */
Epoll::Epoll() : FileDescriptor(::epoll_create1(0))
{
	if (!valid()) {
		throw EpollException("Error whilst creating epollfd");
	}
}

/**
 * Adds a file-descriptor to the epoll watch list.
 * @param fd The file-descriptor to add to the watch list.
 * @param events The events to associate with the file-descriptor on the watch list.
 */
void Epoll::add(FileDescriptor* incoming_fd, EpollEventType::EpollEventType events)
{
	// Construct a native epoll_event structure, to represent the fd being
	// added to the epollfd.
	struct epoll_event evt;
	evt.data.ptr = incoming_fd;
	evt.events = (uint32_t)events;

	// Add the fd to the epollfd.
	if (epoll_ctl(fd(), EPOLL_CTL_ADD, incoming_fd->fd(), &evt) < 0)
		throw EpollException("unable to add file descriptor");
}

/**
 * Removes a file-descriptor from the epoll watch list
 * @param fd The file-descriptor to remove from the watch list.
 */
void Epoll::remove(FileDescriptor* incoming_fd)
{
	// Tell the epollfd to remove the given file-descriptor.
	if (epoll_ctl(fd(), EPOLL_CTL_DEL, incoming_fd->fd(), NULL) < 0)
		throw EpollException("unable to remove file descriptor");
}

/**
 * Waits for one or more events to occur on the watched file-descriptors.
 * @param events A list to populate with events.  This list is NOT cleared.
 * @param max_events The maximum number of events to return.
 * @param timeout A timeout for the wait, or -1 for infinity.
 * @return Whether or not the wait proceeded without errors.
 */
bool Epoll::wait(std::vector<EpollEvent>& events, int max_events, int timeout)
{
	events.reserve(max_events);
	
	// Create a bunch of event descriptors on the stack.
	// TODO: maybe malloc() these instead.
	struct epoll_event evts[max_events];

	// Wait for events to become ready.
	int count = epoll_wait(fd(), evts, max_events, timeout);

	// If there was an error, check to see if it was because it was interrupted,
	// or if there was an ACTUAL error.
	if (count < 0) {
		if (errno == EINTR) {
			return true;
		}
		
		return false;
	}

	// Enumerate over all the ready events, and populate the event list.
	for (int i = 0; i < count; i++) {
		events.push_back({
			(FileDescriptor *)evts[i].data.ptr,
			(EpollEventType::EpollEventType)evts[i].events
		});
	}

	return true;
}
