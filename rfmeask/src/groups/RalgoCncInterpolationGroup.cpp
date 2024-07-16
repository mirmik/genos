#include <moveapi/CNCMovingClient.h>
#include <CommonAxis.h>
#include <groups/RalgoCncInterpolationGroup.h>
#include <ranges>

void RalgoCncInterpolationGroup::on_ready_status_changed(bool ready) {}

void RalgoCncInterpolationGroup::init()
{
    init_correction_table();

    if (check_axes_type(axes) == false)
        throw std::runtime_error("RalgoCncInterpolationGroup: wrong axes type");

    std::vector<CNCTandemMovingClient *> clients;
    std::transform(axes.begin(),
                   axes.end(),
                   std::back_inserter(clients),
                   [](AbstractAxis *axis) -> CNCTandemMovingClient * {
                       auto movclient =
                           dynamic_cast<CommonAxis *>(axis)->move_client.get();
                       return dynamic_cast<CNCTandemMovingClient *>(movclient);
                   });
    servo_symbols = collect_symbols(clients);
    std::transform(servo_symbols.begin(),
                   servo_symbols.end(),
                   std::back_inserter(servos),
                   [](auto &pair) { return pair.first; });
}

std::unordered_map<AbstractServo *, std::string>
RalgoCncInterpolationGroup::collect_symbols(
    std::vector<CNCTandemMovingClient *> clients)
{
    std::unordered_map<AbstractServo *, std::string> ret;
    for (auto *client : clients)
    {
        auto part = client->servos_and_symbols();
        for (auto &[servo, symbol] : part)
            ret[servo] = symbol;
    }
    return ret;
}

void RalgoCncInterpolationGroup::stop()
{
    prevent_multiblocks_sending();
    cnc->send_stop_command();
}

bool RalgoCncInterpolationGroup::check_axes_type(
    std::vector<AbstractAxis *> axes)
{
    try
    {
        for (auto *ax : axes)
        {
            auto &casted = dynamic_cast<CommonAxis &>(*ax);
            auto &api =
                dynamic_cast<CNCTandemMovingClient &>(*casted.move_client);
            (void)api;
        }
    }
    catch (const std::bad_cast &)
    {
        return false;
    }
    return true;
}

void RalgoCncInterpolationGroup::preset()
{
    nos::println("RalgoCncInterpolationGroup::preset");
}

double RalgoCncInterpolationGroup::speed()
{
    return _speed_ups;
}

double RalgoCncInterpolationGroup::acceleration()
{
    return _accel_ups2;
}

int RalgoCncInterpolationGroup::dim()
{
    return servo_symbols.size();
}

int RalgoCncInterpolationGroup::incmove(std::vector<double> vec)
{
    BUG();
    return 0;
}

void RalgoCncInterpolationGroup::absmove_with_correction(
    std::vector<double> target_)
{
    BUG();
}

double RalgoCncInterpolationGroup::setted_speed_ups()
{
    return _speed_ups;
}

double RalgoCncInterpolationGroup::setted_accel_ups2()
{
    return _accel_ups2;
}

void RalgoCncInterpolationGroup::set_common_speed_ups(double spd)
{
    _speed_ups = spd;
}

void RalgoCncInterpolationGroup::set_common_accel_ups2(double acc)
{
    _accel_ups2 = acc;
}

void RalgoCncInterpolationGroup::set_common_decel_ups2(double dcc)
{
    _decel_ups2 = dcc;
}

std::vector<double> RalgoCncInterpolationGroup::axis_pair_task_to_uspd_list(
    const std::vector<GrpAxisMovePair> &axis_pair_task)
{
    auto uspd_list = std::views::transform(
        axis_pair_task, [](auto &pair) { return pair.axis->getSpeed_ups(); });
    return std::vector<double>(uspd_list.begin(), uspd_list.end());
}

void RalgoCncInterpolationGroup::absmove_without_correction(
    const std::vector<GrpAxisMovePair> &axis_pair_task)
{
    BUG();
}

