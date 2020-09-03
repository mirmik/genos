#ifndef GENOS_UTILITY_CONTTY_H
#define GENOS_UTILITY_CONTTY_H

#include <sys/cdefs.h>
#include <igris/shell/readline.h>

#include <genos/schedee/autom.h>
#include <igris/shell/executor.h>
#include <genos/nav.h>

#define CONTTY_LINE_LENGTH 64
#define CONTTY_HISTORY_SIZE 5

namespace genos
{
	class contty : public genos::autom_schedee_base
	{
		struct readline rl;

		node * outside;
		igris::executor * ex;

		const char * prefix_string = "$ ";
		const char * machine_name = "";
		char last;

		// TODO: сделать вариабельными с возможностью отключения истории
		char buffer[CONTTY_LINE_LENGTH];
		char hbuffer[CONTTY_LINE_LENGTH * CONTTY_HISTORY_SIZE];

		void newline();

	public:
		uint8_t debug_mode = 0;
		bool echo = true;

		contty(genos::node * outside, igris::executor* ex) 
			: outside(outside), ex(ex)
		{
			readline_init(&rl, buffer, CONTTY_LINE_LENGTH);
			readline_history_init(&rl, hbuffer, CONTTY_HISTORY_SIZE);
		}

		void set_prefix(const char* str) 
		{
			prefix_string = str;
		} 
		
		void set_machine(const char* str) 
		{
			machine_name = str;
		} 

		void push_history(const char * str) 
		{
			readline_push_line_to_history(&rl, str);
		}

		void execute() override;
	};
}

#endif