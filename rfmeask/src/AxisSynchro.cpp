/**
    @file
*/

#include <AxisSynchro.h>
#include <STUFF.h>
#include <comm/Notify.h>
#include <iostream>
#include <nos/print.h>

#include <comm/notifymap.h>

#include <devices/DeviceChecker.h>

using namespace nos::argument_literal;

#define AXIS_TRY_BEGIN                                                         \
    try                                                                        \
    {
#define AXIS_TRY_END                                                           \
    }                                                                          \
    catch (const DeviceException &expt)                                        \
    {                                                                          \
        stopLastOperation(StopType::ABORT);                                    \
        throw AxisOperationTimeException();                                    \
    }

AxisSynchro::AxisSynchro(const std::string &name,
                         AbstractServo *_mover,
                         AbstractSyncro *_synchro,
                         bool spSignalEnable) :
    Axis(name, _mover, {_mover, _synchro}), synchro(_synchro)
{
    if (spSignalEnable)
    {
        g_scanPointSignal +=
            igris::make_delegate(&AxisSynchro::scanPointHandler, this);
    }
}

void AxisSynchro::on_ready_status_changed(bool ready)
{
    if (ready)
        this->onReadyHandler();
}

void AxisSynchro::operationStatusSlot(uint8_t status)
{
    // Обработка статуса движения сервоусилителя.

    std::lock_guard<std::recursive_mutex> lock(updmtx);
    if (milliseconds_from_last_operation_start() < 300)
        return;

    if (operation_status() == Operation::USERMOVE &&
        status == AbstractServo::STOPED)
    {
        // if (scan_mode && (!scanStop || !lastPoint))
        // {
        //     nos::println("Moving was finished. Wait for last scan point");
        //     return;
        // }

        // nos::log::info("operation finish correct");
        // operationCorrectFinish_SyncAxis();
        moving_finished_handler();
    }
}

void AxisSynchro::stopLastOperation(StopType stoptype)
{
    nos::println("AXIS SYNCRO STOP LAST OPERATION");
    std::lock_guard<std::recursive_mutex> lock(updmtx);
    // nos::log::debug("stopLastOperation");

    try
    {
        mover->stop();
        synchro->stop();
    }
    catch (const ProjectException &err)
    {
    }

    lastPoint = true;
    scanStop = true;
    descanSignal -= igris::make_delegate(&AxisSynchro::stopScanHandler, this);
    wrongScanPointSignal -=
        igris::make_delegate(&AxisSynchro::wrongScanPointHandler, this);

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
        printf("AxisSynchro::stopLastOperation::NOT_CORRECT_CODE\r\n");
        break;
    }

    operation_finish_callback();
}

void AxisSynchro::operationCorrectStart()
{
    nos::println("AXIS SYNCRO OPERATION CORRECT START");
    std::lock_guard<std::recursive_mutex> lock(updmtx);
    // nos::log::debug("operation correct start");
    //Уведомление о начале операции.
    operationStopTypeNotify.notify(NOTIFY_START);
    start_operation_notify();
}

void AxisSynchro::moving_finished_handler()
{
    std::lock_guard<std::recursive_mutex> lock(updmtx);
    Axis::operationCorrectFinish();
}

void AxisSynchro::scanning_finished_handler()
{
    std::lock_guard<std::recursive_mutex> lock(updmtx);
    descanSignal -= igris::make_delegate(&AxisSynchro::stopScanHandler, this);
    wrongScanPointSignal -=
        igris::make_delegate(&AxisSynchro::wrongScanPointHandler, this);

    scanFinishNotify.notify();
}

void AxisSynchro::generate_trigger()
{
    nos::log::debug("generate trigger");
    synchro->generate_trigger();
}

void AxisSynchro::manual_trigger_mode(int32_t en)
{
    nos::log::debug("manual trigger mode");
    synchro->manual_trigger_mode(en);
}

void AxisSynchro::no_trigger_mode(int32_t en)
{
    nos::log::debug("no trigger mode");
    if (en == 0)
        synchro->wait_trigger_mode(1);

    else if (en == 1)
        synchro->wait_trigger_mode(0);

    else
        synchro->wait_trigger_mode(en);
}

void AxisSynchro::bitrigmode(int32_t en)
{
    synchro->bitrigmode((bool)en);
}

