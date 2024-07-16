#ifndef GENOS_UTILITY_SHELL_H
#define GENOS_UTILITY_SHELL_H

#include <genos/schedee/autom.h>

#define SHELL_MAXLINE_SIZE 32

namespace genos 
{
	class shell_automate : public automate_schedee_base
	{
		genos::openres restbl;
		const char * iopath;

	public:
		shell_automate(const char * iopath)
			: iopath(iopath) 
		{
			set_restbl(&restbl, 1);
		}

		int doit(const char * line) 
		{
			(void) line;
			dprln("do:", line);
			return 0;
		}

		int signal_handler(int sig) 
		{
			dprln("SIGNAL!!!");
		}

		void execute() override 
		{
			char line[SHELL_MAXLINE_SIZE];

			switch (state) 
			{
				case 0:
					open(iopath, 0);
					break;

				case 0:
					state = 1;
					outside->write("$ ", 2);
					outside->read(nullptr, 0);
					break;
				case 1:
					state = 2;
					outside->read(line, SHELL_MAXLINE_SIZE);
					doit(line);
					break;
				case 2:
					state = 0;
					break;
			}
		}
	}
}

#endif