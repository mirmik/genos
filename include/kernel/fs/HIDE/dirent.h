#ifndef GENOS_KERNEL_DIRENT_H
#define GENOS_KERNEL_DIRENT_H

namespace Kernel {

	class dirent {
	public:
		uint8_t type;
		uint16_t flags;
		char name[1];
	};

};

#endif