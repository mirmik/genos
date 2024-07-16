#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cstdint>
#include <trigger/direction.h>

// INFO AXIS
int32_t encoder_x();
int32_t encoder_y();

int32_t trigin_count();

// CONTROL AXIS
void impulse_x(int32_t);
void impulse_y(int32_t);

void speed_x(uint32_t speed);
void speed_y(uint32_t speed);

void numer_gen_x(uint32_t numer);
void numer_gen_y(uint32_t numer);

void denum_gen_x(uint32_t denum);
void denum_gen_y(uint32_t denum);

void spdmul_numer_x(uint32_t spdmul_numer);
void spdmul_numer_y(uint32_t spdmul_numer);

void spdmul_denum_x(uint32_t spdmul_denum);
void spdmul_denum_y(uint32_t spdmul_denum);

void direction_x(Direction dir);
void direction_y(Direction dir);

void stop_x();
void stop_y();
void stop();

void reset_encoder_x();
void reset_encoder_y();

void set_scan_cmpctr_x();
void set_scan_cmpctr_y();

// SCANER
void scan_command_send();
void scan_error_send();
void scan_stop_send();

void set_incpos_scan_stop_x(int32_t val);
void set_incpos_scan_strt_x(int32_t val);
void set_incpos_move_x(int32_t val);
void set_incpos_points_x(int32_t val);
void set_trigin_divider_x(int32_t val);

void set_incpos_scan_stop_y(int32_t val);
void set_incpos_scan_strt_y(int32_t val);
void set_incpos_move_y(int32_t val);
void set_incpos_points_y(int32_t val);
void set_trigin_divider_y(int32_t val);

void set_waittrigmode(int32_t val);
void set_bitrigmode(int32_t val);

// HELPERS
void timer4info();
void timer5info();
void timer10info();
void manual_trig_callback(bool val);

//__BEGIN_DECLS

void trigout_send_pulse();
void trigout_send_pulse_without_notify();
void trigout_send_pulse_fast_notify();

#ifdef BOARD_RL_SYNCRO_V2
void switch_mode_state0();
void switch_mode_state1();
void switch_mode_state0_aug();
void switch_mode_state1_aug();
void set_discrete_output_state(int num, int state);
void set_discrete_output_unical(int num);
void disable_all_discrete_outputs();
#endif

//__END_DECLS

#endif