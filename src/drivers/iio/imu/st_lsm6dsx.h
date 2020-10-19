#ifndef DRIVERS_ST_LSM6DSX_H
#define DRIVERS_ST_LSM6DSX_H

#include <igris/dprint.h>

#include <drivers/regmap.h>
#include <drivers/iio/imu/lsm6dso_map.h>

namespace genos
{
	class st_lsm6dx : public genos::regmap
	{
	public:

		enum Register {
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
			INT_OIS                  = 0x6F,
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

		st_lsm6dx() {}
		st_lsm6dx(i2c_client * dev) : regmap(dev) {}
		st_lsm6dx(spi_client * dev) : regmap(dev) {}

		int request_raw_data(
			uint16_t* acc, 
			uint16_t* gyr) 
		{
			uint8_t data[12];
			
			readreg_group(Register::OUTX_L_G, data, 12);

			gyr[0] = (data[1] << 8) | data[0];  
			gyr[1] = (data[3] << 8) | data[2];
			gyr[2] = (data[5] << 8) | data[4];

			acc[0] = (data[7]  << 8) | data[6];  
			acc[1] = (data[9]  << 8) | data[8];
			acc[2] = (data[11] << 8) | data[10];

			return 0;
		}

		int request_raw_accel(uint16_t * vec) 
		{
			uint8_t data[6];

			readreg_group(Register::OUTX_L_A, data, 6);

			vec[0] = (data[1] << 8) | data[0];  
			vec[1] = (data[3] << 8) | data[2];
			vec[2] = (data[5] << 8) | data[4];

			return 0;
		}

		int request_raw_gyro(uint16_t * vec) 
		{
			uint8_t data[6];
			
			readreg_group(Register::OUTX_L_G, data, 6);

			vec[0] = (data[1] << 8) | data[0];  
			vec[1] = (data[3] << 8) | data[2];
			vec[2] = (data[5] << 8) | data[4];

			return 0;
		}

		void print_status() 
		{
			
			unsigned int ret;
			readreg(Register::WHO_AM_I, &ret);
			dprln("WHO_AM_IR:", ret);
			dprbinln(ret);

			readreg(Register::STATUS_REG, &ret);
			dprln("STATUS_REG:", ret);
		}
	};
}

typedef genos::st_lsm6dx lsm6dso_ctx_t;

int32_t lsm6dso_read_reg(lsm6dso_ctx_t *ctx, uint8_t reg, uint8_t* data,
                         uint16_t len);
int32_t lsm6dso_write_reg(lsm6dso_ctx_t *ctx, uint8_t reg, uint8_t* data,
                          uint16_t len);

extern float lsm6dso_from_fs2_to_mg(int16_t lsb);
extern float lsm6dso_from_fs4_to_mg(int16_t lsb);
extern float lsm6dso_from_fs8_to_mg(int16_t lsb);
extern float lsm6dso_from_fs16_to_mg(int16_t lsb);
extern float lsm6dso_from_fs125_to_mdps(int16_t lsb);
extern float lsm6dso_from_fs500_to_mdps(int16_t lsb);
extern float lsm6dso_from_fs250_to_mdps(int16_t lsb);
extern float lsm6dso_from_fs1000_to_mdps(int16_t lsb);
extern float lsm6dso_from_fs2000_to_mdps(int16_t lsb);
extern float lsm6dso_from_lsb_to_celsius(int16_t lsb);
extern float lsm6dso_from_lsb_to_nsec(int16_t lsb);

typedef enum {
  LSM6DSO_2g   = 0,
  LSM6DSO_16g  = 1, /* if XL_FS_MODE = ‘1’ -> LSM6DSO_2g */
  LSM6DSO_4g   = 2,
  LSM6DSO_8g   = 3,
} lsm6dso_fs_xl_t;
int32_t lsm6dso_xl_full_scale_set(lsm6dso_ctx_t *ctx, lsm6dso_fs_xl_t val);
int32_t lsm6dso_xl_full_scale_get(lsm6dso_ctx_t *ctx, lsm6dso_fs_xl_t *val);

typedef enum {
  LSM6DSO_XL_ODR_OFF    = 0,
  LSM6DSO_XL_ODR_12Hz5  = 1,
  LSM6DSO_XL_ODR_26Hz   = 2,
  LSM6DSO_XL_ODR_52Hz   = 3,
  LSM6DSO_XL_ODR_104Hz  = 4,
  LSM6DSO_XL_ODR_208Hz  = 5,
  LSM6DSO_XL_ODR_417Hz  = 6,
  LSM6DSO_XL_ODR_833Hz  = 7,
  LSM6DSO_XL_ODR_1667Hz = 8,
  LSM6DSO_XL_ODR_3333Hz = 9,
  LSM6DSO_XL_ODR_6667Hz = 10,
  LSM6DSO_XL_ODR_6Hz5   = 11, /* (low power only) */
} lsm6dso_odr_xl_t;
int32_t lsm6dso_xl_data_rate_set(lsm6dso_ctx_t *ctx, lsm6dso_odr_xl_t val);
int32_t lsm6dso_xl_data_rate_get(lsm6dso_ctx_t *ctx, lsm6dso_odr_xl_t *val);

typedef enum {
  LSM6DSO_250dps   = 0,
  LSM6DSO_125dps   = 1,
  LSM6DSO_500dps   = 2,
  LSM6DSO_1000dps  = 4,
  LSM6DSO_2000dps  = 6,
} lsm6dso_fs_g_t;
int32_t lsm6dso_gy_full_scale_set(lsm6dso_ctx_t *ctx, lsm6dso_fs_g_t val);
int32_t lsm6dso_gy_full_scale_get(lsm6dso_ctx_t *ctx, lsm6dso_fs_g_t *val);

typedef enum {
  LSM6DSO_GY_ODR_OFF    = 0,
  LSM6DSO_GY_ODR_12Hz5  = 1,
  LSM6DSO_GY_ODR_26Hz   = 2,
  LSM6DSO_GY_ODR_52Hz   = 3,
  LSM6DSO_GY_ODR_104Hz  = 4,
  LSM6DSO_GY_ODR_208Hz  = 5,
  LSM6DSO_GY_ODR_417Hz  = 6,
  LSM6DSO_GY_ODR_833Hz  = 7,
  LSM6DSO_GY_ODR_1667Hz = 8,
  LSM6DSO_GY_ODR_3333Hz = 9,
  LSM6DSO_GY_ODR_6667Hz = 10,
} lsm6dso_odr_g_t;
int32_t lsm6dso_gy_data_rate_set(lsm6dso_ctx_t *ctx, lsm6dso_odr_g_t val);
int32_t lsm6dso_gy_data_rate_get(lsm6dso_ctx_t *ctx, lsm6dso_odr_g_t *val);

int32_t lsm6dso_block_data_update_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_block_data_update_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_LSb_1mg  = 0,
  LSM6DSO_LSb_16mg = 1,
} lsm6dso_usr_off_w_t;
int32_t lsm6dso_xl_offset_weight_set(lsm6dso_ctx_t *ctx,
                                     lsm6dso_usr_off_w_t val);
int32_t lsm6dso_xl_offset_weight_get(lsm6dso_ctx_t *ctx,
                                     lsm6dso_usr_off_w_t *val);

typedef enum {
  LSM6DSO_HIGH_PERFORMANCE_MD  = 0,
  LSM6DSO_LOW_NORMAL_POWER_MD  = 1,
  LSM6DSO_ULTRA_LOW_POWER_MD   = 2,
} lsm6dso_xl_hm_mode_t;
int32_t lsm6dso_xl_power_mode_set(lsm6dso_ctx_t *ctx,
                                  lsm6dso_xl_hm_mode_t val);
int32_t lsm6dso_xl_power_mode_get(lsm6dso_ctx_t *ctx,
                                  lsm6dso_xl_hm_mode_t *val);

