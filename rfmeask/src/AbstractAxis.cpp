/**
    @file
*/

#include <AbstractAxis.h>
#include <SyncScanApiClient.h>
#include <axes_api.h>
#include <comm/notifymap.h>
#include <defs.h>
#include <devices/DeviceChecker.h>
#include <groups/InterpolationGroup.h>
#include <moveapi/MoveApiClient.h>
#include <ndmath/util.h>
#include <nos/log/logger.h>
#include <nos/trent/binder.h>
#include <nos/trent/json_print.h>
#include <nos/trent/settings.h>
#include <tables.h>

#define EPSILON_UNIT 0.1

// bool AbstractAxis::isActive()
// {
//     if (for_modes.size() == 0)
//         return true;

//     for (auto m : for_modes)
//         if (m == system_ctrmode)
//             return true;

//     return false;
// }

AbstractAxis::AbstractAxis(const std::string &name) :
    LogicalDevice(name, "axes")
{
    unitBackwardLimit_syncer = new nos::trent_binder<double>;
    unitForwardLimit_syncer = new nos::trent_binder<double>;
    init_runtime_settings();
    load_one_axis_correction_from_runtime();

    // open_hooks();
}

void AbstractAxis::init_hooks()
{
    nos::println("!!!!!!!!!!!!!!Init hooks for axis: ", name());
    std::string hooks_dir = "/home/rfmeas/.rfmeask/hooks";
    std::string filename = hooks_dir + "/is_moving_allowed_" + name() + ".py";
    hook_is_moving_allowed.init(filename, "result = True\n");
}

nos::trent AbstractAxis::compile_system_state_to_trent()
{
    nos::trent state;
    state["axes"].init(nos::trent_type::list);

    auto &list = axis_list_ref();

    for (size_t i = 0; i < list.size(); i++)
    {
        state["axes"][i]["name"] = list[i]->name();
        state["axes"][i]["curpos"] = list[i]->last_unit_position();
    }

    return state;
}

bool AbstractAxis::check_is_moving_allowed(double start, double final)
{
    std::vector<std::pair<int, double>> glbperm;
    glbperm.push_back(std::make_pair(number(), final));
    bool global_perm = GlobalMoveAllowed(glbperm);
    if (!global_perm)
    {
        throw AxisLimitException();
    }

    nos::trent indata, outdata;
    nos::trent system_state = compile_system_state_to_trent();
    indata["system_state"] = system_state;
    indata["axno"] = number();
    indata["start_position"] = start;
    indata["final_position"] = final;
    hook_is_moving_allowed.execute(indata, outdata);
    nos::println("IS MOVING ALLOWED: ", nos::json::to_string(outdata));
    nos::println("IS MOVING ALLOWED: ", outdata["result"].as_bool());
    if (!outdata["result"].is_bool())
    {
        throw AxisLimitException();
    }
    return outdata["result"].as_bool();
}

void AbstractAxis::unitLimits(double back, double forw)
{
    if (back > forw)
        throw WrongValueException();

    unitBackwardLimit(back);
    unitForwardLimit(forw);
}

void AbstractAxis::init_runtime_settings()
{
    unitBackwardLimit_syncer->init(*runtime_syncer, "unitBackwardLimit");
    unitForwardLimit_syncer->init(*runtime_syncer, "unitForwardLimit");
    unitBackwardLimit_syncer->sync_default(0);
    unitForwardLimit_syncer->sync_default(0);
    limits_update();
}

