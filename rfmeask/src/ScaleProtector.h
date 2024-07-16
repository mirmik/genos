#ifndef AXIS_CONTROL_SETTINGS_H
#define AXIS_CONTROL_SETTINGS_H

#include <algorithm>

class AxisControlSettings
{
    double gain = 1;
    double internal_forwlim = 0;
    double internal_backlim = 0;

    bool reverse = false;
    bool limits_enable = false;

public:
    AxisControlSettings() = default;

    double ext2int(double pos)
    {
        return pos * gain * (reverse ? -1 : 1);
    }

    void set_gain(double gain)
    {
        this->gain = gain;
    }

    void set_internal_limits(double backlim, double forwlim)
    {
        if (backlim > forwlim)
            return;

        this->internal_backlim = backlim;
        this->internal_forwlim = forwlim;
        this->limits_enable = true;
    }

    void set_external_limits(double backlim, double forwlim)
    {
        if (backlim > forwlim)
            return;

        this->internal_backlim = ext2int(backlim);
        this->internal_forwlim = ext2int(forwlim);
        this->limits_enable = true;
    }

    double protect_internal_absolute_position(double pos)
    {
        return std::clamp(pos, internal_backlim, internal_forwlim);
    }

    double protect_ext2int_absolute_position(double pos)
    {
        double intpos = ext2int(pos);
        return protect_internal_absolute_position(intpos);
    }
};

#endif