#include <genos/resource/openres.h>
#include <genos/schedee.h>

void openres_init(struct openres * ores, struct file_head * file) 
{
	ores -> file = file;
}


int schedee_get_free_openres(struct schedee * sch, struct openres ** ores) 
{
	(void) sch; (void) ores;
	return 0;	
}