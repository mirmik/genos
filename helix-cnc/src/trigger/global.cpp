#include <trigger/global.h>

volatile bool notify_on_answer_pulse = false;
bool eventctr_mode = false;
bool waittrigmode = false;

igris::delegate<void> trigin_handler = {};
uint32_t trig_in_precounter = 0;
int disable_trigin_mode = 0;
int32_t trig_in_counter = 0;
int32_t trigreturn_time = 0;
int64_t trigstart_time = 0;
uint32_t trigin_divider = 0;
