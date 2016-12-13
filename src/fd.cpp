/**
 * src/fd.cpp
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
#include <sfd/fd.h>
#include <unistd.h>

using namespace sfd;

/**
 * Move Constructor
 */
FileDescriptor::FileDescriptor(FileDescriptor&& source_fd) : _fd(source_fd._fd)
{
	source_fd._fd = -1;
}

/**
 * Copy Constructor
 */
FileDescriptor::FileDescriptor(const FileDescriptor& source_fd) : _fd(dup(source_fd._fd))
{
}

/**
 * Constructs a FileDescriptor wrapper, for the given fd.
 * @param fd The fd to wrap.
 */
FileDescriptor::FileDescriptor(int fd) : _fd(fd)
{

}

/**
 * Performs a read operation on the file descriptor.
 * @param buffer The buffer to read into.
 * @param size The maximum size of the buffer.
 * @return The number of bytes read into the buffer.
 */
int FileDescriptor::read(void* buffer, size_t size)
{
	return ::read(_fd, buffer, size);
}

/**
 * Performs a write operation on the file descriptor.
 * @param buffer The buffer to read from.
 * @param size The number of bytes in the buffer to write.
 * @return The number of bytes written.
 */
int FileDescriptor::write(const void* buffer, size_t size)
{
	return ::write(_fd, buffer, size);
}

/**
 * Closes the file descriptor.
 */
void FileDescriptor::close()
{
	::close(_fd);
}

/**
 * Performs clean-up on the file descriptor by closing it.
 */
FileDescriptor::~FileDescriptor()
{
	this->close();
}
