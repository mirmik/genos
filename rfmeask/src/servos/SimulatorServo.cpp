/**
    @file
*/

#include <AbstractAxis.h>
#include <Axis.h>
#include <comm/Notify.h>
#include <igris/time/systime.h>
#include <ralgo/global_protection.h>
#include <servos/SimulatorServo.h>

/*int64_t SimulatorServo::request_position()
{
    transactionConnectionSuccess();
    transactionReady();
    return simulator_state.feedpos;
}*/

std::recursive_mutex SimulatorServo::_bus_mutex;

int64_t SimulatorServo::request_position()
{
    transactionConnectionSuccess();
    transactionReady();
    auto curtime = igris::millis();
    auto curpos = position_function(curtime);
    return curpos * _attached_axis->unitRatio(); // simulator_state.feedpos;
}

uint8_t SimulatorServo::request_operation_status()
{
    transactionConnectionSuccess();
    transactionReady();
    auto curtime = igris::millis();
    return position_function.in_interval(curtime);
}

void SimulatorServo::relative_move(double dist)
{
    // pass
    // simulator.incmove(ralgo::discrete_time(), dist / 4194304. * 360);
}

void SimulatorServo::absolute_move(double dist)
{
    // pass
    // simulator.absmove(ralgo::discrete_time(), dist / 4194304. * 360);
}

void SimulatorServo::absolute_unit_move(double dist)
{
    auto curtime = igris::millis();
    auto curpos = position_function(curtime);
    auto absdiff = std::abs(dist - curpos);
    auto ftime = absdiff / speed.unit_per_second();
    nos::println(absdiff, ftime);
    position_function = ralgo::LinePositionFunction<int64_t, double>(
        igris::millis(), igris::millis() + ftime * 1000, curpos, dist);
}

double SimulatorServo::current_torque()
{
    // random value between -5.0 and 5.0
    return (rand() % 1000 - 500) / 100.0;
}

void SimulatorServo::preset() {}

void SimulatorServo::setAccel_ms(uint32_t acc)
{
    m_accel_ms = acc;
}

double SimulatorServo::settedSpeed_native()
{
    return speed.unit_per_second() * 100;
}

void SimulatorServo::setSpeed_native_impl(double spd)
{
    spd = spd / 100;
    speed = PolymorphSpeed(PositionUnit::EXTERNAL, TimeUnit::SECONDS, spd);
}

void SimulatorServo::stop()
{
    auto curtime = igris::millis();
    auto curpos = position_function(curtime);
    position_function = ralgo::LinePositionFunction<int64_t, double>(
        curtime, curtime, curpos, curpos);
}

void SimulatorServo::updaterThreadFunc()
{
    while (1)
    {
        uint8_t l_operationStatus;

        updmtx.lock();
        l_operationStatus = m_operationStatus;

        m_currentPosition = request_position();
        m_operationStatus = request_operation_status();

        if (torqueNotify.has_subscribers())
        {
            auto torque = current_torque();
            torqueNotify.notify(torque);
        }

        if (use_pulse_position_notification())
            positionUpdated(m_currentPosition);
        else
            positionUpdatedUnit(last_unit_position_invert_corrected_if_need());

        if ((m_operationStatus != l_operationStatus) ||
            m_operationStatusForce ||
            (std::chrono::system_clock::now() - last_opstatus_timestamp >
             std::chrono::milliseconds(500)))
        {
            last_opstatus_timestamp = std::chrono::system_clock::now();
            operationStatusSignal(m_operationStatus);
            m_operationStatusForce = false;
        }

        updmtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        // std::this_thread::sleep_for(std::chrono::milliseconds(m_updaterTimeout));
    }
}