typedef enum {
  LSM6DSO_GY_HIGH_PERFORMANCE  = 0,
  LSM6DSO_GY_NORMAL            = 1,
} lsm6dso_g_hm_mode_t;
int32_t lsm6dso_gy_power_mode_set(lsm6dso_ctx_t *ctx,
                                  lsm6dso_g_hm_mode_t val);
int32_t lsm6dso_gy_power_mode_get(lsm6dso_ctx_t *ctx,
                                  lsm6dso_g_hm_mode_t *val);

typedef struct {
  lsm6dso_all_int_src_t       all_int_src;
  lsm6dso_wake_up_src_t       wake_up_src;
  lsm6dso_tap_src_t           tap_src;
  lsm6dso_d6d_src_t           d6d_src;
  lsm6dso_status_reg_t        status_reg;
  lsm6dso_emb_func_status_t   emb_func_status;
  lsm6dso_fsm_status_a_t      fsm_status_a;
  lsm6dso_fsm_status_b_t      fsm_status_b;
} lsm6dso_all_sources_t;
int32_t lsm6dso_all_sources_get(lsm6dso_ctx_t *ctx,
                                lsm6dso_all_sources_t *val);

int32_t lsm6dso_status_reg_get(lsm6dso_ctx_t *ctx,
                               lsm6dso_status_reg_t *val);

int32_t lsm6dso_xl_flag_data_ready_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_gy_flag_data_ready_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_temp_flag_data_ready_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_xl_usr_offset_x_set(lsm6dso_ctx_t *ctx, uint8_t *buff);
int32_t lsm6dso_xl_usr_offset_x_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

int32_t lsm6dso_xl_usr_offset_y_set(lsm6dso_ctx_t *ctx, uint8_t *buff);
int32_t lsm6dso_xl_usr_offset_y_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

int32_t lsm6dso_xl_usr_offset_z_set(lsm6dso_ctx_t *ctx, uint8_t *buff);
int32_t lsm6dso_xl_usr_offset_z_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

int32_t lsm6dso_xl_usr_offset_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_xl_usr_offset_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_timestamp_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_timestamp_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_timestamp_raw_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

typedef enum {
  LSM6DSO_NO_ROUND      = 0,
  LSM6DSO_ROUND_XL      = 1,
  LSM6DSO_ROUND_GY      = 2,
  LSM6DSO_ROUND_GY_XL   = 3,
} lsm6dso_rounding_t;
int32_t lsm6dso_rounding_mode_set(lsm6dso_ctx_t *ctx,
                                    lsm6dso_rounding_t val);
int32_t lsm6dso_rounding_mode_get(lsm6dso_ctx_t *ctx,
                                    lsm6dso_rounding_t *val);

int32_t lsm6dso_temperature_raw_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

int32_t lsm6dso_angular_rate_raw_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

int32_t lsm6dso_acceleration_raw_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

int32_t lsm6dso_fifo_out_raw_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

int32_t lsm6dso_number_of_steps_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

int32_t lsm6dso_steps_reset(lsm6dso_ctx_t *ctx);

int32_t lsm6dso_odr_cal_reg_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_odr_cal_reg_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_USER_BANK           = 0,
  LSM6DSO_SENSOR_HUB_BANK     = 1,
  LSM6DSO_EMBEDDED_FUNC_BANK  = 2,
} lsm6dso_reg_access_t;
int32_t lsm6dso_mem_bank_set(lsm6dso_ctx_t *ctx, lsm6dso_reg_access_t val);
int32_t lsm6dso_mem_bank_get(lsm6dso_ctx_t *ctx, lsm6dso_reg_access_t *val);

int32_t lsm6dso_ln_pg_write_byte(lsm6dso_ctx_t *ctx, uint16_t address,
                                 uint8_t *val);
int32_t lsm6dso_ln_pg_read_byte(lsm6dso_ctx_t *ctx, uint16_t address,
                                uint8_t *val);
int32_t lsm6dso_ln_pg_write(lsm6dso_ctx_t *ctx, uint16_t address,
                            uint8_t *buf, uint8_t len);
int32_t lsm6dso_ln_pg_read(lsm6dso_ctx_t *ctx, uint16_t address,
                           uint8_t *val);

typedef enum {
  LSM6DSO_DRDY_LATCHED = 0,
  LSM6DSO_DRDY_PULSED  = 1,
} lsm6dso_dataready_pulsed_t;
int32_t lsm6dso_data_ready_mode_set(lsm6dso_ctx_t *ctx,
                                    lsm6dso_dataready_pulsed_t val);
int32_t lsm6dso_data_ready_mode_get(lsm6dso_ctx_t *ctx,
                                    lsm6dso_dataready_pulsed_t *val);

int32_t lsm6dso_device_id_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

int32_t lsm6dso_reset_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_reset_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_auto_increment_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_auto_increment_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_boot_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_boot_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_XL_ST_DISABLE  = 0,
  LSM6DSO_XL_ST_POSITIVE = 1,
  LSM6DSO_XL_ST_NEGATIVE = 2,
} lsm6dso_st_xl_t;
int32_t lsm6dso_xl_self_test_set(lsm6dso_ctx_t *ctx, lsm6dso_st_xl_t val);
int32_t lsm6dso_xl_self_test_get(lsm6dso_ctx_t *ctx, lsm6dso_st_xl_t *val);

typedef enum {
  LSM6DSO_GY_ST_DISABLE  = 0,
  LSM6DSO_GY_ST_POSITIVE = 1,
  LSM6DSO_GY_ST_NEGATIVE = 3,
} lsm6dso_st_g_t;
int32_t lsm6dso_gy_self_test_set(lsm6dso_ctx_t *ctx, lsm6dso_st_g_t val);
int32_t lsm6dso_gy_self_test_get(lsm6dso_ctx_t *ctx, lsm6dso_st_g_t *val);

int32_t lsm6dso_xl_filter_lp2_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_xl_filter_lp2_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_gy_filter_lp1_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_gy_filter_lp1_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_filter_settling_mask_set(lsm6dso_ctx_t *ctx,
                                         uint8_t val);
int32_t lsm6dso_filter_settling_mask_get(lsm6dso_ctx_t *ctx,
                                         uint8_t *val);

typedef enum {
  LSM6DSO_ULTRA_LIGHT  = 0,
  LSM6DSO_VERY_LIGHT   = 1,
  LSM6DSO_LIGHT        = 2,
  LSM6DSO_MEDIUM       = 3,
  LSM6DSO_STRONG       = 4, /* not available for data rate > 1k670Hz */
  LSM6DSO_VERY_STRONG  = 5, /* not available for data rate > 1k670Hz */
  LSM6DSO_AGGRESSIVE   = 6, /* not available for data rate > 1k670Hz */
  LSM6DSO_XTREME       = 7, /* not available for data rate > 1k670Hz */
} lsm6dso_ftype_t;
int32_t lsm6dso_gy_lp1_bandwidth_set(lsm6dso_ctx_t *ctx,
                                     lsm6dso_ftype_t val);
int32_t lsm6dso_gy_lp1_bandwidth_get(lsm6dso_ctx_t *ctx,
                                     lsm6dso_ftype_t *val);