void AbstractAxis::notifiesRegistry()
{
    std::function<void(
        NotifyTheme &, const char *, int32_t, const char *, int32_t)>
        notifyreg = [](NotifyTheme &theme,
                       const char *str,
                       int32_t number,
                       const char *header,
                       int32_t tag) {
            char buf[48];
            sprintf(buf, str, number);
            theme.setName(buf);
            theme.setNCPIHeader(header, std::vector<int32_t>({number}));
            brocker.themeRegistry(&theme, tag, std::vector<int32_t>({number}));
        };

    notifyreg(operationStatusNotify,
              "AxisOperationStatus(%d)",
              number(),
              NPattern_AxisOperationStatus,
              Tag_AxisOperationStatus);
    notifyreg(operationStopTypeNotify,
              "AxisOperationStopType(%d)",
              number(),
              NPattern_AxisOperationStopType,
              Tag_AxisOperationStopType);
    notifyreg(checker.statusNotify,
              "AxisStatus(%d)",
              number(),
              NPattern_AxisStatus,
              Tag_AxisStatus);
    notifyreg(positionNotify,
              "AxisPosition(%d)",
              number(),
              NPattern_AxisPosition,
              Tag_AxisPosition);
    notifyreg(positionUnitNotify,
              "AxisUnitPosition(%d)",
              number(),
              NPattern_AxisUnitPosition,
              Tag_AxisUnitPosition);
    notifyreg(positionUnitNotifyNotCorrected,
              "AxisUnitPositionNotCorrected(%d)",
              number(),
              NPattern_AxisUnitPositionNotCorrected,
              Tag_AxisUnitPositionNotCorrected);
    notifyreg(torqueNotifyTheme,
              "AxisTorque(%d)",
              number(),
              NPattern_AxisTorque,
              Tag_AxisTorque);
    notifyreg(limitSwitchStatusNotify,
              "AxisLimitSwitch(%d)",
              number(),
              NPattern_AxisLimitSwitch,
              Tag_AxisLimitSwitch);
    notifyreg(scanPointNotify,
              "AxisScanPoint(%d)",
              number(),
              NPattern_AxisScanPoint,
              Tag_AxisScanPoint);
    notifyreg(scanFinishNotify,
              "AxisScanFinish(%d)",
              number(),
              NPattern_AxisScanFinish,
              Tag_AxisScanFinish);
    notifyreg(scanTrigErrorNotify,
              "AxisScanTrigError(%d)",
              number(),
              NPattern_AxisScanTrigError,
              Tag_AxisScanTrigError);
    notifyreg(isTandemSynchronizedNotifyTheme,
              "AxisTandemSynchronized(%d)",
              number(),
              NPattern_AxisTandemSynchronized,
              Tag_AxisTandemSynchronized);
}

void AbstractAxis::limitSwitchStatusSlot(uint8_t status)
{
    limitSwitchStatusNotify.notify(status);
}

void AbstractAxis::checkAxisIsReady()
{
    if (checker.getStatus() == DeviceChecker::Unready)
    {
        nos::log::warn("axis isn`t ready. throw exception");
        throw AxisUnreadyException();
    };
}

