/**
    @file
*/

#include <Axis.h>

#include <chrono>
#include <thread>

#include <tables.h>

using namespace std::chrono_literals;
extern bool disable_update;

extern std::vector<uint8_t> crowaddr;
std::vector<std::string> remoter_axes_order;

int remoter_axes_total()
{
    if (remoter_axes_order.size() == 0)
    {
        return axes_total();
    }

    else
    {
        return remoter_axes_order.size();
    }
}

AbstractAxis *get_remoter_axis(int axno)
{
    // if (remoter_axes_order.size() == 0)
    // {
    //     return getActiveAxis(axno);
    // }

    // else
    // {
        return get_axis(remoter_axes_order[axno]);
    //}
}

int remote_axis_jog(unsigned int axno, double spd)
{

    // if (axno > active_axes_list.size())
    //     return -1;

    AbstractAxis *ax = get_remoter_axis(axno - 1);
    ax->set_speed_procent(spd);

    ax->absoluteUnitMove(spd > 0 ? ax->unitForwardLimit_protected() - 0.001f
                                 : ax->unitBackwardLimit_protected() + 0.001f);

    return 0;
}

int remote_axis_incpos(unsigned int axno, double pos)
{
    // if (axno > active_axes_list.size())
    //     return -1;

    AbstractAxis *ax = get_remoter_axis(axno - 1);
    ax->setSpeed_rpm(ax->defaultSpeed_rpm());
    ax->setAccel_ms(ax->defaultAccel_ms());

    ax->relativeUnitMove(pos);

    return 0;
}

int remote_stop()
{
    for (auto ax : get_axis_list())
        ax->stop();

    return 0;
}

void set_remoter_order(std::vector<std::string> vec)
{
    remoter_axes_order = vec;
}