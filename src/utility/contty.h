#ifndef GENOS_UTILITY_CONTTY_H
#define GENOS_UTILITY_CONTTY_H

#include <sys/cdefs.h>
#include <igris/shell/readline.h>

#include <genos/schedee/autom.h>
#include <igris/shell/executor.h>
#include <genos/nav.h>

#define CONTTY_LINE_LENGTH 32
#define CONTTY_HISTORY_SIZE 3

namespace genos
{
	class contty : public genos::autom_schedee_base
	{
		struct readline rl;

		node * outside;
		igris::executor * ex;

		const char * prefix_string = "$ ";
		char last;

		char buffer[CONTTY_LINE_LENGTH];
		char hbuffer[CONTTY_LINE_LENGTH * CONTTY_HISTORY_SIZE];

		void newline();

	public:
		uint8_t debug_mode = 0;
		bool echo = true;

		contty(genos::node * outside, igris::executor* ex) 
			: outside(outside), ex(ex)
		{}

		void set_prefix_string(const char* str) 
		{
			prefix_string = str;
		} 

		void execute() override;
	};
}

#endif