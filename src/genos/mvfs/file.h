#ifndef GENOS_MVFS_FILE_H
#define GENOS_MVFS_FILE_H

#include <genos/mvfs/vfsnode.h>

namespace genos
{
	struct file
	{
		int pos;
		uint8_t refs;
		genos::vfsnode * node; ///< Родительский node. Держит ссылку.
	};
}

#endif
