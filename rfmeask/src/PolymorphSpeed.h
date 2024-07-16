#ifndef POLYMORPH_SPEED_H
#define POLYMORPH_SPEED_H

#include <math.h>
#include <measure_units.h>

class PolymorphSpeed
{
public:
    double _rot2internal = 1; // const
    double _unit2internal = 1;

    PositionUnit posunit = {};
    TimeUnit timeunit = {};

    double val = 0;

public:
    bool inited();

    PolymorphSpeed() = default;
    PolymorphSpeed(PositionUnit posunit, TimeUnit timeunit, double val);

    PolymorphSpeed(const PolymorphSpeed &other) = default;
    PolymorphSpeed &operator=(const PolymorphSpeed &other) = default;
    PolymorphSpeed(PolymorphSpeed &&other) = default;
    PolymorphSpeed &operator=(PolymorphSpeed &&other) = default;

    void set_rot2internal_multiplier(double mul);
    void set_unit2internal_multiplier(double mul);
    void set(PositionUnit posunit, TimeUnit timeunit, double val);

    double posunit_to_internal_multiplier(PositionUnit unit) const;

    double internal_to_posunit_multiplier(PositionUnit unit) const;

    double getvalue(PositionUnit target_posunit,
                    TimeUnit target_timeunit) const;
    double unit_per_second() const;
    double unit_per_minut() const;
    double rotation_per_second() const;
    double rotation_per_minut() const;
    double internalpulse_per_second() const;
    double internalpulse_per_minut() const;
};

#endif