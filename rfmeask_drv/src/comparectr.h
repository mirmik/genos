#ifndef COMPARE_CONTROLLER_H
#define COMPARE_CONTROLLER_H

#include "axis.h"
#include "distance_table.h"
#include "inttypes.h"
#include "movectr.h"

class PeriodicCompareController : public MoveController
{
private:
    int trigger_error_counter = 0;
    bool trigin_armed = false;
    int32_t start_enc = 0;
    uint32_t incpos_scan_strt_zone = 0;
    uint32_t incpos_scan_stop_zone = 0;
    Direction incpos_direction = {};
    uint32_t incpos_move = 0;
    uint32_t incpos_points = 0;
    double compare_interval = 0;
    uint32_t waited_point_counter = 0;
    uint32_t count_in = 0;
    distance_table disttbl = {};
    bool use_distance_table = false;
    Axis *axis = nullptr;
    igris::delegate<void> trigwait_hook = {};
    igris::delegate<void> pulse_generate = {};
    volatile bool trigwait = false;

public:
    void set_next_point_to_compare_or_disable_compairing_if_last();
    void clear_compare_table();
    void add_compare_table_point(int32_t enc);
    void init_new_scan();
    void reset();
    int64_t counter_what_we_want();
    void trigger_handler();
    void set_start_position();
    void simpleMode_triginHandler();
    void simpleMode_trigoutHandler();
    int exec() override;
    void remove_impl();
    void eventctrTrigoutHandler();
    void enable_compare_table(bool en);
    int32_t next_compare_point();

    void init_state(Axis *ax)
    {
        incpos_scan_strt_zone = 0;
        incpos_scan_stop_zone = 0;
        incpos_direction = DIRECTION_FORWARD;
        incpos_move = 4000000;
        incpos_points = 10;
        axis = ax;
    }

    void set_trigwait_state(bool en)
    {
        trigwait = en;
    }

    bool is_trigwait_state()
    {
        return trigwait;
    }

    void set_trigwait_hook(igris::delegate<void> trigwait_hook)
    {
        this->trigwait_hook = trigwait_hook;
    }

    void set_incpos_scan_stop_zone(uint32_t incpos_scan_stop)
    {
        this->incpos_scan_stop_zone = incpos_scan_stop;
    }

    void set_incpos_scan_strt_zone(uint32_t incpos_scan_start)
    {
        this->incpos_scan_strt_zone = incpos_scan_start;
    }

    void set_incpos_points(uint32_t incpos_points)
    {
        this->incpos_points = incpos_points;
    }

    void set_incpos_move(uint32_t incpos_move)
    {
        this->incpos_move = incpos_move;
    }

    void set_incpos_direction(Direction incpos_direction)
    {
        this->incpos_direction = incpos_direction;
    }
};

#endif