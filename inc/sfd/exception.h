/**
 * inc/sfd/exception.h
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

#include <string>

namespace sfd {
	/**
	 * Represents a very basic exception.
	 */
	class Exception {
	public:
		static const Exception NotImplementedException;
		
		/**
		 * Constructs an exception object, described by the given message.
		 */
		Exception(const std::string& msg) : _msg(msg), _inner_exception(nullptr) {
		}
		
		/**
		 * Constructs an exception object, described by the given message and
		 * associated with the given inner exception.
		 */
		Exception(const std::string& msg, const Exception& inner) : _msg(msg), _inner_exception(&inner) {
		}

		/**
		 * Returns the message associated with this exception object.
		 */
		const std::string& message() const {
			return _msg;
		}
		
		/**
		 * Returns the inner exception associated with this exception object (if any).
		 */
		const Exception *inner_exception() const { return _inner_exception; }

	private:
		const std::string _msg;
		const Exception *_inner_exception;
	};
}
