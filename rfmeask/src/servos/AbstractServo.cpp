/**
    @file
*/

#include <AbstractAxis.h>
#include <Axis.h>
#include <SyncScanApiClient.h>
#include <comm/Notify.h>
#include <comm/notifymap.h>
#include <config.h>
#include <devices/Device.h>
#include <mutex>
#include <ndmath/cartesian_correction.h>
#include <ndmath/util.h>
#include <servos/AbstractServo.h>
#include <thread>

using namespace nos::argument_literal;

bool prevent_updates = false;
std::recursive_mutex updmtx;

AbstractServo::AbstractServo(const std::string &name) :
    Device(name),
    m_faultStatus(Normal),
    m_currentPosition(0),
    m_limitSwitchStatus(10)
{
    _velocity_multiplier = new nos::trent_binder<double>();
    _velocity_multiplier->init(*runtime_syncer, "velocity_multiplier");
    _velocity_multiplier->sync_default(1);

    load_one_axis_correction_from_runtime();
}

double AbstractServo::last_position()
{
    return m_currentPosition;
}

double AbstractServo::command_pulse_per_userunit()
{
    if (_attached_axis == nullptr)
    {
        return 1;
    }

    auto &gear = gear_settings();
    if (is_fully_closed_mode())
    {
        double command_pulse_per_userunit =
            (fully_closed_gear().load_pulse_per_userunit() /
             native_impulse_per_command_impulse());
        return command_pulse_per_userunit;
    }
    else
    {
        if (gear.has_setted_value())
        {
            return m_currentPosition / gear.get_forced_fullcycle_value();
        }

        double rounds_per_unit = gear.rounds_per_unit();
        double command_pulse_per_round = command_unit_per_round();
        double command_pulse_per_unit =
            command_pulse_per_round * rounds_per_unit;
        return command_pulse_per_unit;
    }
}

double AbstractServo::last_unit_position()
{
    if (_attached_axis == nullptr)
    {
        return m_currentPosition;
    }

    auto &gear = gear_settings();
    if (is_fully_closed_mode())
    {
        double command_pulse_per_userunit =
            (fully_closed_gear().load_pulse_per_userunit() /
             native_impulse_per_command_impulse());
        return m_currentPosition / command_pulse_per_userunit;
    }
    else
    {
        if (gear.has_setted_value())
        {
            return m_currentPosition / gear.get_forced_fullcycle_value();
        }

        double command_pulse_per_round = command_unit_per_round();
        double rounds = m_currentPosition / command_pulse_per_round;
        return gear.rounds_to_units(rounds);
    }
}

double AbstractServo::velocity_multiplier()
{
    return _velocity_multiplier->get();
}

void AbstractServo::attach_axis(AbstractAxis *axis)
{
    _attached_axis = axis;
}

const GearSettings &AbstractServo::gear_settings() const
{
    if (!_gear_settings.has_value())
    {
        if (_attached_axis == nullptr)
            throw std::runtime_error("gear settings not setted");
        return *_attached_axis->gear_settings();
    }

    return _gear_settings.value();
}

const FullyClosedGear &AbstractServo::fully_closed_gear() const
{
    if (!_fully_closed_gear.has_value())
    {
        if (_attached_axis == nullptr)
            throw std::runtime_error("gear settings not setted");
        return *_attached_axis->fully_closed_gear();
    }

    return _fully_closed_gear.value();
}

void AbstractServo::notifiesRegistry(NotifyBrocker &brocker)
{
    notifyreg(torqueNotify,
              "ServoTorqueNotify(%d)",
              number,
              NPattern_ServoTorque,
              Tag_ServoTorque);
    notifyreg(rawPositionNotify,
              "ServoRawPositionNotify(%d)",
              number,
              NPattern_ServoRawPosition,
              Tag_ServoRawPosition);
    notifyreg(unitPositionNotify,
              "ServoUnitPositionNotify(%d)",
              number,
              NPattern_ServoUnitPosition,
              Tag_ServoUnitPosition);

    notifyreg(nonCorrectedUnitPositionNotify,
              "ServoNonCorrectedUnitPositionNotify(%d)",
              number,
              NPattern_ServoNonCorrectedUnitPosition,
              Tag_ServoNonCorrectedUnitPosition);

    notifyreg(limitSwitchNotify,
              "ServoLimitSwitchNotify(%d)",
              number,
              NPattern_ServoLimitSwitch,
              Tag_ServoLimitSwitch);
    parent::notifiesRegistry(brocker);
}

