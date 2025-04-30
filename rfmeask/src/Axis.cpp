/**
    @file
*/

#include <Axis.h>
#include <algorithm>
#include <comm/Notify.h>
#include <crow-support.h>
#include <devices/DeviceChecker.h>
#include <modes.h>
#include <servos/AbstractServo.h>
#include <tables.h>

using namespace nos::argument_literal;

#define EPSILON 0.00001
#define EPSILON_UNIT 0.1

#define AXIS_TRY_BEGIN                                                         \
    try                                                                        \
    {
#define AXIS_TRY_END                                                           \
    }                                                                          \
    catch (const DeviceException &expt)                                        \
    {                                                                          \
        throw AxisOperationTimeException();                                    \
    }

/*Axis::Axis(const std::string &name, AbstractServo *_mover, bool noinit) :
    AbstractAxis(name), mover(_mover), last_status(AbstractServo::STOPED)
{
}*/

Axis::Axis(const std::string &name,
           AbstractServo *_mover,
           std::vector<Device *> vect) :
    AbstractAxis(name), mover(_mover), last_status(AbstractServo::STOPED)
{
    setDevices(vect);

    if (mover)
    {
        mover->attach_axis(this);

        if (mover->use_pulse_position_notification())
            mover->positionUpdated +=
                igris::make_delegate(&Axis::notifyCurrentPosition, this);

        else
            mover->positionUpdatedUnit +=
                igris::make_delegate(&Axis::notifyCurrentPositionUnit, this);

        mover->operationStatusSignal +=
            igris::make_delegate(&Axis::operationStatusSlot, this);
        mover->limitSwitchStatusSignal += igris::make_delegate(
            &AbstractAxis::limitSwitchStatusSlot, (AbstractAxis *)this);

        mover_status_subscription =
            mover->get_device_status_observable().subscribe(
                [&](const auto &pair) {
                    auto [status, dev] = pair;
                    if (status == DeviceStatus::Refuse)
                        this->onCriticalErrorHandler(dev);
                    else if (status == DeviceStatus::Alarm)
                        this->onCriticalErrorHandler(dev);
                });
    }
}

Axis::Axis(const std::string &name, AbstractServo *_mover) :
    Axis(name, _mover, std::vector<Device *>({_mover}))
{
}

void Axis::on_ready_status_changed(bool ready)
{
    if (ready)
        this->onReadyHandler();
}

void Axis::onMoverReady(Device *) {}

void Axis::onReadyHandler()
{
    preset();
}

uint32_t Axis::protect_accel(uint32_t val)
{
    if (val < m_min_accel_ms)
    {
        nos::log::warn("protect accel. setted:{setted} min:{min}",
                       "setted"_a = val,
                       "min"_a = m_min_accel_ms);
        return m_min_accel_ms;
    }
    else
        return val;
}

void Axis::protect_accel_exception(uint32_t val)
{
    if (val < m_min_accel_ms)
    {
        nos::log::warn("protect accel exception. setted:{setted}",
                       "setted"_a = val);
        throw WrongValueException();
    }
}

void registry_axis(AbstractAxis *axis)
{
    size_t number = axes_total();
    add_axis_to_list(axis);
    axis->set_number(number);
    axis->notifiesRegistry();
}

//Обработчики сигналов отправки уведомлений от физ. уровня.

void Axis::stopLastOperation(StopType stoptype)
{
    try
    {
        mover->stop();
    }
    catch (const ProjectException &err)
    {
    }

    if (operation_status() == Operation::NONE)
        return;

    switch (stoptype)
    {
    case StopType::MANUAL:
        operationStopTypeNotify.notify(NOTIFY_STOP_MANUAL);
        break;
    case StopType::ABORT:
        operationStopTypeNotify.notify(NOTIFY_STOP_ABORT);
        break;
    case StopType::NORMAL:
        operationStopTypeNotify.notify(NOTIFY_STOP_NORMAL);
        break;
    }

    nos::println("AXIS STOP LAST OPERATION");
    operation_finish_callback();
}

