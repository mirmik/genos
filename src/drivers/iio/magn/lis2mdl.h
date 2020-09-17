#ifndef DRIVERS_LIS2MDL_H
#define DRIVERS_LIS2MDL_H

#include <drivers/regmap.h>

namespace genos 
{
	class lis2mdl : public regmap 
	{
		enum class Register : uint8_t 
		{
			// 0x00 - 0x44
			OFFSET_X_REG_L = 0x45,
			OFFSET_X_REG_H = 0x46,
			OFFSET_Y_REG_L = 0x47,
			OFFSET_Y_REG_H = 0x48,
			OFFSET_Z_REG_H = 0x49,
			OFFSET_Z_REG_H = 0x4A,
			// 0x4B - 0x4C
			WHO_AM_IR = 0x4F,
			//0x50-0x5FR
			CFG_REG_A = 0x60,
			CFG_REG_B = 0x61,
			CFG_REG_C = 0x62,
			INT_CRTL_REG = 0x63,
			INT_SOURCE_REG = 0x64,
			INT_THS_L_REG = 0x65,
			INT_THS_H_REG = 0x66,
			STATUS_REG = 0x67,
			OUTX_L_REG = 0x68,
			OUTX_H_REG = 0x69,
			OUTY_L_REG = 0x6A,
			OUTY_H_REG = 0x6B,
			OUTZ_L_REG = 0x6C,
			OUTZ_H_REG = 0x6D,
			TEMP_OUT_L_REG = 0x6E,
			TEMP_OUT_H_REG = 0x6F	
		};
	};
}

#endif