#include <sfd/net/tuntap.h>

#include <linux/if.h>
#include <linux/if_tun.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

using namespace sfd::net;

TunTap::TunTap(TunTapMode mode, const std::string& device_name) : FileDescriptor(alloc_tun(mode, device_name))
{

}

TunTap::~TunTap()
{

}

int TunTap::alloc_tun(TunTapMode mode, const std::string& device_name)
{
    struct ifreq ifr;
	int fd, err;

	if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
		return fd;
	}

	bzero(&ifr, sizeof(ifr));
	ifr.ifr_flags = (mode == TunTapMode::TapMode ? IFF_TAP : IFF_TUN) | IFF_NO_PI;

	if (device_name != "") {
		strncpy(ifr.ifr_name, device_name.c_str(), IFNAMSIZ - 1);
	}

	if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
		::close(fd);
		return err;
	}

	return fd;
}
