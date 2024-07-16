#ifndef SCANNING_COMMANDS_H
#define SCANNING_COMMANDS_H

#include <nos/shell/executor.h>

int scan_compare_start(const nos::argv&, nos::ostream& os);
int set_scan_points(const nos::argv&, nos::ostream& os);
int set_scan_distance(const nos::argv&, nos::ostream& os);

extern nos::executor trigger_executor;

#endif