int32_t AxisSynchro::synchroSpeedTest()
{
    return synchro->synchro_speed_test();
}

bool AxisSynchro::haveScan()
{
    return true;
}

// Merge AxisSynchro::preset with Axis::preset
void AxisSynchro::preset()
{
    try
    {
        nos::println("AXIS SYNCRO PRESET");
        checkAxisIsReady();
        mover->preset();

        double loadGain = mover->load_side_per_servo_side_multiplier();

        setSpeed_rpm(m_defaultSpeed_rpm);
        setAccel_ms(protect_accel(m_defaultAccel_ms));

        synchro->preset();
        synchro->setLoadGain(loadGain);

        // synchro->set_enc_numer(mover->enc_numer());
        // synchro->set_enc_denum(mover->enc_denum());

        mover->bindSyncro(synchro);
        synchro->binded_ax = this;

        scanMove(40000000);
        scanForwardZone(0);
        scanBackwardZone(0);
        scanPoints(20);

        lastPoint = false;
        scanStop = false;
        scan_mode = false;

        mover->presetTandem();
        axis_inited = true;
        _last_target_inited = false;

        initial_encoder_position = synchro->initial_encoder_position;
        operation_finish_callback();
    }
    catch (const ProjectException &err)
    {
        nos::log::error("AxisSynchro::preset: {}", err.what());
        //    throw;
    }
}

void AxisSynchro::scanMove(int32_t arg)
{
    if (is_reversed())
        arg = -arg;

    checkAxisIsReady();
    AXIS_TRY_BEGIN
    synchro->setScanZone(arg);
    AXIS_TRY_END
    settedScanMove = arg;
}

void AxisSynchro::scanUnitMove(double arg)
{
    nos::println("scanUnitMove unitRatio():", unitRatio(), "arg:", arg);
    scanMove(arg * unitRatio());
}

void AxisSynchro::scanForwardZone(int32_t arg)
{
    checkAxisIsReady();
    AXIS_TRY_BEGIN
    synchro->setScanStartZone(arg);
    AXIS_TRY_END
    settedScanFZone = arg;
}

void AxisSynchro::scanUnitForwardZone(double arg)
{
    scanForwardZone(arg * unitRatio());
}

void AxisSynchro::scanBackwardZone(int32_t arg)
{
    checkAxisIsReady();
    AXIS_TRY_BEGIN
    synchro->setScanStopZone(arg);
    AXIS_TRY_END
    settedScanBZone = arg;
}

void AxisSynchro::scanUnitBackwardZone(double arg)
{
    scanBackwardZone(arg * unitRatio());
}

std::vector<double> AxisSynchro::prepare_correction_table_for_scan_device()
{
    // Переводим информацию в юниты, чтобы расчитать смещения.
    // Затем переводим скорректированные точки назад
    // И вычитаем начальное смещение, чтобы получить относительное положение.
    auto current_position = mover->currentPosition();

    // mechanical system
    double raw_current_position_unit =
        static_cast<double>(current_position) / unitRatio();

    // ideal system.
    auto current_position_unit =
        one_axis_correction_table.inverted_corrected_point(
            ndmath::vector({raw_current_position_unit}))[0];

    double distance_unit = static_cast<double>(scanMove()) / unitRatio();
    double forward_zone_unit = static_cast<double>(scanUnitForwardZone());

    double finish_position = current_position_unit + distance_unit;

    auto uniform_points =
        ralgo::linspace(current_position_unit + forward_zone_unit,
                        finish_position,
                        scanPoints())
            .to_vector();

    for (auto p : uniform_points)
    {
        nos::println("ppppp:", p);
    }

    // evaluate scan points in mechanical system.
    int start_index = scanForwardZone() == 0 ? 1 : 0;
    std::vector<double> corrected_points(uniform_points.size() - start_index);
    for (size_t i = 0; i < corrected_points.size(); i++)
    {
        auto vec = ndmath::vector({uniform_points[i + start_index]});
        auto pnt = one_axis_correction_table.corrected_point(vec)[0];
        corrected_points[i] =
            (pnt - raw_current_position_unit - forward_zone_unit) * unitRatio();
        // *
        //(is_reversed() ? (-1) : (1));
    }

    return corrected_points;
}

