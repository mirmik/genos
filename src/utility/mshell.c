#include <utility/mshell.h>
//#include <sched/api.h>
//#include <mvfs/file.h>

//#include <unistd.h>
//#include <drivers/cdev/cdev.h>

#include <string.h>
#include <errno.h>
#include <igris/datastruct/argvc.h>

#include <igris/util/bug.h>

extern struct mshell_command mshell_commands_table[];

int mshell_execn(char* str, int len, int flags, int * retptr)
{
	int flen;
	int argc;
	int res;
	char * argv[10];
	struct mshell_command * it;

	if (!(flags & SH_INTERNAL_SPLIT))
	{
		BUG();
	}

	//char* argv[SSHELL_ARGCMAX];
	//int argc;
	//int res;

	if (len == 0)
	{
		return MSHELL_OK;
	}

	flen = argvc_length_of_first(str);

	it = mshell_commands_table;
	for (; it->func != NULL; ++it)
	{
		if ((strlen(it->name) == flen) && !strncmp(str, it->name, flen))
		{
			switch (it->type)
			{
				case MSHELL_FUNCTION:
					argc = argvc_internal_split(str, argv, 10);
					res = it->func(argc, argv);
					if (retptr) *retptr = res;
					return MSHELL_OK;

				case MSHELL_AUTOMATE:
				case MSHELL_PROCESS:
					BUG();
			}
		}
	}

	return ENOENT;
}