void RalgoCncInterpolationGroup::evaluate_servo_task(
    const std::vector<GrpServoMovePair> &servo_pair_task)
{
    std::vector<std::string> symbols;
    std::vector<double> point;
    std::vector<double> gains;
    // std::transform(servo_pair_task.begin(),
    //                servo_pair_task.end(),
    //                std::back_inserter(symbols),
    //                [&](auto task) { return symbol_for_servo(task.servo); });
    // std::transform(servo_pair_task.begin(),
    //                servo_pair_task.end(),
    //                std::back_inserter(point),
    //                [&](auto pair) { return pair.value; });
    // std::transform(
    //     servo_pair_task.begin(),
    //     servo_pair_task.end(),
    //     std::back_inserter(gains),
    //     [&](auto &task) { return task.servo->native_pulses_per_unit(); });
    for (size_t i = 0; i < servo_pair_task.size(); ++i)
    {
        symbols.push_back(symbol_for_servo(servo_pair_task[i].servo));
        point.push_back(servo_pair_task[i].value);
        gains.push_back(servo_pair_task[i].servo->native_pulses_per_unit());
        point[i] = servo_pair_task[i].servo->unit_position_correct(point[i]);
    }

    auto native_target_position =
        ralgo::vecops::mul_vv<std::vector<double>>(point, gains);

    for (auto &servo : servos)
        servo->enable_impulse_control_mode();

    cnc->absolute_move(symbols,
                       native_target_position,
                       0,  // zero speed and acceleration for
                       0); // disable common speed and accel limits.
}

void RalgoCncInterpolationGroup::set_pause_planning(bool en)
{
    cnc->set_pause_mode(en);
}

std::vector<std::pair<std::string, double>>
RalgoCncInterpolationGroup::axis_speed_list_to_symbol_speed_list(
    std::vector<std::pair<AbstractAxis *, double>> pairs)
{
    std::vector<std::pair<std::string, double>> ret;
    for (auto &pair : pairs)
    {
        CommonAxis *axis = dynamic_cast<CommonAxis *>(pair.first);
        auto speed = pair.second;
        auto *moveapi = cnc_moveapi_from_common_axis(axis);
        for (auto &servo : moveapi->servos())
        {
            auto symbol = symbol_for_servo(servo);
            ret.push_back({symbol, speed});
        }
    }
    return ret;
}

std::string RalgoCncInterpolationGroup::symbol_for_servo(AbstractServo *servo)
{
    return servo_symbols[servo];
}
CommonAxis *RalgoCncInterpolationGroup::axis_by_index(size_t index)
{
    auto *ax = axes[index];
    return dynamic_cast<CommonAxis *>(ax);
}

AbstractServo *RalgoCncInterpolationGroup::servo_by_servo_index(size_t index)
{
    AbstractServo *servo = nullptr;
    for (auto *ax : axes)
    {
        auto *common_axis = dynamic_cast<CommonAxis *>(ax);
        auto *move_api =
            dynamic_cast<CNCTandemMovingClient *>(common_axis->move_api());
        servo = move_api->servo_with_index(index);
        if (servo)
            return servo;
    }
    return nullptr;
}

std::vector<AbstractServo *>
RalgoCncInterpolationGroup::servos_for_servo_pair_task(
    const std::vector<GrpServoMovePair> &task_array)
{
    std::vector<AbstractServo *> servos;
    for (auto &t : task_array)
    {
        servos.push_back(t.servo);
    }
    return servos;
}

void RalgoCncInterpolationGroup::set_speeds_ups_parted(
    const std::vector<int> &indexes, const std::vector<double> &speeds)
{
    for (size_t i = 0; i < indexes.size(); i++)
    {
        axes[indexes[i]]->setSpeed_ups(speeds[i]);
    }
}

ndmath::vector RalgoCncInterpolationGroup::current_position()
{
    ndmath::vector ret(axes.size());
    for (auto *ax : axes)
    {
        ret.push_back(ax->last_unit_position());
    }
    return ret;
}

