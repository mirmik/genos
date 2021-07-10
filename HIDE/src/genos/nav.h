#ifndef GENOS_NAV_H
#define GENOS_NAV_H

#define MAX_PATH_LEN 32

/*namespace genos
{
	class navblock
	{
		char * buf;
		int cap;

	public:
		navblock(char* buf, int cap)
			: buf(buf), cap(cap)
		{
			strcpy(buf, "/");
		}

		// Установить новое значение пути. Путь обязан быть абсолютным.
		void set(const char * abspath)
		{
			strcpy(buf, abspath);
		}

		int apply(const char * path)
		{
			char npath[MAX_PATH_LEN];

			if (!path_is_abs(path))
				path_simplify_join(npath, buf, path);
			else
				path_simplify(npath, path);

			if (directory_exists(npath, strlen(npath)))
				set(npath);
			else 
			{
				errno = -ENOENT;
				return -1;
			}

			return 0;
		}
	};

	extern igris::console_command navigation_shell_table[];
}*/

struct schedee;

struct navigation_block 
{
	struct schedee * sch;
	uint8_t flags;
	char * pwd;
}


#endif