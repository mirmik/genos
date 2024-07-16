#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <nos/shell/aggregate_executor.h>
extern nos::aggregate_executor system_executor;

void disable_bottom_level();
void enable_bottom_level();

#endif