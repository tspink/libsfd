/**
 * src/io/tty.cpp
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
#include <sfd/io/tty.h>
#include <termios.h>
#include <cstring>

using namespace sfd::io;

/**
 * Constructs a new managed TTY object.
 */
TTY::TTY(const std::string& device_path, BaudRate::BaudRate speed, DataBits::DataBits data_bits, Parity::Parity parity, StopBits::StopBits stop_bits)
	: RegularFile(device_path, FileOpenMode::READ | FileOpenMode::WRITE | FileOpenMode::NO_CTTY | FileOpenMode::SYNC),
		_speed(speed), _data_bits(data_bits), _parity(parity), _stop_bits(stop_bits)
{
	sync_settings();
}

void TTY::sync_settings()
{
	struct termios t;
	::bzero(&t, sizeof(t));
	
	if (tcgetattr(fd(), &t)) {
		throw TTYException("Unable to read TTY attributes");
	}
	
	// Put TTY into raw mode
	cfmakeraw(&t);
	
	// Apply baud rate
	cfsetispeed(&t, (speed_t)sfd_speed_to_native_speed(_speed));
	cfsetospeed(&t, (speed_t)sfd_speed_to_native_speed(_speed));
	
	// Apply data bits
	t.c_cflag &= ~CSIZE;
	switch (_data_bits) {
	case DataBits::D_8:
		t.c_cflag |= CS8;
		break;
	default: break;
	}
	
	// Apply parity
	t.c_cflag &= ~(PARENB | PARODD);
	switch (_parity) {
	case Parity::EVEN:
		t.c_cflag |= PARENB;
		break;
	case Parity::ODD:
		t.c_cflag |= (PARENB | PARODD);
		break;
	default: break;
	}
	
	// Apply stop bits
	t.c_cflag &= ~CSTOPB;
	switch (_stop_bits) {
	case StopBits::TWO:
		t.c_cflag |= CSTOPB;
		break;
	default: break;
	}

	t.c_cflag &= ~CRTSCTS;
	t.c_cflag |= (CLOCAL | CREAD);
	
	t.c_cc[VMIN] = 0;
	t.c_cc[VTIME] = 5;
	
	if (tcsetattr(fd(), TCSANOW, &t)) {
		throw TTYException("Unable to apply TTY attributes");
	}
}

void TTY::drain()
{
	tcdrain(fd());
}

unsigned int TTY::sfd_speed_to_native_speed(BaudRate::BaudRate speed)
{
	switch (speed) {
	case BaudRate::B_9600: return B9600;
	default:
		return 0;
	}
}
