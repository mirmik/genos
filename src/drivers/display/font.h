#ifndef GENOS_DRIVERS_DISPLAY_FONT_H
#define GENOS_DRIVERS_DISPLAY_FONT_H

#include <stdint.h>

struct font_head
{
	const uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */
};


#endif