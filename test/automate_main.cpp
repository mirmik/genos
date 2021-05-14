#include <genos/adapter/autom_starter.h>
#include <igris/dprint.h>

void automate_main(int argc, char ** argv, int * state) 
{
	switch(*state) 
	{
		case 0:
			*state = 1;
			dprln("Hello");
			return;

		case 1:
			*state = 0;
			dprln("World");
			return;
	}
}

AUTOMATE_SCHEDEE_MAIN(automate_main)