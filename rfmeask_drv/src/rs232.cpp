#include "main.h"

#include <igris/dprint.h>
#include <igris/protocols/numcmd/numcmd.h>
#include <igris/util/bug.h>
#include <version.h>

#include <comparectr.h>

uint32_t com_set_imp_x(uint32_t imp)
{
    return 0;
}
uint32_t com_set_imp_y(uint32_t imp)
{
    return 0;
}

uint32_t com_set_stp_x(uint32_t str)
{
    stop_x();
    return 0;
}
uint32_t com_set_stp_y(uint32_t str)
{
    stop_y();
    return 0;
}

uint32_t com_set_dir_x(uint32_t str)
{
    return 0;
}
uint32_t com_set_dir_y(uint32_t str)
{
    return 0;
}

uint32_t com_set_phase_x(uint32_t arg)
{
    return 0;
}
uint32_t com_set_phase_y(uint32_t arg)
{
    return 0;
}

uint32_t com_ctr_tst_x(uint32_t str)
{
    return 0;
}
uint32_t com_ctr_tst_y(uint32_t str)
{
    return 0;
}

uint32_t com_ctr_scan_x(uint32_t str)
{
    return 0;
}
uint32_t com_ctr_scan_y(uint32_t str)
{
    return 0;
}

extern int GLOBAL_scan_command_counter;
uint32_t com_cmpctr_scan_x(uint32_t str)
{
    GLOBAL_scan_command_counter = 0; // reset counter for eventctr mode
    set_scan_cmpctr_x();
    return 0;
}
uint32_t com_cmpctr_scan_y(uint32_t str)
{
    GLOBAL_scan_command_counter = 0; // reset counter for eventctr mode
    set_scan_cmpctr_y();
    return 0;
}

uint32_t com_incpos_scan_strt_x(uint32_t val)
{
    set_incpos_scan_strt_x(val);
    return 0;
}
uint32_t com_incpos_scan_strt_y(uint32_t val)
{
    set_incpos_scan_strt_y(val);
    return 0;
}

uint32_t com_incpos_scan_stop_x(uint32_t val)
{
    set_incpos_scan_stop_x(val);
    return 0;
}
uint32_t com_incpos_scan_stop_y(uint32_t val)
{
    set_incpos_scan_stop_y(val);
    return 0;
}

uint32_t com_incpos_move_x(uint32_t val)
{
    set_incpos_move_x(val);
    return 0;
}
uint32_t com_incpos_move_y(uint32_t val)
{
    set_incpos_move_y(val);
    return 0;
}

uint32_t com_incpos_points_x(uint32_t val)
{
    set_incpos_points_x(val);
    return 0;
}
uint32_t com_incpos_points_y(uint32_t val)
{
    set_incpos_points_y(val);
    return 0;
}

uint32_t com_trigin_divider_x(uint32_t val)
{
    set_trigin_divider_x(val);
    return 0;
}
uint32_t com_trigin_divider_y(uint32_t val)
{
    set_trigin_divider_y(val);
    return 0;
}

uint32_t com_set_enc_numer_x(uint32_t val)
{
    axis_x.enc_numer = val;
    return 0;
}
uint32_t com_set_enc_numer_y(uint32_t val)
{
    axis_y.enc_numer = val;
    return 0;
}

uint32_t com_set_enc_denum_x(uint32_t val)
{
    axis_x.enc_denum = val;
    return 0;
}
uint32_t com_set_enc_denum_y(uint32_t val)
{
    axis_y.enc_denum = val;
    return 0;
}

uint32_t com_set_gen_numer_x(uint32_t val)
{
    numer_gen_x(val);
    return 0;
}
uint32_t com_set_gen_numer_y(uint32_t val)
{
    numer_gen_y(val);
    return 0;
}

uint32_t com_set_gen_denum_x(uint32_t val)
{
    denum_gen_x(val);
    return 0;
}
uint32_t com_set_gen_denum_y(uint32_t val)
{
    denum_gen_y(val);
    return 0;
}

