#ifndef GENOS_MVFS_FDTABLE_H
#define GENOS_MVFS_FDTABLE_H

#include <string.h>
#include <errno.h>
#include <genos/mvfs/file.h>

#include <igris/dprint.h>

namespace genos
{
	struct fdtable
	{
		genos::file * tbl;
		uint8_t tblsz;

	public:
		void set_table(genos::file * tbl, uint8_t tblsz)
		{
			this->tbl = tbl;
			this->tblsz = tblsz;

			//dprptr(tbl);
			//dprdump(tbl, 20);

			memset(tbl, 0, sizeof(genos::file) * tblsz);
		}

		genos::file & operator[](int fd) { return tbl[fd]; }

		int get_file(int fd, genos::file ** ores)
		{
			if (fd >= tblsz || tbl[fd].node == nullptr)
				return EBADF;

			*ores = &tbl[fd];
			return 0;
		}

		int get_available_fd(int * fdp);
	};
}

#define FDTABLE(name, size) genos::file name[size]

#endif