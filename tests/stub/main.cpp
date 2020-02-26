#include <hal/board.h>

void foo() {}

int main()
{ 
	board_init(); 

	while(1)
	{
		foo();
	}
}