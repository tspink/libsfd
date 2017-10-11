/**
 * inc/sfd/regular-file.h
 *
 * Copyright (c) 2017 Tom Spink <tspink@gmail.com>

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

namespace sfd {
	namespace FileOpenMode {
		enum FileOpenMode {
			READ		= 1,
			WRITE		= 2,
			CREATE		= 4,
			TRUNCATE	= 8,
			SYNC		= 16,
			NO_CTTY		= 32
		};

		static inline FileOpenMode operator|(const FileOpenMode& l, const FileOpenMode& r) {
			return (FileOpenMode) ((unsigned long) l | (unsigned long) r);
		}
	}

	/**
	 * Represents a managed regular file object.
	 */
	class RegularFile : public FileDescriptor {
	public:
		RegularFile(const std::string& filename, FileOpenMode::FileOpenMode mode);
		
	private:
		static int sfd_mode_to_native_mode(FileOpenMode::FileOpenMode mode);
	};

	class RegularFileException : public Exception {
	public:

		RegularFileException(const std::string& msg) : Exception(msg) {
		}		
	};
}