int32_t lsm6dso_xl_lp2_on_6d_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_xl_lp2_on_6d_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_HP_PATH_DISABLE_ON_OUT    = 0x00,
  LSM6DSO_SLOPE_ODR_DIV_4           = 0x10,
  LSM6DSO_HP_ODR_DIV_10             = 0x11,
  LSM6DSO_HP_ODR_DIV_20             = 0x12,
  LSM6DSO_HP_ODR_DIV_45             = 0x13,
  LSM6DSO_HP_ODR_DIV_100            = 0x14,
  LSM6DSO_HP_ODR_DIV_200            = 0x15,
  LSM6DSO_HP_ODR_DIV_400            = 0x16,
  LSM6DSO_HP_ODR_DIV_800            = 0x17,
  LSM6DSO_HP_REF_MD_ODR_DIV_10      = 0x31,
  LSM6DSO_HP_REF_MD_ODR_DIV_20      = 0x32,
  LSM6DSO_HP_REF_MD_ODR_DIV_45      = 0x33,
  LSM6DSO_HP_REF_MD_ODR_DIV_100     = 0x34,
  LSM6DSO_HP_REF_MD_ODR_DIV_200     = 0x35,
  LSM6DSO_HP_REF_MD_ODR_DIV_400     = 0x36,
  LSM6DSO_HP_REF_MD_ODR_DIV_800     = 0x37,
  LSM6DSO_LP_ODR_DIV_10             = 0x01,
  LSM6DSO_LP_ODR_DIV_20             = 0x02,
  LSM6DSO_LP_ODR_DIV_45             = 0x03,
  LSM6DSO_LP_ODR_DIV_100            = 0x04,
  LSM6DSO_LP_ODR_DIV_200            = 0x05,
  LSM6DSO_LP_ODR_DIV_400            = 0x06,
  LSM6DSO_LP_ODR_DIV_800            = 0x07,
} lsm6dso_hp_slope_xl_en_t;
int32_t lsm6dso_xl_hp_path_on_out_set(lsm6dso_ctx_t *ctx,
                                      lsm6dso_hp_slope_xl_en_t val);
int32_t lsm6dso_xl_hp_path_on_out_get(lsm6dso_ctx_t *ctx,
                                      lsm6dso_hp_slope_xl_en_t *val);

int32_t lsm6dso_xl_fast_settling_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_xl_fast_settling_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_USE_SLOPE = 0,
  LSM6DSO_USE_HPF   = 1,
} lsm6dso_slope_fds_t;
int32_t lsm6dso_xl_hp_path_internal_set(lsm6dso_ctx_t *ctx,
                                         lsm6dso_slope_fds_t val);
int32_t lsm6dso_xl_hp_path_internal_get(lsm6dso_ctx_t *ctx,
                                         lsm6dso_slope_fds_t *val);

typedef enum {
  LSM6DSO_HP_FILTER_NONE     = 0x00,
  LSM6DSO_HP_FILTER_16mHz    = 0x80,
  LSM6DSO_HP_FILTER_65mHz    = 0x81,
  LSM6DSO_HP_FILTER_260mHz   = 0x82,
  LSM6DSO_HP_FILTER_1Hz04    = 0x83,
} lsm6dso_hpm_g_t;
int32_t lsm6dso_gy_hp_path_internal_set(lsm6dso_ctx_t *ctx,
                                         lsm6dso_hpm_g_t val);
int32_t lsm6dso_gy_hp_path_internal_get(lsm6dso_ctx_t *ctx,
                                         lsm6dso_hpm_g_t *val);

typedef enum {
  LSM6DSO_AUX_PULL_UP_DISC       = 0,
  LSM6DSO_AUX_PULL_UP_CONNECT    = 1,
} lsm6dso_ois_pu_dis_t;
int32_t lsm6dso_aux_sdo_ocs_mode_set(lsm6dso_ctx_t *ctx,
                                     lsm6dso_ois_pu_dis_t val);
int32_t lsm6dso_aux_sdo_ocs_mode_get(lsm6dso_ctx_t *ctx,
                                     lsm6dso_ois_pu_dis_t *val);

typedef enum {
  LSM6DSO_AUX_ON                    = 1,
  LSM6DSO_AUX_ON_BY_AUX_INTERFACE   = 0,
} lsm6dso_ois_on_t;
int32_t lsm6dso_aux_pw_on_ctrl_set(lsm6dso_ctx_t *ctx, lsm6dso_ois_on_t val);
int32_t lsm6dso_aux_pw_on_ctrl_get(lsm6dso_ctx_t *ctx, lsm6dso_ois_on_t *val);

typedef enum {
  LSM6DSO_USE_SAME_XL_FS        = 0,
  LSM6DSO_USE_DIFFERENT_XL_FS   = 1,
} lsm6dso_xl_fs_mode_t;
int32_t lsm6dso_aux_xl_fs_mode_set(lsm6dso_ctx_t *ctx,
                                   lsm6dso_xl_fs_mode_t val);
int32_t lsm6dso_aux_xl_fs_mode_get(lsm6dso_ctx_t *ctx,
                                   lsm6dso_xl_fs_mode_t *val);

int32_t lsm6dso_aux_status_reg_get(lsm6dso_ctx_t *ctx,
                                   lsm6dso_status_spiaux_t *val);

int32_t lsm6dso_aux_xl_flag_data_ready_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_aux_gy_flag_data_ready_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_aux_gy_flag_settling_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_AUX_XL_DISABLE = 0,
  LSM6DSO_AUX_XL_POS     = 1,
  LSM6DSO_AUX_XL_NEG     = 2,
} lsm6dso_st_xl_ois_t;
int32_t lsm6dso_aux_xl_self_test_set(lsm6dso_ctx_t *ctx,
                                     lsm6dso_st_xl_ois_t val);
int32_t lsm6dso_aux_xl_self_test_get(lsm6dso_ctx_t *ctx,
                                     lsm6dso_st_xl_ois_t *val);

typedef enum {
  LSM6DSO_AUX_DEN_ACTIVE_LOW     = 0,
  LSM6DSO_AUX_DEN_ACTIVE_HIGH    = 1,
} lsm6dso_den_lh_ois_t;
int32_t lsm6dso_aux_den_polarity_set(lsm6dso_ctx_t *ctx,
                                     lsm6dso_den_lh_ois_t val);
int32_t lsm6dso_aux_den_polarity_get(lsm6dso_ctx_t *ctx,
                                     lsm6dso_den_lh_ois_t *val);

typedef enum {
  LSM6DSO_AUX_DEN_DISABLE         = 0,
  LSM6DSO_AUX_DEN_LEVEL_LATCH     = 3,
  LSM6DSO_AUX_DEN_LEVEL_TRIG      = 2,
} lsm6dso_lvl2_ois_t;
int32_t lsm6dso_aux_den_mode_set(lsm6dso_ctx_t *ctx, lsm6dso_lvl2_ois_t val);
int32_t lsm6dso_aux_den_mode_get(lsm6dso_ctx_t *ctx, lsm6dso_lvl2_ois_t *val);

int32_t lsm6dso_aux_drdy_on_int2_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_aux_drdy_on_int2_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_AUX_DISABLE  = 0,
  LSM6DSO_MODE_3_GY    = 1,
  LSM6DSO_MODE_4_GY_XL = 3,
} lsm6dso_ois_en_spi2_t;
int32_t lsm6dso_aux_mode_set(lsm6dso_ctx_t *ctx, lsm6dso_ois_en_spi2_t val);
int32_t lsm6dso_aux_mode_get(lsm6dso_ctx_t *ctx, lsm6dso_ois_en_spi2_t *val);

typedef enum {
  LSM6DSO_250dps_AUX  = 0,
  LSM6DSO_125dps_AUX  = 1,
  LSM6DSO_500dps_AUX  = 2,
  LSM6DSO_1000dps_AUX = 4,
  LSM6DSO_2000dps_AUX = 6,
} lsm6dso_fs_g_ois_t;
int32_t lsm6dso_aux_gy_full_scale_set(lsm6dso_ctx_t *ctx,
                                      lsm6dso_fs_g_ois_t val);
int32_t lsm6dso_aux_gy_full_scale_get(lsm6dso_ctx_t *ctx,
                                      lsm6dso_fs_g_ois_t *val);

