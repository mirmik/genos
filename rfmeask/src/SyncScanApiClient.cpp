/**
    @file
*/

#include <AbstractAxis.h>
#include <AbstractSyncro.h>
#include <STUFF.h>
#include <SyncScanApiClient.h>
#include <comm/Notify.h>
#include <servos/AbstractServo.h>

#include <igris/dprint.h>
#include <stdint.h>
#include <utility>

#define AXIS_TRY_BEGIN                                                         \
    try                                                                        \
    {
#define AXIS_TRY_END                                                           \
    }                                                                          \
    catch (const DeviceException &expt)                                        \
    {                                                                          \
        parent->stopLastOperation(StopType::ABORT);                            \
        throw AxisOperationTimeException();                                    \
    }

std::set<Device *> SyncScanApiClient::get_devices()
{
    std::set<Device *> devices;
    devices.insert(synchro);
    devices.insert(syncsrv);
    return devices;
}

void SyncScanApiClient::init()
{
    // pass
}

void SyncScanApiClient::set_reverse(bool r)
{
    reverse = r;
}

void SyncScanApiClient::scanMove(int32_t arg)
{
    // Реверс сканирования есть произведение реверса оси и собственного реверса
    // сканирования.
    {
        if (parent->is_reversed())
            arg = -arg;

        if (reverse)
            arg = -arg;
    }

    parent->checkAxisIsReady();
    AXIS_TRY_BEGIN
    synchro->setScanZone(arg);
    AXIS_TRY_END
    settedScanMove = arg;
}

void SyncScanApiClient::scanUnitMove(double arg)
{
    nos::println("scanUnitMove");
    assert(gain() != 0);
    parent->scanMove(arg * gain());
}

void SyncScanApiClient::scanForwardZone(int32_t arg)
{
    parent->checkAxisIsReady();
    AXIS_TRY_BEGIN
    synchro->setScanStartZone(arg);
    AXIS_TRY_END
    settedScanFZone = arg;
}

int32_t SyncScanApiClient::get_encoder_position()
{
    int32_t res;
    parent->checkAxisIsReady();
    AXIS_TRY_BEGIN
    res = synchro->get_encoder_position();
    AXIS_TRY_END

    return res;
}

void SyncScanApiClient::scanUnitForwardZone(double arg)
{
    nos::println("scanUnitForwardZone");
    scanForwardZone(arg * gain());
}

void SyncScanApiClient::scanBackwardZone(int32_t arg)
{
    parent->checkAxisIsReady();
    AXIS_TRY_BEGIN
    synchro->setScanStopZone(arg);
    AXIS_TRY_END
    settedScanBZone = arg;
}

void SyncScanApiClient::scanUnitBackwardZone(double arg)
{
    nos::println("scanUnitBackwardZone");
    scanBackwardZone(arg * gain());
}

void SyncScanApiClient::stop()
{
    // synchro->stop();
}

void SyncScanApiClient::scanCompareStart()
{
    if (!parent)
        throw std::runtime_error("parent is null");
    if (!synchro)
        throw std::runtime_error("synchro is null");

    parent->checkAxisIsReady();
    synchro->binded_ax = parent;

    AXIS_TRY_BEGIN
    lastPoint = false;
    scanStop = false;

    g_scanPointSignal.clear();
    descanSignal.clear();
    wrongScanPointSignal.clear();

    descanSignal +=
        igris::make_delegate(&SyncScanApiClient::stopScanHandler, this);
    wrongScanPointSignal +=
        igris::make_delegate(&SyncScanApiClient::wrongScanPointHandler, this);
    scanPointNumber = 0;

    g_scanPointSignal +=
        igris::make_delegate(&SyncScanApiClient::scanPointHandler, this);

    synchro->cmpscanStart();
    AXIS_TRY_END
}

void SyncScanApiClient::scanPoints(int32_t arg)
{
    parent->checkAxisIsReady();
    AXIS_TRY_BEGIN
    synchro->setScanPoints(arg);
    AXIS_TRY_END
    settedScanPoints = arg;
}

