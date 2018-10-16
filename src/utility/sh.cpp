#include <utility/sh.h>
#include <sched/api.h>
#include <mvfs/file.h>

#include <unistd.h>
#include <drivers/cdev/cdev.h>

void * sh_utility(void * arg, int * state) {
	switch(*state) {
		case 0:
		dprln("Mirmik");
		msleep(500);
		*state = 2;
		break;
	
		case 2:
		schedee_debug_print_fds(current_schedee());
		static_cast<struct char_device*>(current_schedee()->fds[0]->node)->write("HelloWorld\r\n", 12);
		(*state)++;
		break;
	
		case 3:
		/*dprln("Mirmik");
		msleep(500);
		(*state)++;
		break;
	*/
		case 4:	
		schedee_exit();
	}
}