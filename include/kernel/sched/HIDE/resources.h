#ifndef GENOS_RESMNGR_H
#define GENOS_RESMNGR_H

#include <gxx/Vector.h>

#include <kernel/fs/file.h>
#include <gxx/ByteArray.h>

namespace Kernel {

	struct Resources {
		gxx::Vector< File* > fd_vector;
		gxx::string pwd = "/";

		int __errno = 0;
	};

	extern Resources* volatile currentResources;

};

#endif