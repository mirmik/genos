#ifndef DRIVERS_ST_LSM6DSX_H
#define DRIVERS_ST_LSM6DSX_H

#include <drivers/regmap.h>

namespace genos
{
	class st_lsm6dx : public genos::regmap
	{
	public:

		enum class Register : uint8_t {
			FUNC_CFG_ACCESS  = 0x01,
			PIN_CTRL         = 0x02,
			// RESERVED :      0x03
			S4S_TPH_L        = 0x04,
			S4S_TPH_H        = 0x05,
			S4S_RR           = 0x06,
			FIFO_CTRL1       = 0x07,
			FIFO_CTRL2       = 0x08,
			FIFO_CTRL3       = 0x09,
			FIFO_CTRL4       = 0x0A,
			COUNTER_BDR_REG1 = 0x0B,
			COUNTER_BDR_REG2 = 0x0C,
			INT1_CTRL        = 0x0D,
			INT2_CTRL        = 0x0E,
			WHO_AM_I         = 0x0F,
			CTRL1_XL         = 0x10,
			CTRL2_G          = 0x11,
			CTRL3_C          = 0x12,
			CTRL4_C          = 0x13,
			CTRL5_C          = 0x14,
			CTRL6_C          = 0x15,
			CTRL7_G          = 0x16,
			CTRL8_XL         = 0x17,
			CTRL9_XL         = 0x18,
			CTRL10_C         = 0x19,
			ALL_INT_SRC      = 0x1A,
			WAKE_UP_SRC      = 0x1B,
			TAP_SRC          = 0x1C,
			D6D_SRC          = 0x1D,
			STATUS_REG       = 0x1E,
			// RESERVED :      0x1F
			OUT_TEMP_L       = 0x20,
			OUT_TEMP_H       = 0x21,
			OUTX_L_G         = 0x22,
			OUTX_H_G         = 0x23,
			OUTY_L_G         = 0x24,
			OUTY_H_G         = 0x25,
			OUTZ_L_G         = 0x26,
			OUTZ_H_G         = 0x27,
			OUTX_L_A         = 0x28,
			OUTX_H_A         = 0x29,
			OUTY_L_A         = 0x2A,
			OUTY_H_A         = 0x2B,
			OUTZ_L_A         = 0x2C,
			OUTZ_H_A         = 0x2D,
			// RESERVED : 0x2E-0x34
			EMB_FUNC_STATUS_MAINPAGE = 0x35,
			FSM_STATUS_A_MAINPAGE    = 0x36,
			FSM_STATUS_B_MAINPAGE    = 0x37,
			MLC_STATUS_MAINPAGE      = 0x38,
			STATUS_MASTER_MAINPAGE   = 0x39,
			FIFO_STATUS1             = 0x3A,
			FIFO_STATUS2             = 0x3B,
			// RESERVED :       0x3C - 0x3F
			TIMESTAMP0               = 0x40,
			TIMESTAMP1               = 0x41,
			TIMESTAMP2               = 0x42,
			TIMESTAMP3               = 0x43,
			// RESERVED :       0x44 - 0x55
			TAP_CFG0                 = 0x56,
			TAP_CFG1                 = 0x57,
			TAP_CFG2                 = 0x58,
			TAP_THS_6D               = 0x59,
			INT_DUR2                 = 0x5A,
			WAKE_UP_THS              = 0x5B,
			WAKE_UP_DUR              = 0x5C,
			FREE_FALL                = 0x5D,
			MD1_CFG                  = 0x5E,
			MD2_CFG                  = 0x5F,
			S4S_ST_CMD_CODE          = 0x60,
			S4S_DT_REG               = 0x61,
			I3C_BUS_AVB              = 0x62,
			INTERNAL_FREQ_FINE       = 0x63,
			// RESERVED :       0x64 - 0x6E
			INT_OIS                  = 0x6F
			CTRL1_OIS                = 0x70,
			CTRL2_OIS                = 0x71,
			CTRL3_OIS                = 0x72,
			X_OFS_USR                = 0x73,
			Y_OFS_USR                = 0x74,
			Z_OFS_USR                = 0x75,
			// RESERVED :       0x76 - 0x77
			FIFO_DATA_OUT_TAG        = 0x78,
			FIFO_DATA_OUT_X_L        = 0x79,
			FIFO_DATA_OUT_X_H        = 0x7A,
			FIFO_DATA_OUT_Y_L        = 0x7B,
			FIFO_DATA_OUT_Y_H        = 0x7C,
			FIFO_DATA_OUT_Z_L        = 0x7D,
			FIFO_DATA_OUT_Z_H        = 0x7E	
		};

		st_lsm6dx(i2c_client * dev) : regmap(dev) {}
		st_lsm6dx(spi_client * dev) : regmap(dev) {}

		int request_raw_accel(uint16_t * vec) 
		{
			uint8_t data[6];
			data[0] = OUTX_L_A;

			read(data, 6);

			vec[0] = (data[1] << 8) | data[0];  
			vec[1] = (data[3] << 8) | data[2];
			vec[2] = (data[5] << 8) | data[4];

			return 0;
		}

		int request_raw_gyro(uint16_t * vec) 
		{
			uint8_t data[6];
			data[0] = OUTX_L_G;

			read(data, 6);

			vec[0] = (data[1] << 8) | data[0];  
			vec[1] = (data[3] << 8) | data[2];
			vec[2] = (data[5] << 8) | data[4];

			return 0;
		}
	};
}

#endif