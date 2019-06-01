#ifndef GENOS_NAV_H
#define GENOS_NAV_H

#include <igris/util/pathops.h>
#include <genos/resmngr.h>

namespace genos 
{
	class navigation_context
	{
		char * pwdbuf;
		int bufsz;

		navigation_context(char* buf, int bufsz) 
			: pwdbuf(buf), bufsz(bufsz) 
		{
			strcpy(pwdbuf, "/");
		}

		void add_node(const char * nd, int sz) 
		{
			strncat(pwdbuf, nd, sz);
		}

		void rm_node() 
		{
			char * lastnode = (char*)path_last_node(pwdbuf);
			*lastnode = '\0';
		}

		int apply(const char* path, size_t size) 
		{
			if (path_is_abs(path)) 
			{
				bool correct = directory_exists(path);
				
				if (!correct)
					return SET_ERRNO(ENOENT);

				strncpy(pwdbuf, path, size);
			}
			else 
			{
				char * oldpath = pwdbuf + strlen(pwdbuf);
				strncat(pwdbuf, path, size);

				bool correct = directory_exists(pwdbuf);

				if (!correct) 
				{
					*oldpath = '\0';
					return SET_ERRNO(ENOENT);
				}
			}

			return 0;
		} 
	};
}

#endif