typedef enum {
  LSM6DSO_AUX_SPI_4_WIRE = 0,
  LSM6DSO_AUX_SPI_3_WIRE = 1,
} lsm6dso_sim_ois_t;
int32_t lsm6dso_aux_spi_mode_set(lsm6dso_ctx_t *ctx, lsm6dso_sim_ois_t val);
int32_t lsm6dso_aux_spi_mode_get(lsm6dso_ctx_t *ctx, lsm6dso_sim_ois_t *val);

typedef enum {
  LSM6DSO_351Hz39 = 0,
  LSM6DSO_236Hz63 = 1,
  LSM6DSO_172Hz70 = 2,
  LSM6DSO_937Hz91 = 3,
} lsm6dso_ftype_ois_t;
int32_t lsm6dso_aux_gy_lp1_bandwidth_set(lsm6dso_ctx_t *ctx,
                                          lsm6dso_ftype_ois_t val);
int32_t lsm6dso_aux_gy_lp1_bandwidth_get(lsm6dso_ctx_t *ctx,
                                          lsm6dso_ftype_ois_t *val);

typedef enum {
  LSM6DSO_AUX_HP_DISABLE = 0x00,
  LSM6DSO_AUX_HP_Hz016   = 0x10,
  LSM6DSO_AUX_HP_Hz065   = 0x11,
  LSM6DSO_AUX_HP_Hz260   = 0x12,
  LSM6DSO_AUX_HP_1Hz040  = 0x13,
} lsm6dso_hpm_ois_t;
int32_t lsm6dso_aux_gy_hp_bandwidth_set(lsm6dso_ctx_t *ctx,
                                         lsm6dso_hpm_ois_t val);
int32_t lsm6dso_aux_gy_hp_bandwidth_get(lsm6dso_ctx_t *ctx,
                                         lsm6dso_hpm_ois_t *val);

typedef enum {
  LSM6DSO_ENABLE_CLAMP  = 0,
  LSM6DSO_DISABLE_CLAMP = 1,
} lsm6dso_st_ois_clampdis_t;
int32_t lsm6dso_aux_gy_clamp_set(lsm6dso_ctx_t *ctx,
                                 lsm6dso_st_ois_clampdis_t val);
int32_t lsm6dso_aux_gy_clamp_get(lsm6dso_ctx_t *ctx,
                                 lsm6dso_st_ois_clampdis_t *val);

typedef enum {
  LSM6DSO_AUX_GY_DISABLE = 0,
  LSM6DSO_AUX_GY_POS     = 1,
  LSM6DSO_AUX_GY_NEG     = 3,
} lsm6dso_st_ois_t;
int32_t lsm6dso_aux_gy_self_test_set(lsm6dso_ctx_t *ctx,
                                     lsm6dso_st_ois_t val);
int32_t lsm6dso_aux_gy_self_test_get(lsm6dso_ctx_t *ctx,
                                     lsm6dso_st_ois_t *val);

typedef enum {
  LSM6DSO_289Hz = 0,
  LSM6DSO_258Hz = 1,
  LSM6DSO_120Hz = 2,
  LSM6DSO_65Hz2 = 3,
  LSM6DSO_33Hz2 = 4,
  LSM6DSO_16Hz6 = 5,
  LSM6DSO_8Hz30 = 6,
  LSM6DSO_4Hz15 = 7,
} lsm6dso_filter_xl_conf_ois_t;
int32_t lsm6dso_aux_xl_bandwidth_set(lsm6dso_ctx_t *ctx,
                                     lsm6dso_filter_xl_conf_ois_t val);
int32_t lsm6dso_aux_xl_bandwidth_get(lsm6dso_ctx_t *ctx,
                                     lsm6dso_filter_xl_conf_ois_t *val);

typedef enum {
  LSM6DSO_AUX_2g  = 0,
  LSM6DSO_AUX_16g = 1,
  LSM6DSO_AUX_4g  = 2,
  LSM6DSO_AUX_8g  = 3,
} lsm6dso_fs_xl_ois_t;
int32_t lsm6dso_aux_xl_full_scale_set(lsm6dso_ctx_t *ctx,
                                      lsm6dso_fs_xl_ois_t val);
int32_t lsm6dso_aux_xl_full_scale_get(lsm6dso_ctx_t *ctx,
                                      lsm6dso_fs_xl_ois_t *val);

typedef enum {
  LSM6DSO_PULL_UP_DISC       = 0,
  LSM6DSO_PULL_UP_CONNECT    = 1,
} lsm6dso_sdo_pu_en_t;
int32_t lsm6dso_sdo_sa0_mode_set(lsm6dso_ctx_t *ctx,
                                 lsm6dso_sdo_pu_en_t val);
int32_t lsm6dso_sdo_sa0_mode_get(lsm6dso_ctx_t *ctx,
                                 lsm6dso_sdo_pu_en_t *val);

typedef enum {
  LSM6DSO_SPI_4_WIRE = 0,
  LSM6DSO_SPI_3_WIRE = 1,
} lsm6dso_sim_t;
int32_t lsm6dso_spi_mode_set(lsm6dso_ctx_t *ctx, lsm6dso_sim_t val);
int32_t lsm6dso_spi_mode_get(lsm6dso_ctx_t *ctx, lsm6dso_sim_t *val);

typedef enum {
  LSM6DSO_I2C_ENABLE  = 0,
  LSM6DSO_I2C_DISABLE = 1,
} lsm6dso_i2c_disable_t;
int32_t lsm6dso_i2c_interface_set(lsm6dso_ctx_t *ctx,
                                  lsm6dso_i2c_disable_t val);
int32_t lsm6dso_i2c_interface_get(lsm6dso_ctx_t *ctx,
                                  lsm6dso_i2c_disable_t *val);

typedef enum {
  LSM6DSO_I3C_DISABLE         = 0x80,
  LSM6DSO_I3C_ENABLE_T_50us   = 0x00,
  LSM6DSO_I3C_ENABLE_T_2us    = 0x01,
  LSM6DSO_I3C_ENABLE_T_1ms    = 0x02,
  LSM6DSO_I3C_ENABLE_T_25ms   = 0x03,
} lsm6dso_i3c_disable_t;
int32_t lsm6dso_i3c_disable_set(lsm6dso_ctx_t *ctx,
                                lsm6dso_i3c_disable_t val);
int32_t lsm6dso_i3c_disable_get(lsm6dso_ctx_t *ctx,
                                lsm6dso_i3c_disable_t *val);

typedef enum {
  LSM6DSO_PULL_DOWN_DISC       = 0,
  LSM6DSO_PULL_DOWN_CONNECT    = 1,
} lsm6dso_int1_pd_en_t;
int32_t lsm6dso_int1_mode_set(lsm6dso_ctx_t *ctx,
                              lsm6dso_int1_pd_en_t val);
int32_t lsm6dso_int1_mode_get(lsm6dso_ctx_t *ctx,
                              lsm6dso_int1_pd_en_t *val);

typedef struct {
    lsm6dso_int1_ctrl_t          int1_ctrl;
    lsm6dso_md1_cfg_t            md1_cfg;
    lsm6dso_emb_func_int1_t      emb_func_int1;
    lsm6dso_fsm_int1_a_t         fsm_int1_a;
    lsm6dso_fsm_int1_b_t         fsm_int1_b;
} lsm6dso_pin_int1_route_t;
int32_t lsm6dso_pin_int1_route_set(lsm6dso_ctx_t *ctx,
                                   lsm6dso_pin_int1_route_t *val);
int32_t lsm6dso_pin_int1_route_get(lsm6dso_ctx_t *ctx,
                                   lsm6dso_pin_int1_route_t *val);

typedef struct {
  lsm6dso_int2_ctrl_t          int2_ctrl;
  lsm6dso_md2_cfg_t            md2_cfg;
  lsm6dso_emb_func_int2_t      emb_func_int2;
  lsm6dso_fsm_int2_a_t         fsm_int2_a;
  lsm6dso_fsm_int2_b_t         fsm_int2_b;
} lsm6dso_pin_int2_route_t;
int32_t lsm6dso_pin_int2_route_set(lsm6dso_ctx_t *ctx,
                                   lsm6dso_pin_int2_route_t *val);
