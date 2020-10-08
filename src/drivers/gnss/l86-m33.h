#ifndef GENOS_DRIVERS_GNSS_L86_M33_H
#define GENOS_DRIVERS_GNSS_L86_M33_H

#include <systime/systime.h>

namespace genos
{
	class l86_m33
	{
		genos::uartring * serial;

	public:
		l86_m33() {}
		l86_m33(genos::uartring * serial) :
			serial(serial)
		{}

		void begin(genos::uartring * serial)
		{
			this->serial = serial;
		}

		void naive_configure()
		{
			// Data rate
			serial->println("$PMTK220,250*29");
			delay(200);

			// Fix rate
			serial->println("PMTK300,250,0,0,0,0"); // without symbol $ ?
			delay(200);

			// GPS, GLONASS and GALLILEO
			serial->println("$PMTK353,1,1,0*2B");
			delay(200);

			// QZSS Disable
			serial->println("$PMTK352,0*2A");
			delay(200);

			// Setting module to show only GPGMC information
			serial->println("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29");
			delay(200);

			// Disabling display of GPTXT service information
			serial->println("$PQTXT,W,0,1*23");
			delay(200);
		}
	};
}

#endif