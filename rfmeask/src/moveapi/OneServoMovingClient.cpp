#include <moveapi/OneServoMovingClient.h>
#include <CommonAxis.h>


int OneServoMovingClient::unsafe_absolute_move_units(double dist, PolymorphSpeed, PolymorphAccel)
{ 
    nos::println("unsafe_absolute_move_units");
    _servo->absolute_unit_move(dist);
    return 0;
}

int OneServoMovingClient::unsafe_absolute_move_iu(double, PolymorphSpeed, PolymorphAccel) 
{ 
    nos::println("unsafe_absolute_move_iu");
    return 0; 
}

int OneServoMovingClient::unsafe_relative_move_iu(double, PolymorphSpeed, PolymorphAccel) 
{ 
    nos::println("unsafe_relative_move_iu");
    return 0; 
}

void OneServoMovingClient::init(CommonAxis *parent)
{
    _servo->attach_axis(_axis);
    _servo->positionUpdated += igris::make_delegate(
        &OneServoMovingClient::notifyCurrentPosition, this);
    _servo->limitSwitchStatusSignal +=
        igris::make_delegate(&AbstractAxis::limitSwitchStatusSlot,
                             dynamic_cast<AbstractAxis *>(_axis));

    MoveApiClient::init(parent);
    _servo->operationStatusSignal += 
        igris::make_delegate(&OneServoMovingClient::operation_status_signal, this);
}

void OneServoMovingClient::notifyCurrentPosition(int32_t arg)
{
    if (!_axis->axis_inited)
        return;

    double curpos = (double)arg * _servo->electronic_gear();
    _axis->update_current_position_handler_with_reverse_if_need(
        curpos / _axis->unitRatio(), false);
}

void OneServoMovingClient::operation_status_signal(uint8_t code) 
{
    if (code == (uint8_t)_axis->operation_status())
        return; 
    
    nos::println(code);
    if (code)
        MoveApiClient::operation_start_callback();
    else
        MoveApiClient::operation_finish_callback();  
}

void OneServoMovingClient::preset()
{
    _servo->preset();
    init_scale_multipliers();
    MoveApiClient::preset();
}

double OneServoMovingClient::native_pulses_per_unit() const
{
    if (_servo->is_fully_closed_mode())
    {
        return native_pulses_per_unit_by_fully_closed();
    }
    return native_pulses_per_unit_by_gear();
}

void OneServoMovingClient::init_scale_multipliers()
{
    auto unit2native = native_pulses_per_unit();
    double rot2native = _servo->native_impulse_per_round();
    scaller.set_gain(unit2native);
    for (auto *a : all_speeds)
    {
        a->set_rot2internal_multiplier(rot2native);
        a->set_unit2internal_multiplier(unit2native);
    }

    for (auto *a : all_accels)
    {
        a->set_rot2internal_multiplier(rot2native);
        a->set_unit2internal_multiplier(unit2native);
    }
}

double OneServoMovingClient::native_pulses_per_unit_by_fully_closed() const
{
    double load_pulse_per_unit = _fully_closed_gear.load_pulse_per_userunit();
    return load_pulse_per_unit;
}

double OneServoMovingClient::native_pulses_per_unit_by_gear() const
{
    if (_gear_settings.has_setted_value())
    {
        // Обратная совместимость со старым стилем
        // Ранее предполагалось, что unitRatio учитывает
        // полное отношение unit к управляющим пульсам.
        // Позже unitRatio был переопределён через gearSettings
        // и стал учитывать только механическую передачу.
        return _gear_settings.get_forced_fullcycle_value();
    }

    double rounds_per_unit = _gear_settings.rounds_per_unit();
    double native_pulse_per_round = _servo->native_impulse_per_round();
    double native_pulse_per_unit = native_pulse_per_round * rounds_per_unit;
    return native_pulse_per_unit;
}

void OneServoMovingClient::stop()
{
    _servo->stop();
    //operation_finish_callback();
}