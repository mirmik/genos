#include <genos/syscmd.h>

igris::executor * genos::system_executor = nullptr;

void genos::set_system_executor(igris::executor * executor)
{
	system_executor = executor;
}

int genos::system(const char * cmd)
{
	if (!system_executor)
		return -1;

	char buf[strlen(cmd) + 1];
	strcpy(buf, cmd);

	int sts;
	int ret = system_executor->execute(
	           buf,
	           strlen(buf),
	           SH_INTERNAL_SPLIT,
	           & sts
	       );

	if (ret) return ret;
	if (sts) return sts;
	return 0; 
}