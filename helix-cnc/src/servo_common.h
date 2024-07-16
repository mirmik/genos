#ifndef SERVO_COMMON_H
#define SERVO_COMMON_H

#include <igris/container/array_view.h>
#include <igris/container/flat_map_view.h>
#include <string_view>

void operation_finish_handle(void * args);
void serve_handler(void * arg);

#endif