int32_t lsm6dso_pin_int2_route_get(lsm6dso_ctx_t *ctx,
                                   lsm6dso_pin_int2_route_t *val);

typedef enum {
  LSM6DSO_PUSH_PULL   = 0,
  LSM6DSO_OPEN_DRAIN  = 1,
} lsm6dso_pp_od_t;
int32_t lsm6dso_pin_mode_set(lsm6dso_ctx_t *ctx, lsm6dso_pp_od_t val);
int32_t lsm6dso_pin_mode_get(lsm6dso_ctx_t *ctx, lsm6dso_pp_od_t *val);

typedef enum {
  LSM6DSO_ACTIVE_HIGH = 0,
  LSM6DSO_ACTIVE_LOW  = 1,
} lsm6dso_h_lactive_t;
int32_t lsm6dso_pin_polarity_set(lsm6dso_ctx_t *ctx,
                                 lsm6dso_h_lactive_t val);
int32_t lsm6dso_pin_polarity_get(lsm6dso_ctx_t *ctx,
                                 lsm6dso_h_lactive_t *val);

int32_t lsm6dso_all_on_int1_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_all_on_int1_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_ALL_INT_PULSED            = 0,
  LSM6DSO_BASE_LATCHED_EMB_PULSED   = 1,
  LSM6DSO_BASE_PULSED_EMB_LATCHED   = 2,
  LSM6DSO_ALL_INT_LATCHED           = 3,
} lsm6dso_lir_t;
int32_t lsm6dso_int_notification_set(lsm6dso_ctx_t *ctx, lsm6dso_lir_t val);
int32_t lsm6dso_int_notification_get(lsm6dso_ctx_t *ctx, lsm6dso_lir_t *val);

typedef enum {
  LSM6DSO_LSb_FS_DIV_64       = 0,
  LSM6DSO_LSb_FS_DIV_256      = 1,
} lsm6dso_wake_ths_w_t;
int32_t lsm6dso_wkup_ths_weight_set(lsm6dso_ctx_t *ctx,
                                      lsm6dso_wake_ths_w_t val);
int32_t lsm6dso_wkup_ths_weight_get(lsm6dso_ctx_t *ctx,
                                       lsm6dso_wake_ths_w_t *val);

int32_t lsm6dso_wkup_threshold_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_wkup_threshold_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_xl_usr_offset_on_wkup_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_xl_usr_offset_on_wkup_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_wkup_dur_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_wkup_dur_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_gy_sleep_mode_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_gy_sleep_mode_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_DRIVE_SLEEP_CHG_EVENT = 0,
  LSM6DSO_DRIVE_SLEEP_STATUS    = 1,
} lsm6dso_sleep_status_on_int_t;
int32_t lsm6dso_act_pin_notification_set(lsm6dso_ctx_t *ctx,
                                         lsm6dso_sleep_status_on_int_t val);
int32_t lsm6dso_act_pin_notification_get(lsm6dso_ctx_t *ctx,
                                         lsm6dso_sleep_status_on_int_t *val);

typedef enum {
  LSM6DSO_XL_AND_GY_NOT_AFFECTED      = 0,
  LSM6DSO_XL_12Hz5_GY_NOT_AFFECTED    = 1,
  LSM6DSO_XL_12Hz5_GY_SLEEP           = 2,
  LSM6DSO_XL_12Hz5_GY_PD              = 3,
} lsm6dso_inact_en_t;
int32_t lsm6dso_act_mode_set(lsm6dso_ctx_t *ctx, lsm6dso_inact_en_t val);
int32_t lsm6dso_act_mode_get(lsm6dso_ctx_t *ctx, lsm6dso_inact_en_t *val);

int32_t lsm6dso_act_sleep_dur_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_act_sleep_dur_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_tap_detection_on_z_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_tap_detection_on_z_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_tap_detection_on_y_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_tap_detection_on_y_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_tap_detection_on_x_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_tap_detection_on_x_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_tap_threshold_x_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_tap_threshold_x_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_XYZ = 0,
  LSM6DSO_YXZ = 1,
  LSM6DSO_XZY = 2,
  LSM6DSO_ZYX = 3,
  LSM6DSO_YZX = 5,
  LSM6DSO_ZXY = 6,
} lsm6dso_tap_priority_t;
int32_t lsm6dso_tap_axis_priority_set(lsm6dso_ctx_t *ctx,
                                      lsm6dso_tap_priority_t val);
int32_t lsm6dso_tap_axis_priority_get(lsm6dso_ctx_t *ctx,
                                      lsm6dso_tap_priority_t *val);

int32_t lsm6dso_tap_threshold_y_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_tap_threshold_y_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_tap_threshold_z_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_tap_threshold_z_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_tap_shock_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_tap_shock_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_tap_quiet_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_tap_quiet_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_tap_dur_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_tap_dur_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_ONLY_SINGLE = 0,
  LSM6DSO_BOTH_SINGLE_DOUBLE = 1,
} lsm6dso_single_double_tap_t;
int32_t lsm6dso_tap_mode_set(lsm6dso_ctx_t *ctx,
                             lsm6dso_single_double_tap_t val);
int32_t lsm6dso_tap_mode_get(lsm6dso_ctx_t *ctx,
                             lsm6dso_single_double_tap_t *val);

typedef enum {
  LSM6DSO_DEG_80  = 0,
  LSM6DSO_DEG_70  = 1,
  LSM6DSO_DEG_60  = 2,
  LSM6DSO_DEG_50  = 3,
} lsm6dso_sixd_ths_t;
int32_t lsm6dso_6d_threshold_set(lsm6dso_ctx_t *ctx, lsm6dso_sixd_ths_t val);
int32_t lsm6dso_6d_threshold_get(lsm6dso_ctx_t *ctx, lsm6dso_sixd_ths_t *val);

int32_t lsm6dso_4d_mode_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_4d_mode_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_FF_TSH_156mg = 0,
  LSM6DSO_FF_TSH_219mg = 1,
  LSM6DSO_FF_TSH_250mg = 2,
  LSM6DSO_FF_TSH_312mg = 3,
  LSM6DSO_FF_TSH_344mg = 4,
  LSM6DSO_FF_TSH_406mg = 5,
  LSM6DSO_FF_TSH_469mg = 6,
  LSM6DSO_FF_TSH_500mg = 7,
} lsm6dso_ff_ths_t;
int32_t lsm6dso_ff_threshold_set(lsm6dso_ctx_t *ctx, lsm6dso_ff_ths_t val);
int32_t lsm6dso_ff_threshold_get(lsm6dso_ctx_t *ctx, lsm6dso_ff_ths_t *val);

int32_t lsm6dso_ff_dur_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_ff_dur_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_fifo_watermark_set(lsm6dso_ctx_t *ctx, uint16_t val);
int32_t lsm6dso_fifo_watermark_get(lsm6dso_ctx_t *ctx, uint16_t *val);

int32_t lsm6dso_compression_algo_init_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_compression_algo_init_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_CMP_DISABLE  = 0x00,
  LSM6DSO_CMP_ALWAYS   = 0x04,
  LSM6DSO_CMP_8_TO_1   = 0x05,
  LSM6DSO_CMP_16_TO_1  = 0x06,
  LSM6DSO_CMP_32_TO_1  = 0x07,
} lsm6dso_uncoptr_rate_t;
int32_t lsm6dso_compression_algo_set(lsm6dso_ctx_t *ctx,
                                     lsm6dso_uncoptr_rate_t val);
int32_t lsm6dso_compression_algo_get(lsm6dso_ctx_t *ctx,
                                     lsm6dso_uncoptr_rate_t *val);

