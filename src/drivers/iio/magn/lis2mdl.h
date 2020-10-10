#ifndef DRIVERS_LIS2MDL_H
#define DRIVERS_LIS2MDL_H

#include <drivers/regmap.h>

namespace genos 
{
	class lis2mdl : public regmap 
	{
		enum Register
		{
			// 0x00 - 0x44
			OFFSET_X_REG_L = 0x45,
			OFFSET_X_REG_H = 0x46,
			OFFSET_Y_REG_L = 0x47,
			OFFSET_Y_REG_H = 0x48,
			OFFSET_Z_REG_L = 0x49,
			OFFSET_Z_REG_H = 0x4A,
			// 0x4B - 0x4C
			WHO_AM_IR = 0x4F,
			//0x50 - 0x5F
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

	public:
		lis2mdl() 
		{  
			
		}
		
		lis2mdl(i2c_client * dev) : regmap(dev) {}
		lis2mdl(spi_client * dev) : regmap(dev) {}

		float mul[3] = {1,1,1};
		float add[3] = {0,0,0};

		void request_raw_magn(uint16_t * ret) 
		{
			uint8_t data[6];

			readreg_group(Register::OUTX_L_REG, data, 6);

			ret[0] = data[0] | (data[1] << 8);
			ret[1] = data[2] | (data[3] << 8);
			ret[2] = data[4] | (data[5] << 8);
		}

		void request_magn(float * ret) 
		{
			uint16_t data[3];

			request_raw_magn(data);

			for (int i = 0; i < 3; ++i)
				ret[i] = data[i] * mul[i] + add[i];
		}

		void set_discrete_mode() 
		{

		}

		void init_4pin() 
		{
			writereg(Register::CFG_REG_C, (1<<2) | (1<<5)); //I2S_DIS | 4WSPI
		}

		void print_status() 
		{
			unsigned int ret;
			readreg(Register::WHO_AM_IR, &ret);
			dprln("WHO_AM_IR:", ret);

			readreg(Register::STATUS_REG, &ret);
			dprln("STATUS_REG:", ret);
		}

	};
}

#endif