//#include <genos/nav.h>
#include <genos/schedee.h>
#include <genos/resmngr.h>

#include <igris/util/pathops.h>
/*
int change_directory(const char * path, size_t sz)
{
	genos::schedee * sch = current_schedee();
	genos::navblock * navblock = sch->navblock;

	if (navblock == nullptr)
		return SET_ERRNO(-ENOTSUP);

	if (path_is_abs(path))
	{
		if (genos::directory_exists(path, sz))
		{
			sch->navblock->set(path);
		}
	}

	else
	{
		char * old = sch->navblock->add(path, sz);

		if (!genos::directory_exists(navblock->path(), sz))
		{
			navblock->restore(old);
			return SET_ERRNO(-ENOENT);
		}
	}

	return 0;
}
*/

int change_directory(int argc, char ** path) { return 0; }

int list_directory(int argc, char ** path) { return 0; }

SYSCMD(genos::navigation_shell_table,
	{"cd", change_directory, CMDFUNC, nullptr},
	{"ls", list_directory, CMDFUNC, nullptr}
);