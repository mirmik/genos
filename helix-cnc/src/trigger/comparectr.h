#ifndef COMPARE_CONTROLLER_H
#define COMPARE_CONTROLLER_H

#include <stdint.h>
#include <trigger/eventctr.h>
#include <trigger/direction.h>
#include <trigger/axis.h>
#include <trigger/functions.h>

#include <igris/event/delegate.h>

extern igris::delegate<void> trigin_handler;

class PeriodicCompareController
{
    bool waittrigmode = false;
    
public:
    bool trig_program_mode_enable = false;
    uint8_t *trig_program = nullptr;

    int trigger_error_counter = 0;
    igris::delegate<void> bitrigmode_first_state_handler = {};
    igris::delegate<void> bitrigmode_second_state_handler = {};
    bool bitrigmode = false;
    bool trigin_armed = false;

    int32_t start_enc = 0;
    bool removeflag = false;
    int triggers_points = 0;

    uint32_t incpos_scan_strt_zone = 0;
    uint32_t incpos_scan_stop_zone = 0;
    Direction incpos_direction = {};
    uint32_t incpos_move = 0;
    uint32_t incpos_points = 0;
    float compare_interval = 0;

    uint32_t count = 0;
    uint32_t count_in = 0;
    bool _runned = false;

public:
    Axis *axis = nullptr;
    igris::delegate<void> trigwait_hook = {};
    igris::delegate<void> pulse_generate = {};

    volatile bool trigwait = false;

    void init_new_scan();
    //void trigout_send_pulse();

    void trigger_handler();
    void set_start_position();

    void triginHandler();
    void trigoutHandler();

    void *exec();
    void remove();
    void eventctrTrigoutHandler();

    void scan_command_send();
    void scan_stop_send();
    void scan_error_send();

    void set_points(int points);
    void start();
    void finish();

    bool runned();
};

extern PeriodicCompareController periodic_compare_x;
extern PeriodicCompareController periodic_compare_y;

#endif