int32_t SyncScanApiClient::scanMove()
{
    return parent->is_reversed() ? -settedScanMove : settedScanMove;
}

double SyncScanApiClient::scanUnitMove()
{
    nos::println("scanUnitMove");
    return (double)settedScanMove / gain();
}

int32_t SyncScanApiClient::scanForwardZone()
{
    return settedScanFZone;
}

double SyncScanApiClient::scanUnitForwardZone()
{
    nos::println("scanUnitForwardZone");
    return (double)settedScanFZone / gain();
}

int32_t SyncScanApiClient::scanBackwardZone()
{
    return settedScanBZone;
}

double SyncScanApiClient::scanUnitBackwardZone()
{
    nos::println("scanUnitBackwardZone");
    return (double)settedScanBZone / gain();
}

int32_t SyncScanApiClient::scanPoints()
{
    return settedScanPoints;
}

void SyncScanApiClient::stopScanHandler()
{
    // g_scanPointSignal.clear();
    descanSignal.clear();
    wrongScanPointSignal.clear();
}

void SyncScanApiClient::scanPointHandler(int num)
{
    parent->scan_point_handler(num);
}

int32_t SyncScanApiClient::trigreturn_time()
{
    return synchro->trigreturn_time();
}

void SyncScanApiClient::unsubscribe() {}

void SyncScanApiClient::wrongScanPointHandler()
{
    parent->wrong_scan_point_handler();
}

void SyncScanApiClient::evaluate_gain(const std::string &strategy)
{
    assert(strategy == "srv_gain_multi");
    gain_strategy = strategy;
}

void SyncScanApiClient::generate_trigger()
{
    synchro->generate_trigger();
}

void SyncScanApiClient::manual_trigger_mode(int32_t en)
{
    synchro->manual_trigger_mode(en);
}

void SyncScanApiClient::no_trigger_mode(int32_t en)
{
    if (en == 0)
        synchro->wait_trigger_mode(1);

    else if (en == 1)
        synchro->wait_trigger_mode(0);

    else
        synchro->wait_trigger_mode(en);
}

void SyncScanApiClient::bitrigmode(int32_t en)
{
    synchro->bitrigmode((bool)en);
}

int32_t SyncScanApiClient::discrete_output_set_state(int num, int state)
{
    synchro->discrete_output_set_state(num, state);
    return 0;
}

int32_t SyncScanApiClient::discrete_output_set_unical(int num)
{
    synchro->discrete_output_set_unical(num);
    return 0;
}

int32_t SyncScanApiClient::discrete_output_disable()
{
    synchro->discrete_output_disable();
    return 0;
}

bool SyncScanApiClient::command(const nos::argv &args, nos::ostream &os)
{
    return false;
}

std::string SyncScanApiClient::command_help()
{
    return "";
}

std::string SyncScanApiClient::info()
{
    return "";
}

double SyncScanApiClient::gain() const
{
    nos::println("SyncScanApiClient::gain");
    assert(gain_strategy == "srv_gain_multi");
    if (gain_strategy == "srv_gain_multi")
    {
        assert(parent->unitRatio() != 0);
        assert(syncsrv->get_input_gain() != 0);
        nos::println("syncsrv->get_input_gain()", syncsrv->get_input_gain());
        auto value = parent->unitRatio() * syncsrv->get_input_gain();
        return value;
    }
    return _gain;
}

int SyncScanApiClient::enable_external_trigger_notify()
{
    synchro->enable_external_trigger_mode();
    return 0;
}

void SyncScanApiClient::preset()
{
    // double loadGain = syncsrv->load_side_per_servo_side_multiplier();
    // synchro->setLoadGain(loadGain);
    // syncsrv->bindSyncro(synchro);
}

std::vector<double>
SyncScanApiClient::scan_points_position(int offset, int size, bool from_end)
{
    std::vector<double> result;
    auto intres = synchro->scan_points_position(offset, size, from_end);

    for (auto &i : intres)
    {
        int rev = parent->is_reversed() ? -1 : 1;
        double val = (double)i / parent->unitRatio() * rev;
        result.push_back(val);
    }

    return result;
}