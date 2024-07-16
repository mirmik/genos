#ifndef MAIN_H
#define MAIN_H

// class Axis;

// class PeriodicMoveController;
// class PeriodicCompareController;
// class TestDirectionController;

#include "axis.h"
#include "comparectr.h"
#include "configure.h"
#include "direction.h"
#include "functions.h"
#include "rs232.h"

#include <eventctr.h>
#include <utility/numcmd.h>
#include <zillot/common/uartring.h>

extern int64_t trigstart_time;
extern int32_t trigreturn_time;

extern struct numcmd_record numcmd_table[];
extern struct numcmd_context numcmd_cntxt;

extern MoveController *volatile current_controller_x;
extern MoveController *volatile current_controller_y;

extern PeriodicCompareController periodic_compare_x;
extern PeriodicCompareController periodic_compare_y;

extern int32_t trig_in_counter;
extern int32_t outimpsummary;

extern bool waittrigmode;
extern bool eventctr_mode;

extern zillot::uartring serial2_uartring;
extern genos::zillot_chardev serial2;
extern genos::zillot_chardev serial6;

extern bool trigin_info_mode;
extern bool eventctr_info_mode;

extern bool external_trigger_notify_mode;

void enable_external_trigger_notify_mode(int xy);

#endif