int32_t lsm6dso_fifo_virtual_sens_odr_chg_set(lsm6dso_ctx_t *ctx,
                                              uint8_t val);
int32_t lsm6dso_fifo_virtual_sens_odr_chg_get(lsm6dso_ctx_t *ctx,
                                              uint8_t *val);

int32_t lsm6dso_compression_algo_real_time_set(lsm6dso_ctx_t *ctx,
                                               uint8_t val);
int32_t lsm6dso_compression_algo_real_time_get(lsm6dso_ctx_t *ctx,
                                               uint8_t *val);

int32_t lsm6dso_fifo_stop_on_wtm_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_fifo_stop_on_wtm_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_XL_NOT_BATCHED     	=  0,
  LSM6DSO_XL_BATCHED_AT_12Hz5   =  1,
  LSM6DSO_XL_BATCHED_AT_26Hz    =  2,
  LSM6DSO_XL_BATCHED_AT_52Hz    =  3,
  LSM6DSO_XL_BATCHED_AT_104Hz   =  4,
  LSM6DSO_XL_BATCHED_AT_208Hz   =  5,
  LSM6DSO_XL_BATCHED_AT_417Hz   =  6,
  LSM6DSO_XL_BATCHED_AT_833Hz   =  7,
  LSM6DSO_XL_BATCHED_AT_1667Hz  =  8,
  LSM6DSO_XL_BATCHED_AT_3333Hz  =  9,
  LSM6DSO_XL_BATCHED_AT_6667Hz  = 10,
  LSM6DSO_XL_BATCHED_AT_6Hz5    = 11,
} lsm6dso_bdr_xl_t;
int32_t lsm6dso_fifo_xl_batch_set(lsm6dso_ctx_t *ctx, lsm6dso_bdr_xl_t val);
int32_t lsm6dso_fifo_xl_batch_get(lsm6dso_ctx_t *ctx, lsm6dso_bdr_xl_t *val);

typedef enum {
  LSM6DSO_GY_NOT_BATCHED         = 0,
  LSM6DSO_GY_BATCHED_AT_12Hz5    = 1,
  LSM6DSO_GY_BATCHED_AT_26Hz     = 2,
  LSM6DSO_GY_BATCHED_AT_52Hz     = 3,
  LSM6DSO_GY_BATCHED_AT_104Hz    = 4,
  LSM6DSO_GY_BATCHED_AT_208Hz    = 5,
  LSM6DSO_GY_BATCHED_AT_417Hz    = 6,
  LSM6DSO_GY_BATCHED_AT_833Hz    = 7,
  LSM6DSO_GY_BATCHED_AT_1667Hz   = 8,
  LSM6DSO_GY_BATCHED_AT_3333Hz   = 9,
  LSM6DSO_GY_BATCHED_AT_6667Hz   = 10,
  LSM6DSO_GY_BATCHED_AT_6Hz5     = 11,
} lsm6dso_bdr_gy_t;
int32_t lsm6dso_fifo_gy_batch_set(lsm6dso_ctx_t *ctx, lsm6dso_bdr_gy_t val);
int32_t lsm6dso_fifo_gy_batch_get(lsm6dso_ctx_t *ctx, lsm6dso_bdr_gy_t *val);

typedef enum {
  LSM6DSO_BYPASS_MODE             = 0,
  LSM6DSO_FIFO_MODE               = 1,
  LSM6DSO_STREAM_TO_FIFO_MODE     = 3,
  LSM6DSO_BYPASS_TO_STREAM_MODE   = 4,
  LSM6DSO_STREAM_MODE             = 6,
  LSM6DSO_BYPASS_TO_FIFO_MODE     = 7,
} lsm6dso_fifo_mode_t;
int32_t lsm6dso_fifo_mode_set(lsm6dso_ctx_t *ctx, lsm6dso_fifo_mode_t val);
int32_t lsm6dso_fifo_mode_get(lsm6dso_ctx_t *ctx, lsm6dso_fifo_mode_t *val);

typedef enum {
  LSM6DSO_TEMP_NOT_BATCHED        = 0,
  LSM6DSO_TEMP_BATCHED_AT_1Hz6    = 1,
  LSM6DSO_TEMP_BATCHED_AT_12Hz5   = 2,
  LSM6DSO_TEMP_BATCHED_AT_52Hz    = 3,
} lsm6dso_odr_t_batch_t;
int32_t lsm6dso_fifo_temp_batch_set(lsm6dso_ctx_t *ctx,
                                    lsm6dso_odr_t_batch_t val);
int32_t lsm6dso_fifo_temp_batch_get(lsm6dso_ctx_t *ctx,
                                    lsm6dso_odr_t_batch_t *val);

typedef enum {
  LSM6DSO_NO_DECIMATION = 0,
  LSM6DSO_DEC_1         = 1,
  LSM6DSO_DEC_8         = 2,
  LSM6DSO_DEC_32        = 3,
} lsm6dso_odr_ts_batch_t;
int32_t lsm6dso_fifo_timestamp_decimation_set(lsm6dso_ctx_t *ctx,
                                              lsm6dso_odr_ts_batch_t val);
int32_t lsm6dso_fifo_timestamp_decimation_get(lsm6dso_ctx_t *ctx,
                                              lsm6dso_odr_ts_batch_t *val);

typedef enum {
  LSM6DSO_XL_BATCH_EVENT   = 0,
  LSM6DSO_GYRO_BATCH_EVENT = 1,
} lsm6dso_trig_counter_bdr_t;

typedef enum {
  LSM6DSO_GYRO_NC_TAG		= 1,
  LSM6DSO_XL_NC_TAG,
  LSM6DSO_TEMPERATURE_TAG,
  LSM6DSO_TIMESTAMP_TAG,
  LSM6DSO_CFG_CHANGE_TAG,
  LSM6DSO_XL_NC_T_2_TAG,
  LSM6DSO_XL_NC_T_1_TAG,
  LSM6DSO_XL_2XC_TAG,
  LSM6DSO_XL_3XC_TAG,
  LSM6DSO_GYRO_NC_T_2_TAG,
  LSM6DSO_GYRO_NC_T_1_TAG,
  LSM6DSO_GYRO_2XC_TAG,
  LSM6DSO_GYRO_3XC_TAG,
  LSM6DSO_SENSORHUB_SLAVE0_TAG,
  LSM6DSO_SENSORHUB_SLAVE1_TAG,
  LSM6DSO_SENSORHUB_SLAVE2_TAG,
  LSM6DSO_SENSORHUB_SLAVE3_TAG,
  LSM6DSO_STEP_CPUNTER_TAG,
  LSM6DSO_GAME_ROTATION_TAG,
  LSM6DSO_GEOMAG_ROTATION_TAG,
  LSM6DSO_ROTATION_TAG,
  LSM6DSO_SENSORHUB_NACK_TAG	= 0x19,
} lsm6dso_fifo_tag_t;
int32_t lsm6dso_fifo_cnt_event_batch_set(lsm6dso_ctx_t *ctx,
                                          lsm6dso_trig_counter_bdr_t val);
int32_t lsm6dso_fifo_cnt_event_batch_get(lsm6dso_ctx_t *ctx,
                                          lsm6dso_trig_counter_bdr_t *val);

int32_t lsm6dso_rst_batch_counter_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_rst_batch_counter_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_batch_counter_threshold_set(lsm6dso_ctx_t *ctx,
                                            uint16_t val);
int32_t lsm6dso_batch_counter_threshold_get(lsm6dso_ctx_t *ctx,
                                            uint16_t *val);

int32_t lsm6dso_fifo_data_level_get(lsm6dso_ctx_t *ctx, uint16_t *val);

int32_t lsm6dso_fifo_status_get(lsm6dso_ctx_t *ctx,
                                lsm6dso_fifo_status2_t *val);