bool Axis::inlimits(int32_t arg)
{
    return arg >= backwardLimit() && arg <= forwardLimit();
}

int32_t Axis::clamp_to_limits(int32_t tgt)
{
    int32_t f = forwardLimit();
    int32_t b = backwardLimit();
    return tgt > f ? f : (tgt < b ? b : tgt);
}

bool Axis::hasInterpolation()
{
    return mover->hasInterpolation();
}

double Axis::units_to_pulses(double units) const
{
    if (mover->is_fully_closed_mode())
    {
        return units * unitRatio_by_fully_closed();
    }

    double command_pulse_per_round = mover->command_unit_per_round();
    return gearSettings.units_to_rounds(units) * command_pulse_per_round;
}

double Axis::pulses_to_units(double pulses) const
{
    if (mover->is_fully_closed_mode())
    {
        return pulses / unitRatio_by_fully_closed();
    }

    double command_pulse_per_round = mover->command_unit_per_round();
    double rounds = pulses / command_pulse_per_round;
    return gearSettings.rounds_to_units(rounds);
}

void Axis::absoluteUnitMove_impl(double dist, bool ctrl)
{
    nos::println("Axis::absoluteUnitMove_impl");
    std::lock_guard<std::recursive_mutex> lock(updmtx);

    // Mitsubishi Servo работает с INT32
    double pulses = units_to_pulses(dist);
    if (pulses > INT32_MAX || pulses < INT32_MIN)
    {
        throw AxisOperationTimeException();
    }

    if (controller)
        controller->stop();

    checkAxisIsReady();

    // Костыль для сканера на газпроме
    // Второй COMPSTART ломает scan stop обработчик
    // Поэтому ручками взводим флаг при начале движения
    lastPoint = false;
    scanStop = false;

    if (servo_absolute_moving_compat())
        absoluteUnitMove_over_unit(dist, ctrl);
    else
        absoluteUnitMove_over_pulse(pulses, ctrl);
}

void Axis::absoluteUnitMove_over_pulse(double pulses, bool ctrl)
{
    nos::println("Axis::absoluteUnitMove_over_pulse");
    std::lock_guard<std::recursive_mutex> lock(updmtx);
    AXIS_TRY_BEGIN
    if (operation_status() != Operation::NONE)
        stopLastOperation(StopType::ABORT);
    mover->resetSpeed();
    mover->absolute_move(pulses);
    AXIS_TRY_END
    operationCorrectStart(ctrl);
}

void Axis::absoluteUnitMove_over_unit(double dist, bool ctrl)
{
    nos::println("Axis::absoluteUnitMove_over_unit");
    std::lock_guard<std::recursive_mutex> lock(updmtx);
    AXIS_TRY_BEGIN
    if (operation_status() != Operation::NONE)
        stopLastOperation(StopType::ABORT);
    mover->resetSpeed();

    nos::println("Axis::absoluteUnitMove_over_unit2", dist);
    mover->absolute_unit_move(dist);
    AXIS_TRY_END
    operationCorrectStart(ctrl);
}

bool Axis::servo_absolute_moving_compat()
{
    return mover->is_absolute_moving_recomended();
}

void Axis::setSpeed_rpm(int32_t arg)
{
    mover->setSpeed_rpm(arg);
}

int32_t Axis::getSpeed_rpm() const
{
    return mover->settedSpeed_rpm();
}

// TODO: Add protect speed
void Axis::setSpeed_unitPerSecond(double arg)
{
    mover->setSpeed_cups(arg * unitRatio_by_gear());
    mover->optional_setRawUnitSpeed(arg);
}

double Axis::getSpeed_unitPerSecond() const
{
    return mover->settedSpeed_cups() / unitRatio_by_gear();
}

// TODO: Add protect speed
void Axis::setSpeed_unitPerMinute(double arg)
{
    mover->setSpeed_cupm(arg * unitRatio_by_gear());
}

double Axis::getSpeed_unitPerMinute() const
{
    return mover->settedSpeed_cupm() / unitRatio_by_gear();
}

void Axis::setAccel_ms(int32_t arg)
{
    arg = protect_accel(arg);
    mover->setAccel_ms(arg);
}

