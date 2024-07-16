#ifndef MAIN_H
#define MAIN_H

#include <genos/ktimer.h>
#include <igris/shell/executor.h>
#include <nos/log/logger.h>
#include <ralgo/cnc/interpreter.h>
#include <ralgo/cnc/planner.h>
#include <ralgo/cnc/revolver.h>

extern nos::log::logger *syslog;

extern uint8_t crow_compiled_addr[64];
extern size_t crow_compiled_alen;

extern igris::syscmd_executor jexecutor;

extern cnc::revolver revolver;
extern cnc::planner planner;
extern cnc::feedback_guard feedback_guard;
extern cnc::interpreter interpreter;

void control_top_level();
void control_bottom_level();

void init_cnc();

void init_top_level();
void init_bottom_level();

void frequency_test_start();
void frequency_test_stop();

int32_t get_serve_handler_count();

#endif