int32_t lsm6dso_fifo_full_flag_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_fifo_ovr_flag_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_fifo_wtm_flag_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_fifo_sensor_tag_get(lsm6dso_ctx_t *ctx,
				    lsm6dso_fifo_tag_t *val);

int32_t lsm6dso_fifo_pedo_batch_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_fifo_pedo_batch_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_sh_batch_slave_0_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_sh_batch_slave_0_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_sh_batch_slave_1_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_sh_batch_slave_1_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_sh_batch_slave_2_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_sh_batch_slave_2_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_sh_batch_slave_3_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_sh_batch_slave_3_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_DEN_DISABLE    = 0,
  LSM6DSO_LEVEL_FIFO     = 6,
  LSM6DSO_LEVEL_LETCHED  = 3,
  LSM6DSO_LEVEL_TRIGGER  = 2,
  LSM6DSO_EDGE_TRIGGER   = 4,
} lsm6dso_den_mode_t;
int32_t lsm6dso_den_mode_set(lsm6dso_ctx_t *ctx, lsm6dso_den_mode_t val);
int32_t lsm6dso_den_mode_get(lsm6dso_ctx_t *ctx, lsm6dso_den_mode_t *val);

typedef enum {
  LSM6DSO_DEN_ACT_LOW  = 0,
  LSM6DSO_DEN_ACT_HIGH = 1,
} lsm6dso_den_lh_t;
int32_t lsm6dso_den_polarity_set(lsm6dso_ctx_t *ctx, lsm6dso_den_lh_t val);
int32_t lsm6dso_den_polarity_get(lsm6dso_ctx_t *ctx, lsm6dso_den_lh_t *val);

typedef enum {
  LSM6DSO_STAMP_IN_GY_DATA     = 0,
  LSM6DSO_STAMP_IN_XL_DATA     = 1,
  LSM6DSO_STAMP_IN_GY_XL_DATA  = 2,
} lsm6dso_den_xl_g_t;
int32_t lsm6dso_den_enable_set(lsm6dso_ctx_t *ctx, lsm6dso_den_xl_g_t val);
int32_t lsm6dso_den_enable_get(lsm6dso_ctx_t *ctx, lsm6dso_den_xl_g_t *val);

int32_t lsm6dso_den_mark_axis_x_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_den_mark_axis_x_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_den_mark_axis_y_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_den_mark_axis_y_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_den_mark_axis_z_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_den_mark_axis_z_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_PEDO_DISABLE              = 0x00,
  LSM6DSO_PEDO_BASE_MODE            = 0x01,
  LSM6DSO_PEDO_ADV_MODE             = 0x03,
  LSM6DSO_FALSE_STEP_REJ            = 0x13,
  LSM6DSO_FALSE_STEP_REJ_ADV_MODE   = 0x33,
} lsm6dso_pedo_md_t;
int32_t lsm6dso_pedo_sens_set(lsm6dso_ctx_t *ctx, lsm6dso_pedo_md_t val);
int32_t lsm6dso_pedo_sens_get(lsm6dso_ctx_t *ctx, lsm6dso_pedo_md_t *val);

int32_t lsm6dso_pedo_step_detect_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_pedo_debounce_steps_set(lsm6dso_ctx_t *ctx,
                                             uint8_t *buff);
int32_t lsm6dso_pedo_debounce_steps_get(lsm6dso_ctx_t *ctx,
                                             uint8_t *buff);

int32_t lsm6dso_pedo_steps_period_set(lsm6dso_ctx_t *ctx, uint8_t *buff);
int32_t lsm6dso_pedo_steps_period_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

typedef enum {
  LSM6DSO_EVERY_STEP     = 0,
  LSM6DSO_COUNT_OVERFLOW = 1,
} lsm6dso_carry_count_en_t;
int32_t lsm6dso_pedo_int_mode_set(lsm6dso_ctx_t *ctx,
                                  lsm6dso_carry_count_en_t val);
int32_t lsm6dso_pedo_int_mode_get(lsm6dso_ctx_t *ctx,
                                  lsm6dso_carry_count_en_t *val);

int32_t lsm6dso_motion_sens_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_motion_sens_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_motion_flag_data_ready_get(lsm6dso_ctx_t *ctx,
                                                uint8_t *val);

int32_t lsm6dso_tilt_sens_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_tilt_sens_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_tilt_flag_data_ready_get(lsm6dso_ctx_t *ctx,
                                              uint8_t *val);

int32_t lsm6dso_mag_sensitivity_set(lsm6dso_ctx_t *ctx, uint8_t *buff);
int32_t lsm6dso_mag_sensitivity_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

int32_t lsm6dso_mag_offset_set(lsm6dso_ctx_t *ctx, uint8_t *buff);
int32_t lsm6dso_mag_offset_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

int32_t lsm6dso_mag_soft_iron_set(lsm6dso_ctx_t *ctx, uint8_t *buff);
int32_t lsm6dso_mag_soft_iron_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

typedef enum {
  LSM6DSO_Z_EQ_Y     = 0,
  LSM6DSO_Z_EQ_MIN_Y = 1,
  LSM6DSO_Z_EQ_X     = 2,
  LSM6DSO_Z_EQ_MIN_X = 3,
  LSM6DSO_Z_EQ_MIN_Z = 4,
  LSM6DSO_Z_EQ_Z     = 5,
} lsm6dso_mag_z_axis_t;
int32_t lsm6dso_mag_z_orient_set(lsm6dso_ctx_t *ctx,
                                         lsm6dso_mag_z_axis_t val);
int32_t lsm6dso_mag_z_orient_get(lsm6dso_ctx_t *ctx,
                                         lsm6dso_mag_z_axis_t *val);

typedef enum {
  LSM6DSO_Y_EQ_Y     = 0,
  LSM6DSO_Y_EQ_MIN_Y = 1,
  LSM6DSO_Y_EQ_X     = 2,
  LSM6DSO_Y_EQ_MIN_X = 3,
  LSM6DSO_Y_EQ_MIN_Z = 4,
  LSM6DSO_Y_EQ_Z     = 5,
} lsm6dso_mag_y_axis_t;
int32_t lsm6dso_mag_y_orient_set(lsm6dso_ctx_t *ctx,
                                         lsm6dso_mag_y_axis_t val);
int32_t lsm6dso_mag_y_orient_get(lsm6dso_ctx_t *ctx,
                                         lsm6dso_mag_y_axis_t *val);

typedef enum {
  LSM6DSO_X_EQ_Y     = 0,
  LSM6DSO_X_EQ_MIN_Y = 1,
  LSM6DSO_X_EQ_X     = 2,
  LSM6DSO_X_EQ_MIN_X = 3,
  LSM6DSO_X_EQ_MIN_Z = 4,
  LSM6DSO_X_EQ_Z     = 5,
} lsm6dso_mag_x_axis_t;
int32_t lsm6dso_mag_x_orient_set(lsm6dso_ctx_t *ctx,
                                         lsm6dso_mag_x_axis_t val);
int32_t lsm6dso_mag_x_orient_get(lsm6dso_ctx_t *ctx,
                                         lsm6dso_mag_x_axis_t *val);

int32_t lsm6dso_long_cnt_flag_data_ready_get(lsm6dso_ctx_t *ctx,
                                             uint8_t *val);

int32_t lsm6dso_emb_fsm_en_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_emb_fsm_en_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef struct {
    lsm6dso_fsm_enable_a_t          fsm_enable_a;
    lsm6dso_fsm_enable_b_t          fsm_enable_b;
} lsm6dso_emb_fsm_enable_t;
int32_t lsm6dso_fsm_enable_set(lsm6dso_ctx_t *ctx,
                               lsm6dso_emb_fsm_enable_t *val);
int32_t lsm6dso_fsm_enable_get(lsm6dso_ctx_t *ctx,
                               lsm6dso_emb_fsm_enable_t *val);