int32_t Axis::getAccel_ms() const
{
    return mover->settedAccel_ms();
}

void Axis::generate_trigger()
{
    throw FunctionIsNotSupported();
}

void Axis::manual_trigger_mode(int32_t en)
{
    throw FunctionIsNotSupported();
}

int32_t Axis::synchroSpeedTest()
{
    throw FunctionIsNotSupported();
}

uint32_t Axis::maxSpeed_rpm() const
{
    return mover->maxspd;
}

void Axis::maxSpeed_rpm(uint32_t maxspd)
{
    mover->maxspd = maxspd;
}

void Axis::preset()
{
    checkAxisIsReady();
    mover->preset();
    setSpeed_rpm(m_defaultSpeed_rpm);
    setAccel_ms(protect_accel(m_defaultAccel_ms));
    mover->presetTandem();

    nos::println("AXIS PRESET");
    operation_finish_callback();

    axis_inited = true;
}

// Count of command??? impulses per unit.
double Axis::unitRatio() const
{
    if (mover->is_fully_closed_mode())
        return unitRatio_by_fully_closed();
    else
        return unitRatio_by_gear();
}

double Axis::unitRatio_by_fully_closed() const
{
    double command_pulse_per_userunit =
        (fullyClosedGear.load_pulse_per_userunit() /
         mover->native_impulse_per_command_impulse());
    return command_pulse_per_userunit;
}

double Axis::unitRatio_by_gear() const
{
    if (gearSettings.has_setted_value())
    {
        // Обратная совместимость со старым стилем
        // Ранее предполагалось, что unitRatio учитывает
        // полное отношение unit к управляющим пульсам.
        // Позже unitRatio был переопределён через gearSettings
        // и стал учитывать только механическую передачу.
        return gearSettings.get_forced_fullcycle_value();
    }

    double rounds_per_unit = gearSettings.rounds_per_unit();
    double command_pulse_per_round = mover->command_unit_per_round();
    double command_pulse_per_unit = command_pulse_per_round * rounds_per_unit;
    return command_pulse_per_unit;
}

int32_t Axis::getLimitSwitchStatus()
{
    int32_t ret;
    checkAxisIsReady();
    AXIS_TRY_BEGIN
    ret = mover->request_limit_switch_status();
    AXIS_TRY_END
    return ret;
}

void Axis::setNullPosition()
{
    nos::log::info("set null position");
    checkAxisIsReady();
    AXIS_TRY_BEGIN
    mover->setNullPosition();
    AXIS_TRY_END
    return;
}

void Axis::servo_on()
{
    nos::log::info("servo_on");
    checkAxisIsReady();
    AXIS_TRY_BEGIN
    mover->servo_on();
    AXIS_TRY_END
    return;
}

void Axis::servo_off()
{
    nos::log::info("servo_off");
    checkAxisIsReady();
    AXIS_TRY_BEGIN
    mover->servo_off();
    AXIS_TRY_END
    return;
}

void Axis::stop_impl()
{
    if (controller)
    {
        controller->stop();
    }
    stopLastOperation(StopType::MANUAL);
}

bool Axis::haveScan()
{
    return false;
}

bool Axis::haveAbsoluteMove()
{
    return mover->haveAbsoluteMove();
}

bool Axis::haveSetReference()
{
    return mover->haveSetReference();
}

long Axis::milliseconds_from_last_operation_start()
{
    auto diff = std::chrono::system_clock::now() - last_operation_start_time;
    auto diff_ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    return diff.count();
}

void Axis::operationStatusSlot(uint8_t status)
{
    std::lock_guard<std::recursive_mutex> lock(updmtx);
    if (milliseconds_from_last_operation_start() < 300)
        return;

    if (status != last_status)
    {
        opstatus_changer_count++;
        if (opstatus_changer_count > 5)
        {
            nos::log::warn(
                "Operation status signal trimmer detected. axname:{}", name());
        }
        last_status = status;
    }

    if (operation_status() == Operation::USERMOVE &&
        status == AbstractServo::STOPED)
    {
        opstatus_changer_count = 0;
        nos::log::info("operation finish correct axname:{}", name());
        operationCorrectFinish();
        return;
    }
}

