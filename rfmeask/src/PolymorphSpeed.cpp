#include <PolymorphSpeed.h>
#include <nos/print.h>

bool PolymorphSpeed::inited()
{
    return _rot2internal != 0 && _unit2internal != 0;
}

PolymorphSpeed::PolymorphSpeed(PositionUnit posunit,
                               TimeUnit timeunit,
                               double val) :
    posunit(posunit), timeunit(timeunit), val(val)
{
}

void PolymorphSpeed::set_rot2internal_multiplier(double mul)
{
    _rot2internal = mul;
}

void PolymorphSpeed::set_unit2internal_multiplier(double mul)
{
    _unit2internal = mul;
}

void PolymorphSpeed::set(PositionUnit posunit, TimeUnit timeunit, double val)
{
    this->val = val;
    this->posunit = posunit;
    this->timeunit = timeunit;
}

double PolymorphSpeed::posunit_to_internal_multiplier(PositionUnit unit) const
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

double PolymorphSpeed::internal_to_posunit_multiplier(PositionUnit unit) const
{
    return 1 / posunit_to_internal_multiplier(unit);
}

double PolymorphSpeed::getvalue(PositionUnit target_posunit,
                                TimeUnit target_timeunit) const
{
    double result = val * posunit_to_internal_multiplier(posunit) *
                    internal_to_posunit_multiplier(target_posunit) *
                    seconds_to_timeunit_multiplier(timeunit) *
                    timeunit_to_seconds_multiplier(target_timeunit);

    if (isinf(result) || isnan(result))
    {
        nos::println("WARN: Invalid speed.");
        return 0;
    }
    else
        return result;
}

double PolymorphSpeed::unit_per_second() const
{
    return getvalue(EXTERNAL, SECONDS);
}
double PolymorphSpeed::unit_per_minut() const
{
    return getvalue(EXTERNAL, MINUTES);
}
double PolymorphSpeed::rotation_per_second() const
{
    return getvalue(ROTATION, SECONDS);
}
double PolymorphSpeed::rotation_per_minut() const
{
    return getvalue(ROTATION, MINUTES);
}
double PolymorphSpeed::internalpulse_per_second() const
{
    return getvalue(INTERNALPULSE, SECONDS);
}
double PolymorphSpeed::internalpulse_per_minut() const
{
    return getvalue(INTERNALPULSE, MINUTES);
}
