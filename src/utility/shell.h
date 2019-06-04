#ifndef GENOS_UTILITY_SHELL_H
#define GENOS_UTILITY_SHELL_H

#define SHELL_MAXLINE_SIZE 32

namespace genos 
{
	class shell_process : public automate_schedee
	{
		int doit(const char * line) 
		{
			(void) line;
			dprln("do:", line);
			return 0;
		}

		int signal_handler(int sig) 
		{
			
		}

		void execute() override 
		{
			char line[SHELL_MAXLINE_SIZE];

			switch (state) 
			{
				case 0:
					state = 1;
					read(STDIN_FILENO, nullptr, 0);
					break;
				case 1:
					state = 2;
					read(STDIN_FILENO, line, SHELL_MAXLINE_SIZE);
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