void Axis::onCriticalErrorHandler(Device *dev)
{
    if (operation_status() == Operation::NONE)
    {
        return;
    }

    stopLastOperation(StopType::ABORT);
}

void Axis::operationCorrectStart(bool ctrl)
{
    nos::println("AXIS OPERATION CORRECT START");
    std::lock_guard<std::recursive_mutex> lock(updmtx);
    operationStopTypeNotify.notify(NOTIFY_START);

    start_operation_notify();
    /// Использавалось для работы со старыми пультами.
    // if (ctrl)
    //    lock_mode_notify();

    last_operation_start_time - std::chrono::system_clock::now();
    opstatus_changer_count = 0;
}

void Axis::operationCorrectFinish()
{
    nos::println("AXIS OPERATION CORRECT FINISH");
    std::lock_guard<std::recursive_mutex> lock(updmtx);
    operationStopTypeNotify.notify(NOTIFY_STOP_NORMAL);
    double curpos = mover->currentPosition();

    if (unitRatio() == 0)
    {
        nos::fprintln("UnitRatio is zero. axname{}", name());
    }
    assert(!isinf(unitRatio()));
    assert(unitRatio() != 0);
    update_current_position_handler_with_reverse_if_need(
        (double)curpos / unitRatio(), true);

    operation_finish_callback();
}

void Axis::scanMove(int32_t arg)
{
    throw FunctionIsNotSupported();
}

void Axis::scanUnitMove(double arg)
{
    throw FunctionIsNotSupported();
}

void Axis::scanForwardZone(int32_t arg)
{
    throw FunctionIsNotSupported();
}

void Axis::scanUnitForwardZone(double arg)
{
    throw FunctionIsNotSupported();
}

void Axis::scanBackwardZone(int32_t arg)
{
    throw FunctionIsNotSupported();
}

void Axis::scanUnitBackwardZone(double arg)
{
    throw FunctionIsNotSupported();
}

void Axis::scanStart()
{
    throw FunctionIsNotSupported();
}

void Axis::scanCompareStart()
{
    throw FunctionIsNotSupported();
}

void Axis::scanPoints(int32_t arg)
{
    throw FunctionIsNotSupported();
}

int32_t Axis::scanMove()
{
    throw FunctionIsNotSupported();
}

double Axis::scanUnitMove()
{
    throw FunctionIsNotSupported();
}

int32_t Axis::scanForwardZone()
{
    throw FunctionIsNotSupported();
}

double Axis::scanUnitForwardZone()
{
    throw FunctionIsNotSupported();
}

int32_t Axis::scanBackwardZone()
{
    throw FunctionIsNotSupported();
}

double Axis::scanUnitBackwardZone()
{
    throw FunctionIsNotSupported();
}

int32_t Axis::scanPoints()
{
    throw FunctionIsNotSupported();
}

void Axis::command(const nos::argv &args, nos::ostream &out)
{
    if (args[0] == "info")
    {
        nos::println_to(out, info());
        return;
    }

    AbstractAxis::command(args, out);
}

std::string Axis::info()
{
    return nos::format("fully_closed: {}\n"
                       "unitRatio: {}\n"
                       "unitRatio_by_gear: {}\n"
                       "unitRatio_by_fully_closed: {}\n",
                       mover->is_fully_closed_mode(),
                       unitRatio(),
                       unitRatio_by_gear(),
                       unitRatio_by_fully_closed());
}

std::string Axis::command_help()
{
    std::string ret = AbstractAxis::command_help();
    ret += "info - get axis information\n";
    return ret;
}

void Axis::notifyCurrentPosition(int32_t arg)
{
    // assert(!isinf(unitRatio()));
    update_current_position_handler_with_reverse_if_need(pulses_to_units(arg),
                                                         false);
}

void Axis::notifyCurrentPositionUnit(double arg)
{
    assert(!isinf(arg));
    update_current_position_handler_with_reverse_if_need(arg, false);
}
