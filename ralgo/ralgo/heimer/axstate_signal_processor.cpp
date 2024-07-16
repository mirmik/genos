#include <ralgo/heimer/axstate_signal_processor.h>
#include <ralgo/heimer/sigtypes.h>
#include <ralgo/log.h>

heimer::axstate_signal_processor::axstate_signal_processor(const char *name,
                                                           int ldim,
                                                           int rdim)
    : heimer::signal_processor(name, ldim, rdim)
{
}

void heimer::axstate_signal_processor::attach_leftside_table(axis_state **table)
{
    _leftside = table;
}

void heimer::axstate_signal_processor::attach_rightside_table(
    axis_state **table)
{
    _rightside = table;
}
