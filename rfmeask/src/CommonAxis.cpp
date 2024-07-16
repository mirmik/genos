#include <CommonAxis.h>
#include <moveapi/MoveApiClient.h>
#include <SyncScanApiClient.h>
#include <comm/Notify.h>
#include <devices/DeviceChecker.h>
#include <igris/util/bug.h>
#include <logging.h>
#include <thread>

CommonAxis::CommonAxis(const std::string &name) : AbstractAxis(name) {}

void CommonAxis::init()
{
    std::set<Device *> devices;

    if (move_client)
    {
        move_client->init(this);
        auto move_client_devices = move_client->get_devices();
        devices.insert(move_client_devices.begin(), move_client_devices.end());
    }

    if (scan_client)
    {
        scan_client->init();
        auto scan_client_devices = move_client->get_devices();
        devices.insert(scan_client_devices.begin(), scan_client_devices.end());
    }

    setDevices(devices);
}

std::vector<double>
CommonAxis::scan_points_position(int offset, int size, bool from_end)
{
    if (scan_client)
        return scan_client->scan_points_position(offset, size, from_end);

    return {};
}

void CommonAxis::on_ready_status_changed(bool ready)
{
    if (ready)
        this->preset();
}

void CommonAxis::maxSpeed_ups(double arg)
{
    BUG();
}

void CommonAxis::maxAccel_ups2(double arg)
{
    BUG();
}

double CommonAxis::maxSpeed_ups() const
{
    BUG();
}

uint32_t CommonAxis::minAccel_ms() const
{
    return move_client->get_current_accel_ms();
}

int32_t CommonAxis::getSpeed_rpm() const
{
    return move_client->get_current_speed_rpm();
}

int32_t CommonAxis::getAccel_ms() const
{
    return move_client->get_current_accel_ms();
}

double CommonAxis::getSpeed_upm() const
{
    return move_client->get_current_speed_upm();
}

double CommonAxis::getAccel_ups2() const
{
    return move_client->get_current_accel_upss();
}

double CommonAxis::maxAccel_ups2() const
{
    return move_client->get_current_accel_upss();
}

void CommonAxis::setAccel_ups2(double upss)
{
    move_client->set_current_accel_upss(upss);
}

void CommonAxis::minAccel_ms(uint32_t arg)
{
    move_client->set_current_accel_ms(arg);
}

void CommonAxis::defaultSpeed_ups(double arg)
{
    return move_client->set_default_speed_ups(arg);
}

void CommonAxis::defaultAccel_ups2(double arg)
{
    return move_client->set_default_accel_upss(arg);
}

double CommonAxis::defaultSpeed_ups() const
{
    return move_client->get_default_speed_ups();
}

double CommonAxis::defaultAccel_ups2() const
{
    return move_client->get_default_accel_upss();
}

void CommonAxis::set_speed_procent(double proc)
{
    BUG();
}

void CommonAxis::absoluteUnitMove_impl(double dist, bool lock_mode)
{
    if (in_moving_state())
    {
        stop();
        return;
    }

    if (prepare_move_operation())
        return;

    move_client->absolute_external_move(dist);
}

int CommonAxis::prepare_move_operation()
{
    nos::println("PREPARE MOVE OPERATION");
    start_operation_notify();
    return 0;
}

void CommonAxis::setSpeed_ups(double arg)
{
    move_client->set_current_speed_ups(arg);
}

double CommonAxis::getSpeed_ups() const
{
    return move_client->get_current_speed_ups();
}

void CommonAxis::setSpeed_rpm(int32_t rpm)
{
    move_client->set_current_speed_rpm(rpm);
}

uint32_t CommonAxis::maxSpeed_rpm() const
{
    return move_client->get_maximum_speed_rpm();
}

uint32_t CommonAxis::defaultSpeed_rpm() const
{
    return move_client->get_default_speed_rpm();
}

uint32_t CommonAxis::defaultAccel_ms() const
{
    return move_client->get_default_accel_ms();
}

void CommonAxis::preset()
{
    nos::println("CommonAxis::preset");
    std::lock_guard<std::recursive_mutex> lock(preset_protector_mutex);

    nos::println("CommonAxis::preset preset client");
    move_client->preset();
    if (scan_client)
        scan_client->preset();
    limits_update();
    axis_inited = true;
}

void CommonAxis::limits_update()
{
    move_client->set_limits_external(unitBackwardLimit(), unitForwardLimit());
}

/*double CommonAxis::getUnitPosition()
{
    if (is_unready())
        return 0;

    return move_client->current_external_position();
}*/

void CommonAxis::stop_impl()
{
    if (scan_client)
        scan_client->stop();
    if (move_client)
        move_client->stop();
}

void CommonAxis::defaultSpeed_rpm(uint32_t arg)
{
    move_client->set_default_speed_rpm(arg);
}

void CommonAxis::maxSpeed_rpm(uint32_t arg)
{
    move_client->set_default_speed_rpm(arg);
}

void CommonAxis::command(const nos::argv &args, nos::ostream &out)
{
    if (args[0] == "clients")
    {
        nos::println_to(
            out, "move:", move_client ? move_client->typestr() : "no");
        nos::println_to(
            out, "scan:", scan_client ? scan_client->typestr() : "no");
        return;
    }

    if (args[0] == "info")
    {
        nos::println_to(out, info());
        return;
    }

    if (move_client)
    {
        bool solved = move_client->command(args, out);
        if (solved)
            return;
    }

    if (scan_client)
    {
        bool solved = scan_client->command(args, out);
        if (solved)
            return;
    }

    AbstractAxis::command(args, out);
}

std::string CommonAxis::info()
{
    auto moveapi_info = move_client ? move_client->info() : std::string("null");
    auto scanapi_info = scan_client ? scan_client->info() : std::string("null");
    return nos::format("moveapi info:\r\n{}\r\nscanapi info:\r\n{}\r\n",
                       moveapi_info,
                       scanapi_info);
}

std::string CommonAxis::command_help()
{
    std::string ret = AbstractAxis::command_help();
    if (move_client)
        ret += move_client->command_help();
    if (scan_client)
        ret += scan_client->command_help();
    return ret;
}
