#ifndef GENOS_NAV_H
#define GENOS_NAV_H

//#include <igris/util/pathops.h>
#include <string.h>
#include <genos/resmngr.h>

namespace genos
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

		// Добавить часть пути к текущему. Возвращает указатель на предыдущий финал строки.
		// Для возможности восстановления её состояния.
		char * add(const char * addpath, size_t sz)
		{
			char * old = buf + strlen(buf);
			strncat(buf, addpath, sz);

			if (buf[strlen(buf) - 1] != '/')
				strcat(buf, "/");

			return old;
		}

		// Восстановить ранее предыдущее состояние.
		void restore(char * old)
		{
			*old = '\0';
		}

		const char * path()
		{
			return buf;
		}
	};

	int change_directory(const char * path);
}

#endif