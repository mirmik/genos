#include <genos/ktimer.h>

void foo(void * priv, genos::ktimer * tim) 
{

}

genos::ktimer tim(foo, nullptr, 1000);

int main() 
{

	while(1) 
	{
		ktimer_manager_step();
	}
}