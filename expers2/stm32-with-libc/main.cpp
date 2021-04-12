#include <genos/ktimer.h>
#include <hal/board.h>

void foo(void * priv, genos::ktimer * tim) 
{

}

genos::ktimer tim(foo, nullptr, 1000);

int main() 
{
	board_init();
	blink_test_cpudelay(100000);

	while(1) 
	{
		ktimer_manager_step();
	}
}