#ifndef RFMEASK_CONSOLE_H
#define RFMEASK_CONSOLE_H

#include <nos/shell/executor.h>

void start_tcp_console_thread(int port);
void start_io_console_thread();

#endif