#ifndef GENOS_EXECUTOR_H
#define GENOS_EXECUTOR_H

#include <genos/syscmd.h>
#include <igris/datastruct/argvc.h>

#include <unistd.h>

#define EXECUTOR_PROCESS_STARTED 2
#define EXECUTOR_ERROR -1

#define EXECUTOR_TBLFIN nullptr

namespace genos
{
	class executor
	{
		syscmd_command ** tbl;

	public:
		executor(genos::syscmd_command** syscmdtbl) : tbl(syscmdtbl) {}

		virtual int execute(char * str, int len, int flags, int * retptr)
		{
			int flen = 0;
			int argc;
			int res;
			char * argv[10];
			struct syscmd_command ** it0;
			struct syscmd_command * it1;

			if (!(flags & SH_INTERNAL_SPLIT))
			{
				BUG();
			}

			if (len <= 0)
			{
				return 0;
			}

			// Скипаем ведущие пробелы
			while (*str == ' ')
				++str;

			// Ищем длину первого слова
			while (flen != len && str[flen] != '\0' && str[flen] != ' ')
				++flen;

			// Встроенная функция help
			if (flen == 4 && !strncmp(str, "help", 4))
			{
				for (it0 = tbl; *it0 != nullptr; ++it0)
				{
					for (it1 = *it0; it1->func != NULL; ++it1)
					{
						if (it1->help)
						{
							write(0, it1->name, strlen(it1->name));
							write(0, " - ", 3);
							write(0, it1->help, strlen(it1->help));
							write(0, "\r\n", 2);
						}
						else
						{
							write(0, it1->name, strlen(it1->name));
							write(0, "\r\n", 2);
						}
					}
				}
				return 0;
			}

			// Основной цикл
			for (it0 = tbl; *it0 != nullptr; ++it0)
			{
				for (it1 = *it0; it1->func != NULL; ++it1)
				{
					if (!strncmp(str, it1->name, flen))
					{
						switch (it1->type)
						{
							case CMDFUNC:
								argc = argvc_internal_split(str, argv, 10);
								res = ((syscmd_func_t)(it1->func))(argc, argv);

								if (retptr) *retptr = res;

								return 0;

							case CMDAUTOM:
								BUG();

							case CMDCOOP:
								BUG();
								argc = 0;
								//return mshell_make_process(it1->func, argc, argv);
						}
					}
				}
			}

			return ENOENT;
		}
	};
}

#define EXECUTOR(name, ...) \
genos::syscmd_command* name##_syscmd_table[] {__VA_ARGS__, EXECUTOR_TBLFIN}; \
genos::executor name{name##_syscmd_table}

#endif