uint32_t com_set_spdmul_numer_x(uint32_t val)
{
    spdmul_numer_x(val);
    return 0;
}
uint32_t com_set_spdmul_numer_y(uint32_t val)
{
    spdmul_numer_y(val);
    return 0;
}

uint32_t com_set_spdmul_denum_x(uint32_t val)
{
    spdmul_denum_x(val);
    return 0;
}
uint32_t com_set_spdmul_denum_y(uint32_t val)
{
    spdmul_denum_y(val);
    return 0;
}

uint32_t com_set_spd_x(uint32_t speed)
{
    speed_x(speed);
    return 0;
}
uint32_t com_set_spd_y(uint32_t speed)
{
    speed_y(speed);
    return 0;
}

uint32_t com_get_enc_pos_x(uint32_t val)
{
    return axis_x.read_encoder();
}
uint32_t com_get_enc_pos_y(uint32_t val)
{
    return axis_y.read_encoder();
}

uint32_t com_nac_pulse(uint32_t val)
{
    trigout_send_pulse();
    return 0;
}

uint32_t com_testlink(uint32_t val)
{
    return version();
}

uint32_t com_waittrigmode(uint32_t val)
{
    set_waittrigmode(val);
    return 0;
}

uint32_t com_bitrigmode(uint32_t val)
{
    // Bitrig mode removed at 06.04.2023
    return 0;
}

uint32_t com_manual_trig_callback(uint32_t val)
{
    manual_trig_callback(val);
    return 0;
}

uint32_t prototest(uint32_t val)
{
    return 0;
}

uint32_t eventctr_reinit_table(uint32_t val)
{
    return 0;
}

uint32_t eventctr_reset_table_prefix(uint32_t val)
{
    eventctr_mode = true;
    eventctr.reset_table_prefix(val);
    return 0;
}

uint32_t eventctr_reset_table_cycle(uint32_t val)
{
    eventctr_mode = true;
    eventctr.reset_table_cycle(val);
    return 0;
}

uint32_t eventctr_reset_table_postfix(uint32_t val)
{
    eventctr_mode = true;
    eventctr.reset_table_postfix(val);
    return 0;
}

uint32_t eventctr_push_cmd(uint32_t val)
{
    eventctr.push_back(val);
    return 0;
}

uint32_t eventctr_save_table(uint32_t val)
{
    eventctr.save_table(val);
    return 0;
}

uint32_t eventctr_load_table(uint32_t val)
{
    eventctr.load_table(val);
    return 0;
}

uint32_t eventctr_set_loop_total(uint32_t val)
{
    eventctr.set_loop_total(val);
    return 0;
}

uint32_t get_trigreturn_time(uint32_t val)
{
    return (uint32_t)trigreturn_time;
}

uint32_t preset(uint32_t val)
{
    eventctr_mode = false;
    return 0;
}

static uint32_t discout(uint32_t val)
{
    uint16_t num = (val & 0xffff0000) >> 16;
    uint16_t state = (val & 0x0000ffff);

    set_discrete_output_state(num, state);
    return 0;
}

static uint32_t unicout(uint32_t val)
{
    uint16_t num = val;

    set_discrete_output_unical(num);
    return 0;
}

static uint32_t disouts(uint32_t val)
{
    disable_all_discrete_outputs();
    return 0;
}

uint32_t com_get_version(uint32_t val)
{
    (void)val;
    return version();
}

uint32_t com_clear_compare_table_x(uint32_t val)
{
    periodic_compare_x.clear_compare_table();
    return 0;
}

uint32_t com_clear_compare_table_y(uint32_t val)
{
    periodic_compare_y.clear_compare_table();
    return 0;
}

uint32_t com_add_compare_table_point_x(uint32_t val)
{
    periodic_compare_x.add_compare_table_point(val);
    return 0;
}

uint32_t com_add_compare_table_point_y(uint32_t val)
{
    periodic_compare_y.add_compare_table_point(val);
    return 0;
}

uint32_t com_enable_compare_table_x(uint32_t val)
{
    periodic_compare_x.enable_compare_table(val);
    return 0;
}

uint32_t com_enable_compare_table_y(uint32_t val)
{
    periodic_compare_y.enable_compare_table(val);
    return 0;
}