void AxisSynchro::scanCompareStart()
{
    std::lock_guard<std::recursive_mutex> lock(updmtx);
    checkAxisIsReady();

    AXIS_TRY_BEGIN
    if (operation_status() != Operation::NONE)
    {
        stopLastOperation(StopType::ABORT);
    }
    lastPoint = false;
    scanStop = false;

    descanSignal.clear();
    wrongScanPointSignal.clear();
    g_scanPointSignal.clear();
    descanSignal += igris::make_delegate(&AxisSynchro::stopScanHandler, this);
    wrongScanPointSignal +=
        igris::make_delegate(&AxisSynchro::wrongScanPointHandler, this);
    scanPointNumber = 0;
    scan_mode = true;

    g_scanPointSignal +=
        igris::make_delegate(&AxisSynchro::scanPointHandler, this);

    if (correction_table_used())
    {
        auto table = prepare_correction_table_for_scan_device();
        for (auto d : table)
        {
            nos::fprintln("table point: {}", d);
        }
        synchro->start_comparing_scan_with_distance_table(table);
    }
    else
    {
        synchro->start_comparing_scan_classic_mode();
    }
    AXIS_TRY_END
}

void AxisSynchro::scanPoints(int32_t arg)
{
    checkAxisIsReady();
    AXIS_TRY_BEGIN
    synchro->setScanPoints(arg);
    AXIS_TRY_END
    settedScanPoints = arg;
}

int32_t AxisSynchro::scanMove()
{
    return is_reversed() ? -settedScanMove : settedScanMove;
}

double AxisSynchro::scanUnitMove()
{
    return (double)settedScanMove / unitRatio();
}

int32_t AxisSynchro::scanForwardZone()
{
    return settedScanFZone;
}

double AxisSynchro::scanUnitForwardZone()
{
    return (double)settedScanFZone / unitRatio();
}

int32_t AxisSynchro::scanBackwardZone()
{
    return settedScanBZone;
}

double AxisSynchro::scanUnitBackwardZone()
{
    return (double)settedScanBZone / unitRatio();
}

int32_t AxisSynchro::scanPoints()
{
    return settedScanPoints;
}

void AxisSynchro::stopScanHandler()
{
    nos::println("stopScanHandler");
    std::lock_guard<std::recursive_mutex> lock(updmtx);
    // nos::log::debug("stop scan handler");
    scanStop = true;
    if (scanStop && lastPoint)
    {
        lastPoint = false;
        scanStop = false;
        // operationCorrectFinish_SyncAxis();
        scanning_finished_handler();
    }
}

void AxisSynchro::scanPointHandler(int num)
{
    nos::println("scanPointHandler", num, settedScanPoints);
    // return;
    // nos::log::debug("scan point handler :{} wait_for", num,
    // settedScanPoints);

    scanPointNotify.notify(num);

    scanPointSignal(num);
    if ((uint32_t)num == settedScanPoints)
        lastPoint = true;

    // if (scanStop && (std::abs((int32_t)num - (int32_t)settedScanPoints) > 3))
    // {
    //     operationCorrectFinish_SyncAxis_from_scanPointHandler();
    //     return;
    // }

    if (lastPoint)
    {
        lastPoint = false;
        scanStop = false;
        // operationCorrectFinish_SyncAxis_from_scanPointHandler();
        scanning_finished_handler();
        return;
    }
}

// void AxisSynchro::operationCorrectFinish_SyncAxis_from_scanPointHandler()
// {
//     operationCorrectFinish_SyncAxis();
// }

int32_t AxisSynchro::trigreturn_time()
{
    return synchro->trigreturn_time();
}

void AxisSynchro::wrongScanPointHandler()
{
    nos::log::debug("wrong scan point handler");
    scanTrigErrorNotify.notify();
    // stopLastOperation(StopType::ABORT);
}

int32_t AxisSynchro::scan_discrete_output_set_state(int num, int state)
{
    synchro->discrete_output_set_state(num, state);
    return 0;
}

int32_t AxisSynchro::scan_discrete_output_set_unical(int num)
{
    synchro->discrete_output_set_unical(num);
    return 0;
}

int32_t AxisSynchro::scan_discrete_output_disable()
{
    synchro->discrete_output_disable();
    return 0;
}
