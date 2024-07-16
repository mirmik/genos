#ifndef POLYMORPH_ACCEL_H
#define POLYMORPH_ACCEL_H

#include <PolymorphSpeed.h>
#include <math.h>
#include <measure_units.h>

class PolymorphAccel
{
public:
    double _rot2internal = 1; // const
    double _unit2internal = 1;

    PositionUnit posunit = {};
    TimeUnit timeunit = {};
    PolymorphSpeed nominal_speed = {};

    bool inited_with_ms = false;
    double tempms = 0;
    double val = 0;

public:
    bool inited();

    PolymorphAccel() = default;
    PolymorphAccel(PositionUnit posunit, TimeUnit timeunit, double val);
    PolymorphAccel(const PolymorphAccel &other) = default;
    PolymorphAccel &operator=(const PolymorphAccel &other) = default;
    PolymorphAccel(PolymorphAccel &&other) = default;
    PolymorphAccel &operator=(PolymorphAccel &&other) = default;

    void set_nominal_speed(PolymorphSpeed spd);
    void set_rot2internal_multiplier(double mul);
    void set_unit2internal_multiplier(double mul);
    void set_milliseconds_for_nominal(double ms);
    double get_milliseconds_for_nominal() const;
    void set(PositionUnit posunit, TimeUnit timeunit, double val);
    double posunit_to_internal_multiplier(PositionUnit unit) const;
    double internal_to_posunit_multiplier(PositionUnit unit) const;

    void update_by_nominal_speed();

    double getvalue(PositionUnit target_posunit,
                    TimeUnit target_timeunit) const;
    double unit_per_second_sqr() const;
    double unit_per_minutes_sqr() const;
    double rotation_per_second_sqr() const;
    double rotation_per_minutes_sqr() const;
    double internalpulse_per_second_sqr() const;
    double internalpulse_per_minutes_sqr() const;
};

#endif