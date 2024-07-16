#ifndef RS232_H
#define RS232_H

#include "main.h"

// extern AdapterSerial ASerial6;
// extern Serial_t<512,512> Serial6;
// extern SerToSer S2S;
// extern numcmd_list Ncmd;

void rs232_init();
uint32_t com_set_imp_x(uint32_t imp);
uint32_t com_set_spd_x(uint32_t speed);
uint32_t com_set_stp_x(uint32_t str);
uint32_t com_set_dir_x(uint32_t str);
uint32_t com_testlink(uint32_t);
uint32_t com_ctr_tst_x(uint32_t str);
uint32_t com_ctr_scan_x(uint32_t str);
uint32_t com_incpos_scan_strt_x(uint32_t val);
uint32_t com_incpos_scan_stop_x(uint32_t val);
uint32_t com_incpos_move_stop_x(uint32_t val);
uint32_t com_incpos_points_x(uint32_t val);
uint32_t com_trigin_divider_x(uint32_t val);
uint32_t com_set_enc_numer_x(uint32_t val);
uint32_t com_set_enc_denum_x(uint32_t val);
uint32_t com_set_gen_numer_x(uint32_t val);
uint32_t com_set_gen_denum_x(uint32_t val);
uint32_t com_enable_external_trigger_notify(uint32_t val);
uint32_t com_get_version();

#endif