void AbstractAxis::scanMove(int32_t arg)
{
    if (scan_client)
    {
        scan_client->scanMove(arg);
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

void AbstractAxis::scanUnitMove(double arg)
{
    if (scan_client)
    {
        return scan_client->scanUnitMove(arg);
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

void AbstractAxis::scanForwardZone(int32_t arg)
{
    if (scan_client)
    {
        scan_client->scanForwardZone(arg);
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

void AbstractAxis::scanUnitForwardZone(double arg)
{
    if (scan_client)
    {
        scan_client->scanUnitForwardZone(arg);
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

void AbstractAxis::scanBackwardZone(int32_t arg)
{
    if (scan_client)
    {
        scan_client->scanBackwardZone(arg);
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

void AbstractAxis::scanUnitBackwardZone(double arg)
{
    if (scan_client)
    {
        scan_client->scanUnitBackwardZone(arg);
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

void AbstractAxis::scanStart()
{
    FUNCTION_IS_NOT_RELEASED
}

void AbstractAxis::scanCompareStart()
{
    if (scan_client)
    {
        scan_client->scanCompareStart();
    }

    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

void AbstractAxis::scanPoints(int32_t arg)
{
    if (scan_client)
    {
        scan_client->scanPoints(arg);
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

int32_t AbstractAxis::scanMove()
{
    if (scan_client)
    {
        return scan_client->scanMove();
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

double AbstractAxis::scanUnitMove()
{
    if (scan_client)
    {
        return scan_client->scanUnitMove();
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

int32_t AbstractAxis::scanForwardZone()
{
    if (scan_client)
    {
        return scan_client->scanForwardZone();
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

double AbstractAxis::scanUnitForwardZone()
{
    if (scan_client)
    {
        return scan_client->scanUnitForwardZone();
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

int32_t AbstractAxis::scanBackwardZone()
{
    if (scan_client)
    {
        return scan_client->scanBackwardZone();
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

void AbstractAxis::generate_trigger()
{
    if (scan_client)
    {
        return scan_client->generate_trigger();
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

void AbstractAxis::scanEnableExternalTriggerNotify()
{
    if (scan_client)
    {
        scan_client->enable_external_trigger_notify();
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

void AbstractAxis::manual_trigger_mode(int32_t en)
{
    if (scan_client)
    {
        return scan_client->manual_trigger_mode(en);
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}
void AbstractAxis::no_trigger_mode(int32_t en)
{
    if (scan_client)
    {
        return scan_client->no_trigger_mode(en);
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}
void AbstractAxis::bitrigmode(int32_t en)
{
    if (scan_client)
    {
        return scan_client->bitrigmode(en);
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

double AbstractAxis::scanUnitBackwardZone()
{
    if (scan_client)
    {
        return scan_client->scanUnitBackwardZone();
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

int32_t AbstractAxis::scanPoints()
{
    if (scan_client)
    {
        return scan_client->scanPoints();
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

int32_t AbstractAxis::trigreturn_time()
{
    if (scan_client)
    {
        return scan_client->trigreturn_time();
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

int32_t AbstractAxis::scan_get_encoder_position()
{
    if (scan_client)
    {
        return scan_client->get_encoder_position();
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

double AbstractAxis::unitBackwardLimit()
{
    return unitBackwardLimit_syncer->get();
}

double AbstractAxis::unitForwardLimit()
{
    return unitForwardLimit_syncer->get();
}

double AbstractAxis::unitBackwardLimit() const
{
    return unitBackwardLimit_syncer->get();
}

double AbstractAxis::unitForwardLimit() const
{
    return unitForwardLimit_syncer->get();
}

void AbstractAxis::unitBackwardLimit(double arg)
{
    unitBackwardLimit_syncer->update(arg);
    limits_update();
}

void AbstractAxis::unitForwardLimit(double arg)
{
    unitForwardLimit_syncer->update(arg);
    limits_update();
}

uint32_t AbstractAxis::minAccel_ms() const
{
    FUNCTION_IS_NOT_RELEASED
}
void AbstractAxis::minAccel_ms(uint32_t){FUNCTION_IS_NOT_RELEASED}

uint32_t AbstractAxis::maxSpeed_rpm() const
{
    FUNCTION_IS_NOT_RELEASED
}
void AbstractAxis::maxSpeed_rpm(uint32_t)
{
    FUNCTION_IS_NOT_RELEASED
}

void AbstractAxis::setSpeed_rpm(int32_t rpm){FUNCTION_IS_NOT_RELEASED}

uint32_t AbstractAxis::defaultSpeed_rpm() const
{
    FUNCTION_IS_NOT_RELEASED
}
void AbstractAxis::defaultSpeed_rpm(uint32_t)
{
    FUNCTION_IS_NOT_RELEASED
}

void AbstractAxis::setAccel_ms(int32_t ms){FUNCTION_IS_NOT_RELEASED}

uint32_t AbstractAxis::defaultAccel_ms() const
{
    FUNCTION_IS_NOT_RELEASED
}
void AbstractAxis::defaultAccel_ms(uint32_t)
{
    FUNCTION_IS_NOT_RELEASED
}

void AbstractAxis::onReadyHandler()
{
    FUNCTION_IS_NOT_RELEASED
}
void AbstractAxis::onMoverReady(Device *)
{
    FUNCTION_IS_NOT_RELEASED
}
void AbstractAxis::onCriticalErrorHandler(Device *dev)
{
    FUNCTION_IS_NOT_RELEASED
}

void AbstractAxis::preset()
{
    FUNCTION_IS_NOT_RELEASED
}
void AbstractAxis::stopLastOperation(StopType arg)
{
    FUNCTION_IS_NOT_RELEASED
}

void AbstractAxis::interpolationRelativeUnitMove(
    double arg, const std::vector<std::pair<int, double>> &subax)
{
    FUNCTION_IS_NOT_RELEASED
}
void AbstractAxis::interpolationAbsoluteUnitMove(
    double arg,
    const std::vector<std::pair<int, double>> &subax){FUNCTION_IS_NOT_RELEASED}

int32_t AbstractAxis::getAccel_ms() const
{
    FUNCTION_IS_NOT_RELEASED
}

double AbstractAxis::last_unit_position()
{
    // std::lock_guard<std::mutex> lock(last_data_mutex);
    return _last_position_unit;
}

int AbstractAxis::getLimitSwitchStatus(){FUNCTION_IS_NOT_RELEASED}

int32_t AbstractAxis::getStatus()
{
    return checker.getStatus();
}

std::string AbstractAxis::status_string()
{
    FUNCTION_IS_NOT_RELEASED
}

void AbstractAxis::setNullPosition()
{
    FUNCTION_IS_NOT_RELEASED
}

bool AbstractAxis::haveScan()
{
    if (scan_client == nullptr)
    {
        return false;
    }
    return true;
}
bool AbstractAxis::haveAbsoluteMove()
{
    return false;
}
bool AbstractAxis::haveSetReference()
{
    return false;
}
bool AbstractAxis::hasInterpolation()
{
    return false;
}

int32_t AbstractAxis::synchroSpeedTest()
{
    FUNCTION_IS_NOT_RELEASED
}

void AbstractAxis::operation_finish_callback()
{
    nos::println("OPERATION FINISH CALLBACK");
    if (igcontroller)
    {
        igcontroller->on_axis_operation_status(this, Operation::NONE);
        return;
    }

    operation = Operation::NONE;
    operationStatusNotify.notify((int)Operation::NONE);
}

void AbstractAxis::start_operation_notify()
{
    nos::println("START OPERATION NOTIFY");
    if (igcontroller)
    {
        igcontroller->on_axis_operation_status(this, Operation::USERMOVE);
        return;
    }

    operation = Operation::USERMOVE;
    operationStatusNotify.notify((int)Operation::USERMOVE);
}

void AbstractAxis::send_operation_status_notify_for_interpolation_mode(
    Operation operation)
{
    operationStatusNotify.notify((int)operation);
}

void AbstractAxis::servo_on()
{
    return;
}
void AbstractAxis::servo_off()
{
    return;
}

int32_t AbstractAxis::getOperationCode()
{
    return (int32_t)operation;
}

AbstractAxis::~AbstractAxis() = default;

int32_t AbstractAxis::scan_discrete_output_set_state(int num, int state)
{
    if (scan_client)
    {
        return scan_client->discrete_output_set_state(num, state);
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

int32_t AbstractAxis::scan_discrete_output_set_unical(int num)
{
    if (scan_client)
    {
        return scan_client->discrete_output_set_unical(num);
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

int32_t AbstractAxis::scan_discrete_output_disable()
{
    if (scan_client)
    {
        return scan_client->discrete_output_disable();
    }
    else
    {
        FUNCTION_IS_NOT_RELEASED
    }
}

void AbstractAxis::save_one_axis_correction_to_runtime()
{
    auto tr = one_axis_correction_as_trent();
    runtime_syncer->node()["one_axis_correction"] = tr;
    runtime_syncer->save();
}

nos::trent AbstractAxis::one_axis_correction_as_trent() const
{
    if (!_has_correction)
    {
        nos::trent ret;
        ret["enabled"] = false;
        return ret;
    }

    auto ret = one_axis_cartesian_correction::correction_to_trent(
        one_axis_correction_table);
    ret["enabled"] = _correction_enabled;
    return ret;
}

void AbstractAxis::load_one_axis_correction_from_runtime()
{
    // Запись содержит два обязательных поля - координаты точек points и
    // смещения deltas. И ключ enabled, указывающий, активировано ли применение
    // таблицы.
    auto &tr = runtime_syncer->node();
    if (tr.contains("one_axis_correction"))
    {
        auto cc = one_axis_cartesian_correction::trent_to_correction(
            tr["one_axis_correction"]);
        auto enabled = tr["one_axis_correction"]["enabled"].as_bool();
        set_one_axis_correction(cc, enabled);
    }
}

void AbstractAxis::command(const nos::argv &args, nos::ostream &out)
{
    if (args.size() == 0)
    {
        nos::println_to(out, command_help());
    }

    if (args[0] == "help")
    {
        nos::println_to(out, command_help());
        return;
    }
    else if (args[0] == "name")
    {
        nos::println_to(out, name());
        return;
    }
    else if (args[0] == "type")
    {
        nos::println_to(out, declared_type());
        return;
    }

    if (args[0] == "absmove")
    {
        absoluteUnitMove(std::stod(args[1]));
        return;
    }

    if (args[0] == "incmove")
    {
        relativeUnitMove(std::stod(args[1]));
        return;
    }

    if (args[0] == "stop")
    {
        stop();
        return;
    }

    if (args[0] == "setzer")
    {
        setNullPosition();
        return;
    }

    if (args[0] == "spd")
    {
        if (args.size() == 1)
        {
            nos::println_to(out, getSpeed_rpm());
            return;
        }
        else
        {
            setSpeed_rpm(std::stod(args[1]));
            return;
        }
        return;
    }

    if (args[0] == "uspd")
    {
        if (args.size() == 1)
        {
            nos::println_to(out, getSpeed_ups());
            return;
        }
        else
        {
            setSpeed_ups(std::stod(args[1]));
            return;
        }
        return;
    }

    if (args[0] == "pos")
    {
        nos::println_to(out, last_unit_position());
        return;
    }

    if (args[0] == "info")
    {
        nos::fprintln_to(out,
                         "uposition: {}\n"
                         "speed: {}\n"
                         "uspeed: {}\n"
                         "acceleration: {}\n"
                         "opstatus: {}\n",
                         last_unit_position(),
                         getSpeed_rpm(),
                         getSpeed_ups(),
                         getAccel_ms(),
                         getOperationCode());
        return;
    }

    nos::println_to(out, "Undefined command");
    return;
}

std::string AbstractAxis::command_help()
{
    return "help - show this help\n"
           "name - show axis name\n"
           "type - show axis type\n"
           "stop - stop\n";
}

void AbstractAxis::relativeUnitMove(double arg)
{
    auto curpos = last_unit_position();
    auto tgtpos = arg + curpos;
    absoluteUnitMove(tgtpos);
}

void AbstractAxis::absoluteUnitMove_delegateToIGroup(double tgtpos)
{
    if ((tgtpos > (unitForwardLimit() + EPSILON_UNIT)) ||
        (tgtpos < (unitBackwardLimit() - EPSILON_UNIT)))
    {
        throw AxisLimitException();
    }

    GrpAxisMovePair task = {this, tgtpos};
    igcontroller->absmove_by_blocks({task});
}

void AbstractAxis::absoluteUnitMove(double tgtpos)
{
    auto curpos = last_unit_position();

    if (is_need_to_prevent_moving_to_position(tgtpos, true))
        return;

    if (igcontroller)
        return absoluteUnitMove_delegateToIGroup(tgtpos);

    if (!check_is_moving_allowed(curpos, tgtpos))
        return;

    if (correction_table_used())
    {
        try
        {
            tgtpos = one_axis_correction_table.corrected_point(
                ndmath::vector{tgtpos})[0];
        }
        catch (const std::exception &e)
        {
            nos::println("absoluteUnitMove: use correction: {}", e.what());
        }
    }

    if ((tgtpos > (unitForwardLimit() + EPSILON_UNIT)) ||
        (tgtpos < (unitBackwardLimit() - EPSILON_UNIT)))
    {
        throw AxisLimitException();
    }

    if (is_reversed())
    {
        tgtpos = -tgtpos;
    }

    absoluteUnitMove_impl(tgtpos, true);
}

bool AbstractAxis::is_need_to_prevent_moving_to_position(
    double tgtpos, bool notify_if_prevented)
{
#if TRUCK_PROJECT
    if (name() == "Sl")
    {
        auto El_axis = get_axis("El");
        auto El_curpos = El_axis->last_unit_position();
        if ((El_curpos < -1 || El_curpos > 1) && (tgtpos < 171))
        {
            nos::println(
                "TRUCK PROTECTION: Try to Sl moving in wrong El position");
            return true;
        }
    }
    if (name() == "El")
    {
        auto Sl_axis = get_axis("Sl");
        auto Sl_curpos = Sl_axis->last_unit_position();
        if (Sl_curpos < 171 && tgtpos != 0)
        {
            nos::println(
                "TRUCK PROTECTION: Try to move El in wrong Sl position");
            return true;
        }
    }
#endif

    return false;
}

void AbstractAxis::jog(Direction dir)
{
#if TRUCK_PROJECT
    auto curpos = last_unit_position();

    if (dir == Direction::FORWARD)
    {
        auto limit = unitForwardLimit();
        if (name() == "El")
        {
            auto Sl_axis = get_axis("Sl");
            auto Sl_curpos = Sl_axis->last_unit_position();
            if (Sl_curpos < 171)
            {
                if (curpos < 0)
                    limit = 0;
            }
        }
        absoluteUnitMove(limit);
    }
    else
    {
        auto limit = unitBackwardLimit();
        if (name() == "Sl")
        {
            auto El_axis = get_axis("El");
            auto El_curpos = El_axis->last_unit_position();
            if ((El_curpos < -1 || El_curpos > 1))
            {
                limit = 171;
            }
        }
        if (name() == "El")
        {
            auto Sl_axis = get_axis("Sl");
            auto Sl_curpos = Sl_axis->last_unit_position();
            if (Sl_curpos < 171)
            {
                if (curpos > 0)
                    limit = 0;
            }
        }
        absoluteUnitMove(limit);
    }
#else
    if (dir == Direction::FORWARD)
        absoluteUnitMove(unitForwardLimit());
    else
        absoluteUnitMove(unitBackwardLimit());

#endif
}

void AbstractAxis::update_current_position_handler_with_reverse_if_need(
    double noncorrected_unitpos, bool force)
{
    assert(!isinf(noncorrected_unitpos));
    assert(!isnan(noncorrected_unitpos));
    auto value = is_reversed() ? -noncorrected_unitpos : noncorrected_unitpos;
    update_current_position_handler(value, force);
}

void AbstractAxis::update_current_position_handler(double noncorrected_unitpos,
                                                   bool force)
{
    if (!axis_inited)
    {
        return;
    }

    if (unitRatio() == 0)
    {
        nos::fprintln("UnitRatio is zero. axname{}", name());
    }

    assert(!isinf(noncorrected_unitpos));
    assert(unitRatio() != 0);

    {
        double unitpos_reverse_correction;
        if (igcontroller)
        {
            unitpos_reverse_correction =
                igcontroller->update_position_and_reverse_correct_unit_position(
                    this, noncorrected_unitpos);
        }
        else
        {
            unitpos_reverse_correction =
                reverse_corrected_unit_position(noncorrected_unitpos);
        }
        std::lock_guard<std::mutex> lock(last_data_mutex);
        _last_position_unit = unitpos_reverse_correction;
        _last_position_unit_not_corrected = noncorrected_unitpos;
    }

    // positionNotify.notifySmooth(_last_position, force);
    positionUnitNotify.notifySmooth(_last_position_unit, force);
    positionUnitNotifyNotCorrected.notifySmooth(
        _last_position_unit_not_corrected, force);
}

void AbstractAxis::stop()
{
    if (igcontroller)
    {
        igcontroller->stop();
        return;
    }
    else
    {
        stop_impl();
    }
}

bool AbstractAxis::correction_table_used()
{
    if (igcontroller)
    {
        return igcontroller->is_correction_enabled();
    }

    return _correction_enabled && has_correction_table();
}
