#ifndef GENOS_UTILITY_CONTTY_H
#define GENOS_UTILITY_CONTTY_H

#include <sys/cdefs.h>
#include <igris/shell/readline.h>

#include <genos/schedee/autom.h>
#include <genos/executor.h>
#include <genos/nav.h>

#define CONTTY_LINE_LENGTH 32
#define CONTTY_HISTORY_SIZE 3

namespace genos
{
	class contty : public genos::autom_schedee_base
	{
		struct readline rl;

		node * outside;
		executor * ex;

		char last;

		char buffer[CONTTY_LINE_LENGTH];
		char hbuffer[CONTTY_LINE_LENGTH * CONTTY_HISTORY_SIZE];

		void newline();

	public:
		uint8_t debug_mode = 0;

		contty(genos::node * outside, executor* ex) 
			: outside(outside), ex(ex)
		{}

		void execute() override;
	};
}

#endif