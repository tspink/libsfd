/**
 * src/regular-file.cpp
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
#include <sfd/regular-file.h>

#include <fcntl.h>

using namespace sfd;

/**
 * Constructs a new managed regular file.
 */
RegularFile::RegularFile(const std::string& filename, FileOpenMode::FileOpenMode mode) 
	: FileDescriptor(::open(filename.c_str(), sfd_mode_to_native_mode(mode)))
{
	if (!valid()) {
		throw RegularFileException("Error whilst opening file");
	}
}

/**
 * Converts an SFD file opening mode into the corresponding "native" opening mode.  An
 * exception is thrown for illegal combinations/invalid modes.
 * @param mode The SFD mode to convert.
 * @return Returns the native representation of this mode.
 */
int RegularFile::sfd_mode_to_native_mode(FileOpenMode::FileOpenMode mode)
{
	int native_mode;
	
	if (mode & FileOpenMode::READ) {
		if (mode & FileOpenMode::WRITE) {
			native_mode = O_RDWR;
		} else {
			native_mode = O_RDONLY;
		}
	} else if (mode & FileOpenMode::WRITE) {
		native_mode = O_WRONLY;
	} else {
		throw RegularFileException("Invalid file opening mode");
	}
	
	if (mode & FileOpenMode::CREATE) {
		native_mode |= O_CREAT;
	}

	if (mode & FileOpenMode::TRUNCATE) {
		native_mode |= O_TRUNC;
	}

	if (mode & FileOpenMode::SYNC) {
		native_mode |= O_SYNC;
	}

	if (mode & FileOpenMode::NO_CTTY) {
		native_mode |= O_NOCTTY;
	}
	
	return native_mode;
}
