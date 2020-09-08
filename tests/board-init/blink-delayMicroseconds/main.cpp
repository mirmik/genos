#include <hal/board.h>
#include <hal/irq.h>

#include <systime/systime.h>
#include <asm/systime.h>

int main()
{
	board_init();

	irqs_enable();

	int64_t last = 0;

	DPRINT(systime_lomax());

	int i = 0;

	jiffies_pair_t lastpair;// = {10, 16001};
	//dprln(jiffies_pair_to_micros(pair));

	while(1) 
	{
		dprln(micros());
	}
}