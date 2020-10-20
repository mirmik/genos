/*
 ******************************************************************************
 * @file    lsm6dso_reg.h
 * @author  Sensor Solutions Software Team
 * @brief   This file contains all the functions prototypes for the
 *          lsm6dso_reg.c driver.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LSM6DSO_DRIVER_H
#define LSM6DSO_DRIVER_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <math.h>

/** @addtogroup LSM6DSO
  * @{
  *
  */

/** @defgroup LSM6DSO_sensors_common_types
  * @{
  *
  */

#ifndef MEMS_SHARED_TYPES
#define MEMS_SHARED_TYPES

/**
  * @defgroup axisXbitXX_t
  * @brief    These unions are useful to represent different sensors data type.
  *           These unions are not need by the driver.
  *
  *           REMOVING the unions you are compliant with:
  *           MISRA-C 2012 [Rule 19.2] -> " Union are not allowed "
  *
  * @{
  *
  */

typedef union{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} axis3bit16_t;

typedef union{
  int16_t i16bit;
  uint8_t u8bit[2];
} axis1bit16_t;

typedef union{
  int32_t i32bit[3];
  uint8_t u8bit[12];
} axis3bit32_t;

typedef union{
  int32_t i32bit;
  uint8_t u8bit[4];
} axis1bit32_t;

/**
  * @}
  *
  */

typedef struct{
  uint8_t bit0       : 1;
  uint8_t bit1       : 1;
  uint8_t bit2       : 1;
  uint8_t bit3       : 1;
  uint8_t bit4       : 1;
  uint8_t bit5       : 1;
  uint8_t bit6       : 1;
  uint8_t bit7       : 1;
} bitwise_t;

#define PROPERTY_DISABLE                (0U)
#define PROPERTY_ENABLE                 (1U)

#endif /* MEMS_SHARED_TYPES */

/**
  * @}
  *
  */

/** @addtogroup  LSM6DSO_Interfaces_Functions
  * @brief       This section provide a set of functions used to read and
  *              write a generic register of the device.
  *              MANDATORY: return 0 -> no Error.
  * @{
  *
  */

typedef int32_t (*lsm6dso_write_ptr)(void *, uint8_t, uint8_t*, uint16_t);
typedef int32_t (*lsm6dso_read_ptr) (void *, uint8_t, uint8_t*, uint16_t);

typedef struct {
  /** Component mandatory fields **/
  lsm6dso_write_ptr  write_reg;
  lsm6dso_read_ptr   read_reg;
  /** Customizable optional pointer **/
  void *handle;
} lsm6dso_ctx_t;

/**
  * @}
  *
  */

/** @defgroup LSM6DSO_Infos
  * @{
  *
  */

/** I2C Device Address 8 bit format  if SA0=0 -> D5 if SA0=1 -> D7 **/
#define LSM6DSO_I2C_ADD_L                    0xD5
#define LSM6DSO_I2C_ADD_H                    0xD7


/**
  * @defgroup LSM6DSO_Register_Union
  * @brief    This union group all the registers that has a bitfield
  *           description.
  *           This union is useful but not need by the driver.
  *
  *           REMOVING this union you are compliant with:
  *           MISRA-C 2012 [Rule 19.2] -> " Union are not allowed "
  *
  * @{
  *
  */
