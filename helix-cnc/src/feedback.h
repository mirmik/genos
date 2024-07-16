#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <array>
#include <board_defs.h>
#include <string>

void axes_feedback_init();
std::string encoder_state_as_string();
std::array<int64_t, TOTAL_CTRL_AXES> feedback_position_as_array();
void feedback_position_set(size_t ax, int64_t pos);

#endif