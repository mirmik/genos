#ifndef GENOS_CONSOLE_UTILITY_H
#define GENOS_CONSOLE_UTILITY_H

#include <console/SimpleShell.h>
#include <kernel/stdstream.h>

//TODO: добавить абстрактный класс Shell

namespace Genos {
	namespace utility { 
		int kill(int argc, char** argv);
		int killall(int argc, char** argv);
		int printSchedeeTable(int argc, char** argv);

		int addKernelUtility(SimpleShell& shell);
	}
}

#endif