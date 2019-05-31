#ifndef GENOS_FDTABLE_H
#define GENOS_FDTABLE_H

#include <string.h>
#include <errno.h>

#include <genos/node.h>
#include <igris/dprint.h>

namespace genos
{
	struct fdtable
	{
		genos::opennode * tbl;
		uint8_t tblsz;

	public:
		void set_table(genos::opennode * tbl, uint8_t tblsz)
		{
			this->tbl = tbl;
			this->tblsz = tblsz;

			//dprptr(tbl);
			//dprdump(tbl, 20);

			memset(tbl, 0, sizeof(genos::opennode) * tblsz);
		}

		genos::opennode & operator[](int fd) { return tbl[fd]; }

		int get_file(int fd, genos::opennode ** ores)
		{
			if (fd >= tblsz || tbl[fd].node == nullptr)
				return EBADF;

			*ores = &tbl[fd];
			return 0;
		}

		int get_available_fd(int * fdp);
	};
}

#define FDTABLE(name, size) genos::opennode name[size]

#endif