void RalgoCncInterpolationGroup::command(const nos::argv &args,
                                         nos::ostream &out)
{
    if (args.size() == 0)
    {
        println_to(out, "Usage: grp <id> <command> [args...]");
        return;
    }

    auto command = args[0];

    if (args[0] == "help")
    {
        println_to(out, command_help());
        return;
    }

    if (args[0] == "speed")
    {
        if (args.size() == 1)
        {
            println_to(out, "Speed: ", speed());
            return;
        }

        if (args.size() == 2)
        {
            set_common_speed_ups(std::stod(args[1]));
            return;
        }
    }

    if (command == "pos")
    {
        println_to(out, "Pos: ", current_position());
        return;
    }

    InterpolationGroup_v2::command(args, out);
}

std::string RalgoCncInterpolationGroup::command_help()
{
    std::string ret = InterpolationGroup_v2::command_help();
    return ret;
}

const std::vector<AbstractServo *> &RalgoCncInterpolationGroup::servos_list()
{
    return servos;
}

std::string RalgoCncInterpolationGroup::info()
{
    auto &axes = axis_list();
    auto &servos = servos_list();

    std::string ret = nos::format("RalgoCncInterpolationGroup {}\n", name());
    ret += nos::format("CNC: {}\n", cnc->name());
    ret += nos::format("speed_ups: {}\n", _speed_ups);
    ret += nos::format("accel_ups2: {}\n", _accel_ups2);
    ret += nos::format("Axes: {}\n", axes.size());
    for (auto *ax : axes)
    {
        auto servo_and_symbols =
            cnc_moveapi_from_common_axis(dynamic_cast<CommonAxis *>(ax))
                ->servos_and_symbols();
        std::string servo_names;
        for (auto &[servo, symbol] : servo_and_symbols)
        {
            servo_names += servo->name() + ",";
        }
        servo_names.pop_back();
        ret += nos::format("  {} (gain:{} servos:[{}] ups:{})\n",
                           ax->name(),
                           ax->unitRatio(),
                           servo_names,
                           ax->getSpeed_ups());
    }
    ret += nos::format("Servos: {}\n", servos.size());
    for (auto *servo : servos)
    {
        auto symbol = symbol_for_servo(servo);
        ret += nos::format("  {} (gain:{} symbol:{})\n",
                           servo->name(),
                           servo->native_pulses_per_unit(),
                           symbol);
    }
    return ret;
}

std::vector<std::pair<AbstractServo *, double>>
RalgoCncInterpolationGroup::all_servos_max_velocities_native_per_second()
{
    auto axes = axis_list();
    std::vector<std::pair<AbstractServo *, double>> ret;
    for (auto *ax : axes)
    {
        auto *common_axis = dynamic_cast<CommonAxis *>(ax);
        auto *move_api =
            dynamic_cast<CNCTandemMovingClient *>(common_axis->move_api());
        auto servos_and_symbols = move_api->servos_and_symbols();
        for (auto &[servo, symbol] : servos_and_symbols)
        {
            ret.emplace_back(
                servo, servo->native_pulses_per_unit() * ax->getSpeed_ups());
        }
    }

    return ret;
}

std::vector<std::pair<AbstractServo *, double>>
RalgoCncInterpolationGroup::all_servos_max_velocities_ups()
{
    auto axes = axis_list();
    std::vector<std::pair<AbstractServo *, double>> ret;
    for (auto *ax : axes)
    {
        auto *common_axis = dynamic_cast<CommonAxis *>(ax);
        auto *move_api =
            dynamic_cast<CNCTandemMovingClient *>(common_axis->move_api());
        auto servos_and_symbols = move_api->servos_and_symbols();
        for (auto &[servo, symbol] : servos_and_symbols)
        {
            ret.emplace_back(servo, ax->getSpeed_ups());
        }
    }

    return ret;
}
