#ifndef GENOS_NAV_H
#define GENOS_NAV_H

#include <igris/util/pathops.h>
#include <genos/resmngr.h>

namespace genos 
{
	class navigation_context
	{
		char * buf;
		int cap;

	public:
		navigation_context(char* buf, int cap) 
			: buf(buf), cap(cap) 
		{
			strcpy(path, "/");
		}

		// Установить новое значение пути. Путь обязан быть абсолютным.
		char * set(char * abspath); 
		 

		// Добавить часть пути к текущему. Возвращает указатель на предыдущий финал строки.
		// Для возможности восстановления её состояния.
		char * add(char * addpath);

		// Восстановить ранее предыдущее состояние.
		void restore(char * old);

		const char * path() 
		{
			return buf;
		}

		/*int apply(const char* path, size_t size) 
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
		} */
	};
}

#endif