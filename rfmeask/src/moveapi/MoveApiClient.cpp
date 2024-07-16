
#include <CommonAxis.h>
#include <moveapi/MoveApiClient.h>
#include <nos/fprint.h>

using namespace nos::argument_literal;

bool MoveApiClient::is_reversed()
{
    return _axis->is_reversed();
}

int MoveApiClient::absolute_external_move(double exttgt)
{
    if (unready_for_moving(true))
        return 0;

    bool reversed = is_reversed();
    double protected_exttgt = extclamp(exttgt, reversed);
    return unsafe_absolute_move_units(
        protected_exttgt, _current_speed, _current_accel);
}


void MoveApiClient::preset()
{
    _current_speed = _default_speed;
    _current_accel = _default_accel;
    assert(_current_speed.internalpulse_per_second() != 0);
    assert(_current_accel.internalpulse_per_second_sqr() != 0);
    limits_reevaluate();
}

void MoveApiClient::init(CommonAxis *parent) 
{
    auto nominal_speed = 
        PolymorphSpeed(PositionUnit::ROTATION, TimeUnit::MINUTES, 3000);
    _current_accel.set_nominal_speed(nominal_speed);
    _default_accel.set_nominal_speed(nominal_speed);
    _maximum_accel.set_nominal_speed(nominal_speed);
}


    void MoveApiClient::operation_finish_callback()
    {
        _axis->operation_finish_callback();
    }

    void MoveApiClient::operation_start_callback()
    {
        _axis->start_operation_notify();
    }


std::string MoveApiClient::info() {
    std::string ret = nos::format(
        "axis: {axname}\r\n"
        "current position: {pos}\r\n"
        "unitRatio: {unitRatio}\r\n"
        "gear: {gearstr}\r\n",
        "axname"_a = _axis->name(),
        "pos"_a = _axis->last_unit_position(),
        "unitRatio"_a = _axis->unitRatio(),
        "gearstr"_a = gear_settings()->info());       
    return ret;
}