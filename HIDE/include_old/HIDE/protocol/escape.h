#ifndef GENOS_PROTOCOL_ESCAPE_H
#define GENOS_PROTOCOL_ESCAPE_H

#include "inttypes.h"
#include "util/ascii.h"
#include "util/ascii_convert.h"
#include <sys/cdefs.h>
#include "ctype.h"

#define KEYCODE_STREAM 0
#define KEYCODE_ESC 1
#define KEYCODE_CSI 2
#define KEYCODE_CSI2 3

#define KEYCODE_CHAR 0
#define KEYCODE_CTRL 1

#define KEYCODE_NONE 0
#define KEYCODE_UP 1
#define KEYCODE_DOWN 2
#define KEYCODE_RIGHT 3
#define KEYCODE_LEFT 4
#define KEYCODE_BACKSPACE 5
#define KEYCODE_LINE 6

struct keycode
{
	uint8_t type;
	uint8_t code;
};

//struct csiseq
//{
//	uint8_t type;
//	uint8_t code;
//	int8_t arg0;
//	int8_t arg1;	
//};

struct keycode_data
{
	uint8_t mode;
	//struct keycode kcode;
};

__BEGIN_DECLS

inline static void keycode_data_init(struct keycode_data* data)
{
	data->mode = KEYCODE_STREAM;
	//kcode->type = 0;
	//kcode->code = 0;
};

inline static struct keycode keycode_constructor(uint8_t type, uint8_t code)
{
	struct keycode ret = {type, code};
	return ret;
}

inline static struct keycode keycode_none()
{
	struct keycode ret = {KEYCODE_CTRL, KEYCODE_NONE};
	return ret;
}

inline static struct keycode keycode_parse(struct keycode_data* data, uint8_t c)
{
	switch (data->mode)
	{ 
		case KEYCODE_STREAM:
			switch (c) 
			{
				case '\r': 
			 		return keycode_none();
				case '\b': 
			 		return keycode_constructor(KEYCODE_CTRL, KEYCODE_BACKSPACE);
				case '\n': 
			 		return keycode_constructor(KEYCODE_CTRL, KEYCODE_LINE);
				case ASCII_ESC: 
			 		data->mode = KEYCODE_ESC;
					return keycode_none();
				case ASCII_CSI: 
			 		data->mode = KEYCODE_CSI;
					return keycode_none();	
				default:
					return keycode_constructor(KEYCODE_CHAR, c);
			};
		break;

		case KEYCODE_ESC:
			switch (c) 
			{
				case '[': 
					data->mode = KEYCODE_CSI;
					return keycode_none();
				default:
					debug_print("notreg_esc_seq");
					data->mode = KEYCODE_STREAM;
					return keycode_none();
			}
		break;
		
		case KEYCODE_CSI:
			data->mode = KEYCODE_STREAM; //optimization
			switch (c) 
			{
				case CSI_CUU: 
					//data->mode = KEYCODE_STREAM;
					return keycode_constructor(KEYCODE_CTRL, KEYCODE_UP);
				case CSI_CUD: 
					//data->mode = KEYCODE_STREAM;
					return keycode_constructor(KEYCODE_CTRL, KEYCODE_DOWN);
				case CSI_CUF: 
					//data->mode = KEYCODE_STREAM;
					return keycode_constructor(KEYCODE_CTRL, KEYCODE_LEFT);
				case CSI_CUB: 
					//data->mode = KEYCODE_STREAM;
					return keycode_constructor(KEYCODE_CTRL, KEYCODE_RIGHT);
				default:
					debug_print("notreg_csi_seq");
					data->mode = KEYCODE_STREAM;
					return keycode_none();
			}
		break;
	};
};

inline static void debug_print_keycode(struct keycode kcode)
{
	if(kcode.type == KEYCODE_CHAR)
	{
		debug_putchar(kcode.code);
	}
	else
	{
		switch(kcode.code)
		{
			case KEYCODE_UP:
				debug_print("up_code");dln(); 
				break;
			case KEYCODE_DOWN:
				debug_print("down_code");dln();
				break;
			case KEYCODE_RIGHT:
				debug_print("right_code");dln();
				break;
			case KEYCODE_LEFT:
				debug_print("left_code");dln();
				break;
			case KEYCODE_NONE:
				debug_print("none_code");dln();
				break;
			default:
				debug_print("unregistred_code");dln();
				break;
		};
	};
}

/*inline static int csi_parse(
	struct keycode_data* data, uint8_t c)
{
	switch (data->mode)
	{ 
		case KEYCODE_STREAM:
			switch (c) 
			{
				case ASCII_ESC: 
			 		data->mode = KEYCODE_ESC;
					return 0;
				case ASCII_CSI: 
			 		data->mode = KEYCODE_CSI;
					return 0;	
				default:
					data->seq.type = KEYCODE_CHAR;
					data->seq.code = c;
					return 1;
			};
		break;

		case KEYCODE_ESC:
			switch (c) 
			{
				case '[': 
					data->mode = KEYCODE_CSI;
					data->seq.arg0 = 0;
					data->seq.arg1 = -1;
					return 0;
				default:
					debug_print("notreg_esc_seq");
					data->mode = KEYCODE_STREAM;
					return 0;
			}
		break;
		
		case KEYCODE_CSI:
			if (isdigit(c)) 
			{
				if (data->seq.arg1 == -1) { 
					keycode_data_argform(&data->seq.arg0,c);
					return 0;
				}
				else {
					keycode_data_argform(&data->seq.arg1,c);
					return 0;	
				};
			};
			switch (c) 
			{
				case ';': 
					data->seq.arg1 = 0;
					return 0;
				case CSI_CUU: 
					data->seq.code = KEYCODE_UP; 
					break;
				case CSI_CUD: 
					data->seq.code = KEYCODE_DOWN; 
					break;
				case CSI_CUF: 
					data->seq.code = KEYCODE_RIGHT; 
					break;
				case CSI_CUB: 
					data->seq.code = KEYCODE_LEFT; 
					break;
				default:
					debug_print("notreg_esc_seq");
					data->mode = KEYCODE_STREAM;
					return 0;
			}
			data->mode = KEYCODE_STREAM;
			return 1;
		break;
	};
	dprln("allbad");	
};*/


__END_DECLS
#endif