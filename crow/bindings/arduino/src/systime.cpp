#include <crow/tower.h>
#include <igris/systime.h>

uint16_t crow::millis() 
{
	return systime_millis();
}