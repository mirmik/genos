
#include <PolymorphAccel.h>
#include <igris/util/bug.h>
#include <nos/print.h>
#include <cassert>

bool PolymorphAccel::inited()
{
    return _rot2internal != 0 && _unit2internal != 0;
}

PolymorphAccel::PolymorphAccel(PositionUnit posunit,
                               TimeUnit timeunit,
                               double val) :
    posunit(posunit), timeunit(timeunit), val(val)
{
}

void PolymorphAccel::set_nominal_speed(PolymorphSpeed spd)
{
    nominal_speed = spd;
    if (inited_with_ms)
    {
        update_by_nominal_speed();
    }
}

void PolymorphAccel::set_rot2internal_multiplier(double mul)
{
    _rot2internal = mul;
    nominal_speed.set_rot2internal_multiplier(mul);
    if (inited_with_ms)
    {
        update_by_nominal_speed();
    }
}

void PolymorphAccel::set_unit2internal_multiplier(double mul)
{
    _unit2internal = mul;
    nominal_speed.set_unit2internal_multiplier(mul);
    if (inited_with_ms)
    {
        update_by_nominal_speed();
    }
}

void PolymorphAccel::set_milliseconds_for_nominal(double ms)
{
    inited_with_ms = true;
    tempms = ms;
}

void PolymorphAccel::update_by_nominal_speed()
{
    double nominal_servo_speed_rps =
        nominal_speed.getvalue(PositionUnit::ROTATION, TimeUnit::SECONDS);
    double acceleration_time_s = tempms / 1000.;
    double rpss = nominal_servo_speed_rps / acceleration_time_s;
    set(PositionUnit::ROTATION, TimeUnit::SECONDS, rpss);
}

double PolymorphAccel::get_milliseconds_for_nominal() const
{
    double rpss = rotation_per_second_sqr();
    double nominal_servo_speed_rps = nominal_speed.rotation_per_second();
    double acceleration_time_s = nominal_servo_speed_rps / rpss;
    double acceleration_time_ms = acceleration_time_s * 1000.;
    return acceleration_time_ms;
}

void PolymorphAccel::set(PositionUnit posunit, TimeUnit timeunit, double val)
{
    this->val = val;
    this->posunit = posunit;
    this->timeunit = timeunit;
}

double PolymorphAccel::posunit_to_internal_multiplier(PositionUnit unit) const
{
    switch (unit)
    {
    case PositionUnit::INTERNALPULSE:
        return 1.;
    case PositionUnit::ROTATION:
        return _rot2internal;
    case PositionUnit::EXTERNAL:
        return _unit2internal;
    }
    return 0;
}

double PolymorphAccel::internal_to_posunit_multiplier(PositionUnit unit) const
{
    return 1 / posunit_to_internal_multiplier(unit);
}

double PolymorphAccel::getvalue(PositionUnit target_posunit,
                                TimeUnit target_timeunit) const
{
    assert(val != 0);
    assert(posunit_to_internal_multiplier(posunit) != 0);
    assert(internal_to_posunit_multiplier(target_posunit) != 0);
    assert(timeunit_to_seconds_multiplier(timeunit) != 0);
    assert(seconds_to_timeunit_multiplier(target_timeunit) != 0);

    double result = val * posunit_to_internal_multiplier(posunit) *
                    internal_to_posunit_multiplier(target_posunit) *
                    timeunit_to_seconds_multiplier(timeunit) *
                    seconds_to_timeunit_multiplier(target_timeunit) *
                    timeunit_to_seconds_multiplier(timeunit) *
                    seconds_to_timeunit_multiplier(target_timeunit);

    if (isinf(result) || isnan(result))
    {
        nos::println("WARN: Invalid acceleration.");
        return 0;
    }
    else
        return result;
}

double PolymorphAccel::unit_per_second_sqr() const
{
    return getvalue(EXTERNAL, SECONDS);
}
double PolymorphAccel::unit_per_minutes_sqr() const
{
    return getvalue(EXTERNAL, MINUTES);
}
double PolymorphAccel::rotation_per_second_sqr() const
{
    return getvalue(ROTATION, SECONDS);
}
double PolymorphAccel::rotation_per_minutes_sqr() const
{
    return getvalue(ROTATION, MINUTES);
}
double PolymorphAccel::internalpulse_per_second_sqr() const
{
    return getvalue(INTERNALPULSE, SECONDS);
}
double PolymorphAccel::internalpulse_per_minutes_sqr() const
{
    return getvalue(INTERNALPULSE, MINUTES);
}