typedef union{
  lsm6dso_func_cfg_access_t               func_cfg_access;
  lsm6dso_pin_ctrl_t                      pin_ctrl;
  lsm6dso_fifo_ctrl1_t                    fifo_ctrl1;
  lsm6dso_fifo_ctrl2_t                    fifo_ctrl2;
  lsm6dso_fifo_ctrl3_t                    fifo_ctrl3;
  lsm6dso_fifo_ctrl4_t                    fifo_ctrl4;
  lsm6dso_counter_bdr_reg1_t              counter_bdr_reg1;
  lsm6dso_counter_bdr_reg2_t              counter_bdr_reg2;
  lsm6dso_int1_ctrl_t                     int1_ctrl;
  lsm6dso_int2_ctrl_t                     int2_ctrl;
  lsm6dso_ctrl1_xl_t                      ctrl1_xl;
  lsm6dso_ctrl2_g_t                       ctrl2_g;
  lsm6dso_ctrl3_c_t                       ctrl3_c;
  lsm6dso_ctrl4_c_t                       ctrl4_c;
  lsm6dso_ctrl5_c_t                       ctrl5_c;
  lsm6dso_ctrl6_c_t                       ctrl6_c;
  lsm6dso_ctrl7_g_t                       ctrl7_g;
  lsm6dso_ctrl8_xl_t                      ctrl8_xl;
  lsm6dso_ctrl9_xl_t                      ctrl9_xl;
  lsm6dso_ctrl10_c_t                      ctrl10_c;
  lsm6dso_all_int_src_t                   all_int_src;
  lsm6dso_wake_up_src_t                   wake_up_src;
  lsm6dso_tap_src_t                       tap_src;
  lsm6dso_d6d_src_t                       d6d_src;
  lsm6dso_status_reg_t                    status_reg;
  lsm6dso_status_spiaux_t                 status_spiaux;
  lsm6dso_fifo_status1_t                  fifo_status1;
  lsm6dso_fifo_status2_t                  fifo_status2;
  lsm6dso_tap_cfg0_t                      tap_cfg0;
  lsm6dso_tap_cfg1_t                      tap_cfg1;
  lsm6dso_tap_cfg2_t                      tap_cfg2;
  lsm6dso_tap_ths_6d_t                    tap_ths_6d;
  lsm6dso_int_dur2_t                      int_dur2;
  lsm6dso_wake_up_ths_t                   wake_up_ths;
  lsm6dso_wake_up_dur_t                   wake_up_dur;
  lsm6dso_free_fall_t                     free_fall;
  lsm6dso_md1_cfg_t                       md1_cfg;
  lsm6dso_md2_cfg_t                       md2_cfg;
  lsm6dso_i3c_bus_avb_t                   i3c_bus_avb;
  lsm6dso_internal_freq_fine_t            internal_freq_fine;
  lsm6dso_int_ois_t                       int_ois;
  lsm6dso_ctrl1_ois_t                     ctrl1_ois;
  lsm6dso_ctrl2_ois_t                     ctrl2_ois;
  lsm6dso_ctrl3_ois_t                     ctrl3_ois;
  lsm6dso_fifo_data_out_tag_t             fifo_data_out_tag;
  lsm6dso_page_sel_t                      page_sel;
  lsm6dso_emb_func_en_a_t                 emb_func_en_a;
  lsm6dso_emb_func_en_b_t                 emb_func_en_b;
  lsm6dso_page_address_t                  page_address;
  lsm6dso_page_value_t                    page_value;
  lsm6dso_emb_func_int1_t                 emb_func_int1;
  lsm6dso_fsm_int1_a_t                    fsm_int1_a;
  lsm6dso_fsm_int1_b_t                    fsm_int1_b;
  lsm6dso_emb_func_int2_t                 emb_func_int2;
  lsm6dso_fsm_int2_a_t                    fsm_int2_a;
  lsm6dso_fsm_int2_b_t                    fsm_int2_b;
  lsm6dso_emb_func_status_t               emb_func_status;
  lsm6dso_fsm_status_a_t                  fsm_status_a;
  lsm6dso_fsm_status_b_t                  fsm_status_b;
  lsm6dso_page_rw_t                       page_rw;
  lsm6dso_emb_func_fifo_cfg_t	            emb_func_fifo_cfg;
  lsm6dso_fsm_enable_a_t                  fsm_enable_a;
  lsm6dso_fsm_enable_b_t                  fsm_enable_b;
  lsm6dso_fsm_long_counter_clear_t        fsm_long_counter_clear;
  lsm6dso_fsm_outs1_t                     fsm_outs1;
  lsm6dso_fsm_outs2_t                     fsm_outs2;
  lsm6dso_fsm_outs3_t                     fsm_outs3;
  lsm6dso_fsm_outs4_t                     fsm_outs4;
  lsm6dso_fsm_outs5_t                     fsm_outs5;
  lsm6dso_fsm_outs6_t                     fsm_outs6;
  lsm6dso_fsm_outs7_t                     fsm_outs7;
  lsm6dso_fsm_outs8_t                     fsm_outs8;
  lsm6dso_fsm_outs9_t                     fsm_outs9;
  lsm6dso_fsm_outs10_t                    fsm_outs10;
  lsm6dso_fsm_outs11_t                    fsm_outs11;
  lsm6dso_fsm_outs12_t                    fsm_outs12;
  lsm6dso_fsm_outs13_t                    fsm_outs13;
  lsm6dso_fsm_outs14_t                    fsm_outs14;
  lsm6dso_fsm_outs15_t                    fsm_outs15;
  lsm6dso_fsm_outs16_t                    fsm_outs16;
  lsm6dso_emb_func_odr_cfg_b_t            emb_func_odr_cfg_b;
  lsm6dso_emb_func_src_t                  emb_func_src;
  lsm6dso_emb_func_init_a_t               emb_func_init_a;
  lsm6dso_emb_func_init_b_t               emb_func_init_b;
  lsm6dso_mag_cfg_a_t                     mag_cfg_a;
  lsm6dso_mag_cfg_b_t                     mag_cfg_b;
  lsm6dso_pedo_cmd_reg_t                  pedo_cmd_reg;
  lsm6dso_sensor_hub_1_t                  sensor_hub_1;
  lsm6dso_sensor_hub_2_t                  sensor_hub_2;
  lsm6dso_sensor_hub_3_t                  sensor_hub_3;
  lsm6dso_sensor_hub_4_t                  sensor_hub_4;
  lsm6dso_sensor_hub_5_t                  sensor_hub_5;
  lsm6dso_sensor_hub_6_t                  sensor_hub_6;
  lsm6dso_sensor_hub_7_t                  sensor_hub_7;
  lsm6dso_sensor_hub_8_t                  sensor_hub_8;
  lsm6dso_sensor_hub_9_t                  sensor_hub_9;
  lsm6dso_sensor_hub_10_t                 sensor_hub_10;
  lsm6dso_sensor_hub_11_t                 sensor_hub_11;
  lsm6dso_sensor_hub_12_t                 sensor_hub_12;
  lsm6dso_sensor_hub_13_t                 sensor_hub_13;
  lsm6dso_sensor_hub_14_t                 sensor_hub_14;
  lsm6dso_sensor_hub_15_t                 sensor_hub_15;
  lsm6dso_sensor_hub_16_t                 sensor_hub_16;
  lsm6dso_sensor_hub_17_t                 sensor_hub_17;
  lsm6dso_sensor_hub_18_t                 sensor_hub_18;
  lsm6dso_master_config_t                 master_config;
  lsm6dso_slv0_add_t                      slv0_add;
  lsm6dso_slv0_subadd_t                   slv0_subadd;
  lsm6dso_slv0_config_t                   slv0_config;
  lsm6dso_slv1_add_t                      slv1_add;
  lsm6dso_slv1_subadd_t                   slv1_subadd;
  lsm6dso_slv1_config_t                   slv1_config;
  lsm6dso_slv2_add_t                      slv2_add;
  lsm6dso_slv2_subadd_t                   slv2_subadd;
  lsm6dso_slv2_config_t                   slv2_config;
  lsm6dso_slv3_add_t                      slv3_add;
  lsm6dso_slv3_subadd_t                   slv3_subadd;
  lsm6dso_slv3_config_t                   slv3_config;
  lsm6dso_datawrite_src_mode_sub_slv0_t   datawrite_src_mode_sub_slv0;
  lsm6dso_status_master_t                 status_master;
  bitwise_t                               bitwise;
  uint8_t                                 byte;
} lsm6dso_reg_t;

/**
  * @}
  *
  */

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

/**
  * @}
  *
  */

#ifdef __cplusplus
}
#endif

#endif /*LSM6DSO_DRIVER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
