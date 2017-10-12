/**
 * inc/sfd/io/tty.h
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

#include <sfd/regular-file.h>
#include <sfd/exception.h>

namespace sfd {
	namespace io {
		namespace BaudRate {
			enum BaudRate {
				B_9600 = 0
			};
		}

		namespace DataBits {
			enum DataBits {
				D_8
			};
		}

		namespace Parity {
			enum Parity {
				NONE,
				ODD,
				EVEN
			};
		}

		namespace StopBits {
			enum StopBits {
				ONE,
				TWO
			};
		}

		/**
		 * Represents a tty device
		 */
		class TTY : public RegularFile {
		public:
			TTY(const std::string& device_path, 
					BaudRate::BaudRate speed, 
					DataBits::DataBits data_bits = DataBits::D_8, 
					Parity::Parity parity = Parity::NONE, 
					StopBits::StopBits stop_bits = StopBits::ONE);

			void drain();
			
			void set_rts(bool set);
			
		private:
			static unsigned int sfd_speed_to_native_speed(BaudRate::BaudRate speed);

			void sync_settings();

			BaudRate::BaudRate _speed;
			DataBits::DataBits _data_bits;
			Parity::Parity _parity;
			StopBits::StopBits _stop_bits;
		};

		class TTYException : public Exception {
		public:

			TTYException(const std::string& msg) : Exception(msg) {
			}		
		};
	}
}
