#ifndef EXECUTOR_H
#define EXECUTOR_H

namespace genos
{
	class basic_executor
	{
	public:
		virtual int execute(const char* str, int len, int flags, int* p_ret) = 0;
	};

	template <class Record = std::pair<>>
	class executor
	{
		int (*funcs)(int, char**);

	public:
		func_executor(int (*funcs)(int, char**)) :
			funcs(funcs) {}

		int execute(const char* str, int len, int flags, int* p_ret) override
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

		};
	}

#endif