#ifndef RABBIT_FONT_NAIVE_H
#define RABBIT_FONT_NAIVE_H

#include <cstdint>

namespace rabbit
{
	class opengl_texture;

	extern const uint16_t Font6x8 [];
	extern const uint16_t Font7x10 [];
	extern const uint16_t Font11x18 [];
	extern const uint16_t Font16x26 [];

	opengl_texture naive_font16x26_texture(int no);
}

#endif