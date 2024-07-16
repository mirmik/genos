#include <board_defs.h>
#include <control.h>
#include <crow-service.h>
#include <feedback.h>
#include <igris/creader.h>
#include <igris/shell/rshell_executor.h>
#include <main.h>
#include <nos/shell/executor.h>
#include <quadgen.h>
#include <ralgo/cnc/feedback_guard.h>
#include <ralgo/robo/quadgen.h>
#include <sizes.h>

extern robo::quadgen *control_quadgens[TOTAL_CTRL_AXES];
robo::stepper *steppers[TOTAL_CTRL_AXES];

igris::ring<cnc::planner_block> planner_ring(PLANER_RING_SIZE);

cnc::revolver revolver;
cnc::planner planner(&planner_ring, &revolver);
cnc::feedback_guard feedback_guard(TOTAL_CTRL_AXES);
cnc::interpreter
    interpreter(&planner_ring, &planner, &revolver, &feedback_guard);

void init_cnc()
{
    for (int i = 0; i < TOTAL_CTRL_AXES; ++i)
        steppers[i] = control_quadgens[i];

    revolver.set_steppers(steppers, TOTAL_CTRL_AXES);
    interpreter.set_revolver_frequency(fast_cycle_frequence());
    interpreter.init_axes(TOTAL_CTRL_AXES);
    interpreter.set_final_shift_handler(&send_final_status_handle);
    interpreter.set_start_shift_handler(&send_start_status_handle);
    planner.info_mode = true;

    feedback_guard.set_set_feedback_position_by_axis_callback(
        feedback_position_set);
}
