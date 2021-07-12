#include <genos/resource/openres.h>
#include <genos/schedee.h>

void openres_init(struct openres * ores, struct file_head * file) 
{
	ores -> file = file;
}


int current_schedee_get_free_openres(struct openres ** ores) 
{
	struct schedee * sch = current_schedee();
}