/** Функция потока опроса сервоусилителя.
    Опрашивается информация о положении. Статусе движения. Статусе концевых
   выключателей.
*/
void AbstractServo::updaterThreadFunc()
{
    while (1)
    {
        update_iteration++;
        uint8_t last_operationStatus = m_operationStatus;
        uint8_t last_limitSwitchStatus = m_limitSwitchStatus;
        int64_t last_currentPosition = m_currentPosition;
        if (prevent_updates)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        updmtx.lock();
        if (updaterThreadFunc_local())
            goto end;

        try
        {
            if (alarm_simulation)
            {
                transactionAlarmError("simulation");
                goto end;
            }
            m_currentPosition = request_position();
            updmtx.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            updmtx.lock();

            if (limit_switch_update_needed)
            {
                m_limitSwitchStatus = request_limit_switch_status();
                updmtx.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                updmtx.lock();
            }

            // int32_t position_diff = last_currentPosition - m_currentPosition;
            /*if (std::abs(position_diff) > 10)
            {
                m_operationStatus = OperationStatus::MOVED;
            }
            else
            {
                m_operationStatus = OperationStatus::STOPED;
            }*/

            // if (update_iteration % 3 == 0)
            //{
            m_operationStatus = request_operation_status();
            updmtx.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            updmtx.lock();
            //}
        }
        catch (const DeviceRefuseException &)
        {
            goto end;
        };

        if (use_pulse_position_notification())
            positionUpdated(m_currentPosition);
        else
            positionUpdatedUnit(last_unit_position_invert_corrected_if_need());

        if (last_currentPosition != m_currentPosition)
        {
            _position_subject.get_subscriber().on_next(m_currentPosition);
            _unit_position_subject.get_subscriber().on_next(
                last_unit_position_invert_corrected_if_need());
            rawPositionNotify.notify((float)m_currentPosition);
            unitPositionNotify.notify(
                (float)last_unit_position_invert_corrected_if_need());
            nonCorrectedUnitPositionNotify.notify((float)last_unit_position());
        }

        if (torqueNotify.has_subscribers())
        {
            auto torque = current_torque();
            updmtx.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            updmtx.lock();
            torqueNotify.notify(torque);
        }

        if ((m_operationStatus != last_operationStatus) ||
            m_operationStatusForce ||
            (std::chrono::system_clock::now() - last_opstatus_timestamp >
             std::chrono::milliseconds(500)))
        {
            // if (m_operationStatus != last_operationStatus)
            // {
            //     nos::fprintln("Operation Status change: {}: {}",
            //                   name(),
            //                   m_operationStatus);
            // }

            last_opstatus_timestamp = std::chrono::system_clock::now();
            operationStatusSignal(m_operationStatus);
            m_operationStatusForce = false;
        }

        if (m_limitSwitchStatus != last_limitSwitchStatus)
        {
            limitSwitchStatusSignal(m_limitSwitchStatus);
            limitSwitchNotify.notify(m_limitSwitchStatus);
        }

    end:
        updmtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

double AbstractServo::protect_speed(double spd)
{
    if (maxspd_native == 0)
    {
        maxspd_native = maxspd * rpmToSpdKoeff();
    }

    if (spd > maxspd_native)
    {
        nos::log::warn("protect speed. setted:{setted} max:{max} "
                       "maxnative:{maxnative}",
                       "setted"_a = spd,
                       "max"_a = maxspd,
                       "maxnative"_a = maxspd_native);
        return maxspd_native;
    }

    return spd;
}

void AbstractServo::load_one_axis_correction_from_runtime()
{
    // Запись содержит два обязательных поля - координаты точек points и
    // смещения deltas. И ключ enabled, указывающий, активировано ли применение
    // таблицы.
    if (!runtime_syncer || !runtime_syncer->good())
        return;

    auto &tr = runtime_syncer->node();
    if (tr.contains("one_axis_correction"))
    {
        auto cc = one_axis_cartesian_correction::trent_to_correction(
            tr["one_axis_correction"]);
        auto enabled = tr["one_axis_correction"]["enabled"].as_bool();
        set_one_axis_correction(cc, enabled);
    }
}

void AbstractServo::save_one_axis_correction_to_runtime()
{
    auto tr = one_axis_correction_as_trent();
    runtime_syncer->node()["one_axis_correction"] = tr;
    runtime_syncer->save();
}

nos::trent AbstractServo::one_axis_correction_as_trent() const
{
    auto ret = one_axis_cartesian_correction::correction_to_trent(
        one_axis_correction_table);
    ret["enabled"] = _correction_enabled;
    return ret;
}

double AbstractServo::unit_position_correct(double pos)
{
    if (_correction_enabled)
        return one_axis_correction_table.corrected_point({pos})[0];
    else
        return pos;
}

double AbstractServo::inverse_unit_position_correct(double pos)
{
    if (_correction_enabled)
        return one_axis_correction_table.inverted_corrected_point({pos})[0];
    else
        return pos;
}

/*double AbstractServo::impulse_position_correct(double pos)
{
    return pos;
}

double AbstractServo::inverse_impulse_position_correct(double pos)
{
    return pos;
}*/