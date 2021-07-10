#include <genos/adapter/autom_starter.h>

void autom_schedee_emulator(int argc, char ** argv, automfunc_t automfunc) 
{
	int state = 0;

	while(1) 
	{
		automfunc(argc, argv, &state);
	}
}