/**
    @file
*/

#include <Axis.h>

#include <chrono>
#include <thread>

#include <tables.h>
#include <servos/AbstractServo.h>

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
        //return get_axis(remoter_axes_order[axno]);
    //}
    return get_axis(axno);
}

int remote_axis_jog(unsigned int axno, double spd)
{
    nos::println("remote_axis_jog:", axno, spd);
    
    // if (axno > active_axes_list.size())
    //     return -1;


    prevent_updates = true;
    AbstractAxis *ax = get_remoter_axis(axno - 1);
    ax->set_speed_procent(spd);

    ax->absoluteUnitMove(spd > 0 ? ax->unitForwardLimit_protected() - 0.001f
                                 : ax->unitBackwardLimit_protected() + 0.001f);

    
    prevent_updates = false;
    return 0;
}

int remote_axis_incpos(unsigned int axno, double pos)
{
    // if (axno > active_axes_list.size())
    //     return -1;

    prevent_updates = true;
    AbstractAxis *ax = get_remoter_axis(axno - 1);
    ax->setSpeed_rpm(ax->defaultSpeed_rpm());
    ax->setAccel_ms(ax->defaultAccel_ms());

    ax->relativeUnitMove(pos);

    prevent_updates = false;
    return 0;
}

int remote_stop()
{
    prevent_updates = true;
    for (auto ax : get_axis_list())
        ax->stop();

    prevent_updates = false;
    return 0;
}

void set_remoter_order(std::vector<std::string> vec)
{
    remoter_axes_order = vec;
}