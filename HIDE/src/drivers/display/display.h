#ifndef GENOS_DRIVERS_DISPLAY_H
#define GENOS_DRIVERS_DISPLAY_H

#include <stdint.h>
#include <string.h>

#include <drivers/display/font.h>

class display_device
{
public:
	virtual void draw_pixel(int16_t x, int16_t y, uint32_t color) = 0;
	virtual void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color);
	virtual void draw_circle(int16_t R, int16_t x, int16_t y, uint32_t color);

	void draw_char(int16_t x, int16_t y, unsigned char c,
	               uint16_t color, uint16_t bg, uint8_t size_x,
	               uint8_t size_y);

	void draw_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h,
	                    uint16_t color);

	int write(uint8_t c);


	virtual void fill(uint32_t color) = 0;
	void clear() { fill(0); }

	virtual void update() = 0;

	virtual int16_t width() = 0;
	virtual int16_t height() = 0;

	void print(const char* str)
	{
		int len = strlen(str);
		while (len--)
		{
			write(*str++);
		}
	}

	void display_draw_monochrome_bmp2(
	    uint8_t x0, uint8_t y0,
	    uint8_t height, uint8_t width,
	    long int * image);

	void display_draw_monochrome_bmp(
	    int x, int y,
	    int w, int h,
	    const uint8_t * arr,
	    uint16_t color, uint16_t bg,
	    int size_x, int size_y);

	void display_draw_battery(
	    int x, int y,
	    int w, int h,
	    const uint8_t * arr,
	    uint16_t color, uint16_t bg,
	    int size_x, int size_y, float procent);

	int16_t textsize_y = 2;
	int16_t textsize_x = 1;
	int textbgcolor = 0;
	int textcolor = 1;

	int cursor_x = 0;
	int cursor_y = 0;

	void set_cursor_position(int x, int y)
	{
		cursor_x = x;
		cursor_y = y;
	}

	void set_font_size(int x, int y)
	{
		textsize_x = x;
		textsize_y = y;
	}

	int wrap = 0;








	char draw_char2(char ch, struct font_head Font, uint32_t color);
	char print2(char* str, struct font_head Font, uint32_t color);
};
#endif