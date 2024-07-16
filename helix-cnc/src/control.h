#ifndef CONTROL_H
#define CONTROL_H

#include <nos/shell/executor.h>
#include <nos/shell/aggregate_executor.h>

extern nos::aggregate_executor system_executor;

double fast_cycle_frequence();
void init_cnc();

#endif