uint32_t com_reset_x(uint32_t val)
{
    periodic_compare_x.reset();
    return 0;
}

uint32_t com_reset_y(uint32_t val)
{
    periodic_compare_y.reset();
    return 0;
}

uint32_t com_enable_external_trigger_notify_x(uint32_t val)
{
    enable_external_trigger_notify_mode(0);
    return 0;
}

uint32_t com_enable_external_trigger_notify_y(uint32_t val)
{
    enable_external_trigger_notify_mode(1);
    return 0;
}

uint32_t com_trigger_polarity(uint32_t val)
{
    int reverse_polarity = val;
    tim10_configure_polarity(reverse_polarity);
    return 0;
}

uint32_t com_trigger_duration(uint32_t val)
{
    int duration = val;
    tim10_configure_duration(duration); 
    return 0;
}

struct numcmd_record numcmd_table[] = {
    {0, prototest},
    {1, com_set_stp_x},
    {3, com_set_imp_x},
    {4, com_set_spd_x},
    {5, com_set_dir_x},
    {12, com_ctr_tst_x},
    {14, com_ctr_scan_x},
    {15, com_cmpctr_scan_x},
    {18, com_set_spdmul_numer_x},
    {19, com_set_spdmul_denum_x},
    {20, com_set_gen_numer_x},
    {21, com_set_gen_denum_x},
    {22, com_set_enc_numer_x},
    {23, com_set_enc_denum_x},
    {24, com_incpos_scan_strt_x},
    {25, com_incpos_scan_stop_x},
    {26, com_incpos_move_x},
    {27, com_incpos_points_x},
    {30, com_set_phase_x},
    {55, com_trigin_divider_x},
    {56, com_get_enc_pos_x},
    {57, com_enable_external_trigger_notify_x},

    {28, com_waittrigmode},
    {29, com_bitrigmode},
    {61, get_trigreturn_time},
    {62, com_manual_trig_callback},
    {63, com_nac_pulse},
    {64, com_testlink},

    {65, eventctr_push_cmd},
    {66, eventctr_reinit_table},
    {67, eventctr_save_table},
    {68, eventctr_load_table},

    {69, eventctr_reset_table_prefix},
    {70, eventctr_reset_table_cycle},
    {71, eventctr_reset_table_postfix},
    {72, eventctr_set_loop_total},

    {73, preset},

    // Функции управления дискретными выводами.
    {74, discout},
    {75, unicout},
    {76, disouts},
    {77, com_get_version},

    // Таблица сравнений.
    {78, com_clear_compare_table_x},
    {79, com_add_compare_table_point_x},
    {80, com_enable_compare_table_x},
    {81, com_reset_x},
    
    {82, com_trigger_polarity},
    {83, com_trigger_duration},

    {128 + 1, com_set_stp_y},
    {128 + 3, com_set_imp_y},
    {128 + 4, com_set_spd_y},
    {128 + 5, com_set_dir_y},
    {128 + 12, com_ctr_tst_y},
    {128 + 14, com_ctr_scan_y},
    {128 + 15, com_cmpctr_scan_y},
    {128 + 18, com_set_spdmul_numer_y},
    {128 + 19, com_set_spdmul_denum_y},
    {128 + 20, com_set_gen_numer_y},
    {128 + 21, com_set_gen_denum_y},
    {128 + 22, com_set_enc_numer_y},
    {128 + 23, com_set_enc_denum_y},
    {128 + 24, com_incpos_scan_strt_y},
    {128 + 25, com_incpos_scan_stop_y},
    {128 + 26, com_incpos_move_y},
    {128 + 27, com_incpos_points_y},
    {128 + 30, com_set_phase_y},
    {128 + 55, com_trigin_divider_y},
    {128 + 56, com_get_enc_pos_y},
    {128 + 57, com_enable_external_trigger_notify_y},
    {128 + 64, com_testlink},

    // Таблица сравнений.
    {128 + 78, com_clear_compare_table_y},
    {128 + 79, com_add_compare_table_point_y},
    {128 + 80, com_enable_compare_table_y},
    {128 + 81, com_reset_y},

    NUMCMD_TBLFIN};
