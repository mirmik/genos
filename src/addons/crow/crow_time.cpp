#include <crow/tower.h>
#include <systime/systime.h>

uint16_t crow::millis() {
	return (uint16_t) ::millis();
}
