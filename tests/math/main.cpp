#include <hal/board.h>
#include <hal/irq.h>

#include <math.h>

#include <util/cpu_delay.h>

int main()
{
	double dret;
	long double dlret;

	board_init();

	dprln("\r\nMATH TEST:");
	dprln("\r\nmodf:");
	DPRINT(modf(10.12, &dret));
	DPRINT(dret);

	dprln("\r\nlog10:");
	DPRINT(log10(10.12));

	dprln("\r\nfmod:");
	DPRINT(fmod(10.12, 2));

	dprln("\r\npow:");
	DPRINT(pow(1.5, 2));

	dprln("\r\nfabs:");
	DPRINT(fabs(-10.12));

	dprln("\r\nlong:");
	dprln("\r\nmodfl:");
	DPRINT(modf(10.12, &dret));
	DPRINT(dlret);

	dprln("\r\nlog10l:");
	DPRINT(log10(10.12));

	dprln("\r\nfmodl:");
	DPRINT(fmod(10.12, 2));

	dprln("\r\npowl:");
	DPRINT(pow(1.5, 2));

	dprln("\r\nfabsl:");
	DPRINT(fabs(-10.12));



	while(1) 
	{
		cpu_delay(500000);
	}
}