int32_t lsm6dso_long_cnt_set(lsm6dso_ctx_t *ctx, uint8_t *buff);
int32_t lsm6dso_long_cnt_get(lsm6dso_ctx_t *ctx, uint8_t *buff);

typedef enum {
  LSM6DSO_LC_NORMAL     = 0,
  LSM6DSO_LC_CLEAR      = 1,
  LSM6DSO_LC_CLEAR_DONE = 2,
} lsm6dso_fsm_lc_clr_t;
int32_t lsm6dso_long_clr_set(lsm6dso_ctx_t *ctx, lsm6dso_fsm_lc_clr_t val);
int32_t lsm6dso_long_clr_get(lsm6dso_ctx_t *ctx, lsm6dso_fsm_lc_clr_t *val);

typedef struct {
    lsm6dso_fsm_outs1_t    fsm_outs1;
    lsm6dso_fsm_outs2_t    fsm_outs2;
    lsm6dso_fsm_outs3_t    fsm_outs3;
    lsm6dso_fsm_outs4_t    fsm_outs4;
    lsm6dso_fsm_outs5_t    fsm_outs5;
    lsm6dso_fsm_outs6_t    fsm_outs6;
    lsm6dso_fsm_outs7_t    fsm_outs7;
    lsm6dso_fsm_outs8_t    fsm_outs8;
    lsm6dso_fsm_outs9_t    fsm_outs9;
    lsm6dso_fsm_outs10_t   fsm_outs10;
    lsm6dso_fsm_outs11_t   fsm_outs11;
    lsm6dso_fsm_outs12_t   fsm_outs12;
    lsm6dso_fsm_outs13_t   fsm_outs13;
    lsm6dso_fsm_outs14_t   fsm_outs14;
    lsm6dso_fsm_outs15_t   fsm_outs15;
    lsm6dso_fsm_outs16_t   fsm_outs16;
} lsm6dso_fsm_out_t;
int32_t lsm6dso_fsm_out_get(lsm6dso_ctx_t *ctx, lsm6dso_fsm_out_t *val);

typedef enum {
  LSM6DSO_ODR_FSM_12Hz5 = 0,
  LSM6DSO_ODR_FSM_26Hz  = 1,
  LSM6DSO_ODR_FSM_52Hz  = 2,
  LSM6DSO_ODR_FSM_104Hz = 3,
} lsm6dso_fsm_odr_t;
int32_t lsm6dso_fsm_data_rate_set(lsm6dso_ctx_t *ctx, lsm6dso_fsm_odr_t val);
int32_t lsm6dso_fsm_data_rate_get(lsm6dso_ctx_t *ctx, lsm6dso_fsm_odr_t *val);

int32_t lsm6dso_fsm_init_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_fsm_init_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_long_cnt_int_value_set(lsm6dso_ctx_t *ctx, uint16_t val);
int32_t lsm6dso_long_cnt_int_value_get(lsm6dso_ctx_t *ctx, uint16_t *val);

int32_t lsm6dso_fsm_number_of_programs_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_fsm_number_of_programs_get(lsm6dso_ctx_t *ctx, uint8_t *val);

int32_t lsm6dso_fsm_start_address_set(lsm6dso_ctx_t *ctx, uint16_t val);
int32_t lsm6dso_fsm_start_address_get(lsm6dso_ctx_t *ctx, uint16_t *val);

int32_t lsm6dso_sh_read_data_raw_get(lsm6dso_ctx_t *ctx, uint8_t *val,
                                     uint8_t len);

typedef enum {
  LSM6DSO_SLV_0       = 0,
  LSM6DSO_SLV_0_1     = 1,
  LSM6DSO_SLV_0_1_2   = 2,
  LSM6DSO_SLV_0_1_2_3 = 3,
} lsm6dso_aux_sens_on_t;
int32_t lsm6dso_sh_slave_connected_set(lsm6dso_ctx_t *ctx,
                                       lsm6dso_aux_sens_on_t val);
int32_t lsm6dso_sh_slave_connected_get(lsm6dso_ctx_t *ctx,
                                       lsm6dso_aux_sens_on_t *val);

int32_t lsm6dso_sh_master_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_sh_master_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_EXT_PULL_UP      = 0,
  LSM6DSO_INTERNAL_PULL_UP = 1,
} lsm6dso_shub_pu_en_t;
int32_t lsm6dso_sh_pin_mode_set(lsm6dso_ctx_t *ctx, lsm6dso_shub_pu_en_t val);
int32_t lsm6dso_sh_pin_mode_get(lsm6dso_ctx_t *ctx, lsm6dso_shub_pu_en_t *val);

int32_t lsm6dso_sh_pass_through_set(lsm6dso_ctx_t *ctx, uint8_t val);
int32_t lsm6dso_sh_pass_through_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_EXT_ON_INT2_PIN = 0,
  LSM6DSO_XL_GY_DRDY      = 1,
} lsm6dso_start_config_t;
int32_t lsm6dso_sh_syncro_mode_set(lsm6dso_ctx_t *ctx,
                                   lsm6dso_start_config_t val);
int32_t lsm6dso_sh_syncro_mode_get(lsm6dso_ctx_t *ctx,
                                   lsm6dso_start_config_t *val);

typedef enum {
  LSM6DSO_EACH_SH_CYCLE    = 0,
  LSM6DSO_ONLY_FIRST_CYCLE = 1,
} lsm6dso_write_once_t;
int32_t lsm6dso_sh_write_mode_set(lsm6dso_ctx_t *ctx,
                                  lsm6dso_write_once_t val);
int32_t lsm6dso_sh_write_mode_get(lsm6dso_ctx_t *ctx,
                                  lsm6dso_write_once_t *val);

int32_t lsm6dso_sh_reset_set(lsm6dso_ctx_t *ctx);
int32_t lsm6dso_sh_reset_get(lsm6dso_ctx_t *ctx, uint8_t *val);

typedef enum {
  LSM6DSO_SH_ODR_104Hz = 0,
  LSM6DSO_SH_ODR_52Hz  = 1,
  LSM6DSO_SH_ODR_26Hz  = 2,
  LSM6DSO_SH_ODR_13Hz  = 3,
} lsm6dso_shub_odr_t;
int32_t lsm6dso_sh_data_rate_set(lsm6dso_ctx_t *ctx, lsm6dso_shub_odr_t val);
int32_t lsm6dso_sh_data_rate_get(lsm6dso_ctx_t *ctx, lsm6dso_shub_odr_t *val);

typedef struct{
  uint8_t   slv0_add;
  uint8_t   slv0_subadd;
  uint8_t   slv0_data;
} lsm6dso_sh_cfg_write_t;
int32_t lsm6dso_sh_cfg_write(lsm6dso_ctx_t *ctx, lsm6dso_sh_cfg_write_t *val);

typedef struct{
  uint8_t   slv_add;
  uint8_t   slv_subadd;
  uint8_t   slv_len;
} lsm6dso_sh_cfg_read_t;
int32_t lsm6dso_sh_slv0_cfg_read(lsm6dso_ctx_t *ctx,
                                 lsm6dso_sh_cfg_read_t *val);
int32_t lsm6dso_sh_slv1_cfg_read(lsm6dso_ctx_t *ctx,
                                 lsm6dso_sh_cfg_read_t *val);
int32_t lsm6dso_sh_slv2_cfg_read(lsm6dso_ctx_t *ctx,
                                 lsm6dso_sh_cfg_read_t *val);
int32_t lsm6dso_sh_slv3_cfg_read(lsm6dso_ctx_t *ctx,
                                 lsm6dso_sh_cfg_read_t *val);

int32_t lsm6dso_sh_status_get(lsm6dso_ctx_t *ctx,
                              lsm6dso_status_master_t *val);


#endif