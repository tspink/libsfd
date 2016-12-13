/**
 * inc/sfd/fd.h
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

#include <cstddef>

namespace sfd {

	/**
	 * An abstract representation of a file descriptor.
	 */
	class FileDescriptor {
	public:
		typedef int NativeFD;

		FileDescriptor(FileDescriptor&& fd);
		FileDescriptor(const FileDescriptor& fd);
		
		~FileDescriptor();
		void close();

		inline NativeFD fd() const { return _fd; }

		int read(void *buffer, size_t size);
		int write(const void *buffer, size_t size);
		
		inline bool valid() const { return _fd >= 0; }

	protected:
		FileDescriptor(NativeFD fd);

	private:
		NativeFD _fd;
	};
}
