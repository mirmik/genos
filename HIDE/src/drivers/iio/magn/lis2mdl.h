#ifndef DRIVERS_LIS2MDL_H
#define DRIVERS_LIS2MDL_H

#include <igris/dprint.h>

#include <drivers/regmap.h>
#include <drivers/iio/magn/lis2mdl_map.h>

class lis2mdl_device : public genos::regmap
{
public:
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
	lis2mdl_device()
	{

	}

	lis2mdl_device(genos::i2c_client * dev) : regmap(dev) {}
	lis2mdl_device(genos::spi_client * dev) : regmap(dev) {}

	float mul[3] = {1, 1, 1};
	float add[3] = {0, 0, 0};

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
		writereg(Register::CFG_REG_C, (1 << 2) | (1 << 5)); //I2S_DIS | 4WSPI
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

typedef lis2mdl_device lis2mdl_ctx_t;
typedef float float_t;

extern float_t lis2mdl_from_lsb_to_mgauss(int16_t lsb);
extern float_t lis2mdl_from_lsb_to_celsius(int16_t lsb);

int32_t lis2mdl_mag_user_offset_set(lis2mdl_ctx_t *ctx, uint8_t *buff);
int32_t lis2mdl_mag_user_offset_get(lis2mdl_ctx_t *ctx, uint8_t *buff);

typedef enum {
  LIS2MDL_CONTINUOUS_MODE  = 0,
  LIS2MDL_SINGLE_TRIGGER   = 1,
  LIS2MDL_POWER_DOWN       = 2,
} lis2mdl_md_t;
int32_t lis2mdl_operating_mode_set(lis2mdl_ctx_t *ctx, lis2mdl_md_t val);
int32_t lis2mdl_operating_mode_get(lis2mdl_ctx_t *ctx, lis2mdl_md_t *val);

typedef enum {
  LIS2MDL_ODR_10Hz   = 0,
  LIS2MDL_ODR_20Hz   = 1,
  LIS2MDL_ODR_50Hz   = 2,
  LIS2MDL_ODR_100Hz  = 3,
} lis2mdl_odr_t;
int32_t lis2mdl_data_rate_set(lis2mdl_ctx_t *ctx, lis2mdl_odr_t val);
int32_t lis2mdl_data_rate_get(lis2mdl_ctx_t *ctx, lis2mdl_odr_t *val);

typedef enum {
  LIS2MDL_HIGH_RESOLUTION  = 0,
  LIS2MDL_LOW_POWER        = 1,
} lis2mdl_lp_t;
int32_t lis2mdl_power_mode_set(lis2mdl_ctx_t *ctx, lis2mdl_lp_t val);
int32_t lis2mdl_power_mode_get(lis2mdl_ctx_t *ctx, lis2mdl_lp_t *val);

int32_t lis2mdl_offset_temp_comp_set(lis2mdl_ctx_t *ctx, uint8_t val);
int32_t lis2mdl_offset_temp_comp_get(lis2mdl_ctx_t *ctx, uint8_t *val);

typedef enum {
  LIS2MDL_ODR_DIV_2  = 0,
  LIS2MDL_ODR_DIV_4  = 1,
} lis2mdl_lpf_t;
int32_t lis2mdl_low_pass_bandwidth_set(lis2mdl_ctx_t *ctx,
                                       lis2mdl_lpf_t val);
int32_t lis2mdl_low_pass_bandwidth_get(lis2mdl_ctx_t *ctx,
                                       lis2mdl_lpf_t *val);

typedef enum {
  LIS2MDL_SET_SENS_ODR_DIV_63        = 0,
  LIS2MDL_SENS_OFF_CANC_EVERY_ODR    = 1,
  LIS2MDL_SET_SENS_ONLY_AT_POWER_ON  = 2,
} lis2mdl_set_rst_t;
int32_t lis2mdl_set_rst_mode_set(lis2mdl_ctx_t *ctx,
                                 lis2mdl_set_rst_t val);
int32_t lis2mdl_set_rst_mode_get(lis2mdl_ctx_t *ctx,
                                 lis2mdl_set_rst_t *val);

int32_t lis2mdl_set_rst_sensor_single_set(lis2mdl_ctx_t *ctx,
                                          uint8_t val);
int32_t lis2mdl_set_rst_sensor_single_get(lis2mdl_ctx_t *ctx,
                                          uint8_t *val);

int32_t lis2mdl_block_data_update_set(lis2mdl_ctx_t *ctx, uint8_t val);
int32_t lis2mdl_block_data_update_get(lis2mdl_ctx_t *ctx, uint8_t *val);

int32_t lis2mdl_mag_data_ready_get(lis2mdl_ctx_t *ctx, uint8_t *val);

int32_t lis2mdl_mag_data_ovr_get(lis2mdl_ctx_t *ctx, uint8_t *val);

int32_t lis2mdl_magnetic_raw_get(lis2mdl_ctx_t *ctx, uint8_t *buff);

int32_t lis2mdl_temperature_raw_get(lis2mdl_ctx_t *ctx, uint8_t *buff);

int32_t lis2mdl_device_id_get(lis2mdl_ctx_t *ctx, uint8_t *buff);

int32_t lis2mdl_reset_set(lis2mdl_ctx_t *ctx, uint8_t val);
int32_t lis2mdl_reset_get(lis2mdl_ctx_t *ctx, uint8_t *val);

int32_t lis2mdl_boot_set(lis2mdl_ctx_t *ctx, uint8_t val);
int32_t lis2mdl_boot_get(lis2mdl_ctx_t *ctx, uint8_t *val);

int32_t lis2mdl_self_test_set(lis2mdl_ctx_t *ctx, uint8_t val);
int32_t lis2mdl_self_test_get(lis2mdl_ctx_t *ctx, uint8_t *val);

typedef enum {
  LIS2MDL_LSB_AT_LOW_ADD  = 0,
  LIS2MDL_MSB_AT_LOW_ADD  = 1,
} lis2mdl_ble_t;
int32_t lis2mdl_data_format_set(lis2mdl_ctx_t *ctx, lis2mdl_ble_t val);
int32_t lis2mdl_data_format_get(lis2mdl_ctx_t *ctx, lis2mdl_ble_t *val);

int32_t lis2mdl_status_get(lis2mdl_ctx_t *ctx, lis2mdl_status_reg_t *val);

typedef enum {
  LIS2MDL_CHECK_BEFORE  = 0,
  LIS2MDL_CHECK_AFTER   = 1,
} lis2mdl_int_on_dataoff_t;
int32_t lis2mdl_offset_int_conf_set(lis2mdl_ctx_t *ctx,
                                    lis2mdl_int_on_dataoff_t val);
int32_t lis2mdl_offset_int_conf_get(lis2mdl_ctx_t *ctx,
                                    lis2mdl_int_on_dataoff_t *val);

int32_t lis2mdl_drdy_on_pin_set(lis2mdl_ctx_t *ctx, uint8_t val);
int32_t lis2mdl_drdy_on_pin_get(lis2mdl_ctx_t *ctx, uint8_t *val);

int32_t lis2mdl_int_on_pin_set(lis2mdl_ctx_t *ctx, uint8_t val);
int32_t lis2mdl_int_on_pin_get(lis2mdl_ctx_t *ctx, uint8_t *val);

int32_t lis2mdl_int_gen_conf_set(lis2mdl_ctx_t *ctx,
                                 lis2mdl_int_crtl_reg_t *val);
int32_t lis2mdl_int_gen_conf_get(lis2mdl_ctx_t *ctx,
                                 lis2mdl_int_crtl_reg_t *val);

int32_t lis2mdl_int_gen_source_get(lis2mdl_ctx_t *ctx,
                                   lis2mdl_int_source_reg_t *val);

int32_t lis2mdl_int_gen_treshold_set(lis2mdl_ctx_t *ctx, uint8_t *buff);
int32_t lis2mdl_int_gen_treshold_get(lis2mdl_ctx_t *ctx, uint8_t *buff);

typedef enum {
  LIS2MDL_I2C_ENABLE   = 0,
  LIS2MDL_I2C_DISABLE  = 1,
} lis2mdl_i2c_dis_t;
int32_t lis2mdl_i2c_interface_set(lis2mdl_ctx_t *ctx,
                                  lis2mdl_i2c_dis_t val);
int32_t lis2mdl_i2c_interface_get(lis2mdl_ctx_t *ctx,
                                  lis2mdl_i2c_dis_t *val);


#endif