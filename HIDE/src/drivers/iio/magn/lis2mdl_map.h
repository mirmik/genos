#ifndef LIS2MDL_MAP_H
#define LIS2MDL_MAP_H

#define LIS2MDL_OFFSET_X_REG_L          0x45U
#define LIS2MDL_OFFSET_X_REG_H          0x46U
#define LIS2MDL_OFFSET_Y_REG_L          0x47U
#define LIS2MDL_OFFSET_Y_REG_H          0x48U
#define LIS2MDL_OFFSET_Z_REG_L          0x49U
#define LIS2MDL_OFFSET_Z_REG_H          0x4AU
#define LIS2MDL_WHO_AM_I                0x4FU
#define LIS2MDL_CFG_REG_A               0x60U
typedef struct {
  uint8_t md                     : 2;
  uint8_t odr                    : 2;
  uint8_t lp                     : 1;
  uint8_t soft_rst               : 1;
  uint8_t reboot                 : 1;
  uint8_t comp_temp_en           : 1;
} lis2mdl_cfg_reg_a_t;

#define LIS2MDL_CFG_REG_B               0x61U
typedef struct {
  uint8_t lpf                    : 1;
  uint8_t set_rst                : 2; /* OFF_CANC + Set_FREQ */
  uint8_t int_on_dataoff         : 1;
  uint8_t off_canc_one_shot      : 1;
  uint8_t not_used_01            : 3;
} lis2mdl_cfg_reg_b_t;

#define LIS2MDL_CFG_REG_C               0x62U
typedef struct {
  uint8_t drdy_on_pin            : 1;
  uint8_t self_test              : 1;
  uint8_t not_used_01            : 1;
  uint8_t ble                    : 1;
  uint8_t bdu                    : 1;
  uint8_t i2c_dis                : 1;
  uint8_t int_on_pin             : 1;
  uint8_t not_used_02            : 1;
} lis2mdl_cfg_reg_c_t;

#define LIS2MDL_INT_CRTL_REG            0x63U
typedef struct {
  uint8_t ien                    : 1;
  uint8_t iel                    : 1;
  uint8_t iea                    : 1;
  uint8_t not_used_01            : 2;
  uint8_t zien                   : 1;
  uint8_t yien                   : 1;
  uint8_t xien                   : 1;
} lis2mdl_int_crtl_reg_t;

#define LIS2MDL_INT_SOURCE_REG          0x64U
typedef struct {
  uint8_t _int                    : 1;
  uint8_t mroi                   : 1;
  uint8_t n_th_s_z               : 1;
  uint8_t n_th_s_y               : 1;
  uint8_t n_th_s_x               : 1;
  uint8_t p_th_s_z               : 1;
  uint8_t p_th_s_y               : 1;
  uint8_t p_th_s_x               : 1;
} lis2mdl_int_source_reg_t;

#define LIS2MDL_INT_THS_L_REG           0x65U
#define LIS2MDL_INT_THS_H_REG           0x66U
#define LIS2MDL_STATUS_REG              0x67U
typedef struct {
  uint8_t xda                    : 1;
  uint8_t yda                    : 1;
  uint8_t zda                    : 1;
  uint8_t zyxda                  : 1;
  uint8_t xior                   : 1;  //changed as xor is a reserved word in c++
  uint8_t yor                    : 1;
  uint8_t zor                    : 1;
  uint8_t zyxor                  : 1;
} lis2mdl_status_reg_t;

#define LIS2MDL_OUTX_L_REG              0x68U
#define LIS2MDL_OUTX_H_REG              0x69U
#define LIS2MDL_OUTY_L_REG              0x6AU
#define LIS2MDL_OUTY_H_REG              0x6BU
#define LIS2MDL_OUTZ_L_REG              0x6CU
#define LIS2MDL_OUTZ_H_REG              0x6DU
#define LIS2MDL_TEMP_OUT_L_REG          0x6EU
#define LIS2MDL_TEMP_OUT_H_REG          0x6FU

#endif