#include <moveapi/CNCMovingClient.h>
#include <CommonAxis.h>
#include <MessageBus.h>
#include <comm/Notify.h>
#include <logging.h>
#include <ralgo/linalg/vecops.h>
#include <ranges>
#include <utilxx/range.h>

std::string CNCTandemMovingClient::Record::info()
{
    return nos::format("Record: {} {} {} {} {}",
                       servo->name(),
                       cncax_symbol,
                       multiplier,
                       gear_coeff,
                       feedback_gear_coeff);
}

void CNCTandemMovingClient::init(CommonAxis *parent)
{
    this->_axis = parent;
    auto *mover = records[0].servo;
    mover->positionUpdated += igris::make_delegate(
        &CNCTandemMovingClient::notifyCurrentPosition, this);
    mover->limitSwitchStatusSignal +=
        igris::make_delegate(&AbstractAxis::limitSwitchStatusSlot,
                             dynamic_cast<AbstractAxis *>(parent));

    MoveApiClient::init(parent);

    cnc_driver->stop_operation_callback += igris::make_delegate(
        &MoveApiClient::operation_finish_callback, (MoveApiClient*)this);
    cnc_driver->start_operation_callback += igris::make_delegate(
        &MoveApiClient::operation_start_callback, (MoveApiClient*)this);

    for (auto &record : records)
    {
        record.servo->attach_axis(parent);
    }
}

void CNCTandemMovingClient::notifyCurrentPosition(int32_t arg)
{
    if (!_axis->axis_inited)
        return;

    auto &main_servo_record = records[0];
    double curpos = (double)arg * main_servo_record.gear_coeff;
    _axis->update_current_position_handler_with_reverse_if_need(
        curpos / _axis->unitRatio(), false);
}

void CNCTandemMovingClient::clear_position_notification()
{
    for (auto &sub : _servo_subscriptions)
        sub.unsubscribe();
    _servo_subscriptions.clear();
}

bool CNCTandemMovingClient::check_tandem_synchronization()
{
    int64_t reference = last_positions[0];
    for (size_t i = 0; i < multipliers().size(); ++i)
    {
        int64_t pos = last_positions[i];
        pos *= multipliers()[i];
        if (std::abs(pos - reference) > 3000)
            return false;
    }
    return true;
}

template <typename T>
rxcpp::observable<std::shared_ptr<std::vector<T>>>
zip_v(const std::vector<rxcpp::observable<T>> &observables)
{
    // map the first observable to convert values to a single-element vector
    auto it = observables.cbegin();
    rxcpp::observable<std::shared_ptr<std::vector<T>>> acc = it->map([](T t) {
        return std::make_shared<std::vector<T>>(std::initializer_list<T>{t});
    });

    // fold each observable into accumulator by zipping, and pushing back value
    while (++it != observables.cend())
    {
        acc = acc.zip(
            [](std::shared_ptr<std::vector<T>> acc, T next) {
                acc->push_back(next);
                return acc;
            },
            *it);
    }
    return acc;
}

void CNCTandemMovingClient::subscribe_to_servo_position_notification()
{
    std::vector<rxcpp::observable<std::pair<DeviceStatus, Device *>>>
        observables;

    last_positions.resize(records.size());

    _servo_subscriptions.push_back(
        _is_tandem_syncronized.get_observable()
            .distinct_until_changed()
            .subscribe([this](bool val) {
                _last_is_tandem_synchronized = val;
                _axis->isTandemSynchronizedNotifyTheme.notify(val);
            }));

    for (size_t i = 0; i < records.size(); ++i)
    {
        _servo_subscriptions.push_back(
            records[i].servo->position_observable().subscribe([this,
                                                               i](int64_t val) {
                last_positions[i] = val;
                bool correctness = check_tandem_synchronization();
                _is_tandem_syncronized.get_subscriber().on_next(correctness);
            }));

        observables.push_back(records[i].servo->status_observable());
    }

    auto zip = zip_v(observables);
    _servo_subscriptions.push_back(zip.subscribe([this](auto vecptr) {
        auto &vec = *vecptr;
        (void)vec;
    }));
}

void CNCTandemMovingClient::preset()
{
    std::lock_guard<std::recursive_mutex> lock(preset_protector_mutex);
    clear_position_notification();
    for (auto &rec : records)
    {
        rec.servo->preset();
    }
    subscribe_to_servo_position_notification();
    init_scale_multipliers();
    MoveApiClient::preset();

    cnc_driver->preset();
    update_cnc_gear_coeffs();
    update_cnc_positions();

    auto muls = multipliers();
    auto refmul = muls[0];
    for (auto &mul : muls)
        mul = mul / refmul;
    cnc_driver->declare_symbols_as_tandem(symbols(), muls, maxmistake_pulses());

    bool all_fully_closed = all_servos_in_fully_closed_mode();
    bool any_fully_closed = any_servo_in_fully_closed_mode();
    if (all_fully_closed != any_fully_closed)
    {
        auto msg = nos::format("CNCTandemMovingClient {}: servos in "
                               "different fully closed state",
                               _axis->name());
        nos::log::error(msg);
        message_bus_notify(msg);
    }
}

void CNCTandemMovingClient::update_cnc_positions()
{
    for (auto &rec : records)
    {
        auto position = rec.servo->request_position();
        cnc_driver->set_axis_position(rec.cncax_symbol, position);
    }
}

void CNCTandemMovingClient::init_scale_multipliers()
{
    auto unit2native = native_pulses_per_unit();
    double rot2native = main_servo().native_impulse_per_round();
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

// synchronize electronic gear information
void CNCTandemMovingClient::update_cnc_gear_coeffs()
{
    for (auto &record : records)
    {
        auto &servo = record.servo;
        auto elgear = servo->electronic_gear();
        auto feedback_elgear = servo->feedback_electronic_gear();
        cnc_driver->set_stepper_gear(record.cncax_symbol, elgear);
        cnc_driver->set_stepper_feedback_gear(record.cncax_symbol,
                                              feedback_elgear);
        record.gear_coeff = elgear;
        record.feedback_gear_coeff = feedback_elgear;
    }
}

void CNCTandemMovingClient::add_axis_record(Record record)
{
    records.push_back(record);
}

bool CNCTandemMovingClient::all_servos_is_ready()
{
    for (auto &rec : records)
    {
        if (!rec.servo->is_ready())
            return false;
    }
    return true;
}

int CNCTandemMovingClient::unsafe_absolute_move_iu(double tgt,
                                                   PolymorphSpeed speed,
                                                   PolymorphAccel accel)
{
    abort();
    // if (!all_servos_is_ready())
    // {
    //     nos::log::warn(
    //         "Tandem movement is not possible due to the unavailability "
    //         "of the servos.");
    //     return -1;
    // }

    // if (!_last_is_tandem_synchronized)
    // {
    //     nos::log::warn("Tandem is not synced.");
    //     return -1;
    // }

    // if (_is_position_updated_before_moving)
    // {
    //     update_cnc_gear_coeffs();
    //     update_cnc_positions();
    //     limits_reevaluate();
    // }

    // for (auto &rec : records)
    // {
    //     rec.servo->enable_impulse_control_mode();
    // }

    // assert(speed.internalpulse_per_second() != 0);
    // assert(accel.internalpulse_per_second_sqr() != 0);

    // setup_axes_accelvelocities();
    // std::vector<double> dists(records.size());

    // dists = ralgo::vecops::mul_vs(multipliers(), tgt);
    // nos::println("dists:", dists);

    // for (size_t i = 0; i < records.size(); ++i)
    // {
    //     auto *servo = records[i].servo;
    //     auto dist = servo->unit_position_correct(dists[i]);
    //     dists[i] = dist;
    // }
    // nos::println("corrected:", dists);

    // return cnc_driver->unsafe_absolute_move_iu_vec(
    //     symbols(),
    //     dists,
    //     speed.internalpulse_per_second(),
    //     accel.internalpulse_per_second_sqr());
}

int CNCTandemMovingClient::unsafe_absolute_move_units(double units_tgt,
                                                      PolymorphSpeed speed,
                                                      PolymorphAccel accel)
{
    if (!all_servos_is_ready())
    {
        nos::log::warn(
            "Tandem movement is not possible due to the unavailability "
            "of the servos.");
        return -1;
    }

    if (!_last_is_tandem_synchronized)
    {
        nos::log::warn("Tandem is not synced.");
        return -1;
    }

    if (_is_position_updated_before_moving)
    {
        update_cnc_gear_coeffs();
        update_cnc_positions();
        limits_reevaluate();
    }

    for (auto &rec : records)
    {
        rec.servo->enable_impulse_control_mode();
    }

    assert(speed.internalpulse_per_second() != 0);
    assert(accel.internalpulse_per_second_sqr() != 0);

    setup_axes_accelvelocities();
    std::vector<double> dists(records.size());

    dists = ralgo::vecops::mul_vs(multipliers(), units_tgt);
    nos::println("BEFORE CORRECTION", dists);
    nos::println("multipliers:", multipliers());
    nos::println("units_tgt:", units_tgt);

    for (size_t i = 0; i < records.size(); ++i)
    {
        auto *servo = records[i].servo;
        auto dist = servo->unit_position_correct(dists[i]);
        dists[i] = scaller.ext2int(dist);
    }
    nos::println("AFTER CORRECTION", dists);

    return cnc_driver->unsafe_absolute_move_iu_vec(
        symbols(),
        dists,
        speed.internalpulse_per_second(),
        accel.internalpulse_per_second_sqr());
}

std::vector<size_t> CNCTandemMovingClient::servo_indexes()
{
    std::vector<size_t> ret;
    for (auto &rec : records)
    {
        auto symbol = rec.cncax_symbol;
        auto index = cnc::symbol_to_index(symbol[0]);
        ret.push_back(index);
    }
    return ret;
}

int CNCTandemMovingClient::unsafe_relative_move_iu(double dist,
                                                   PolymorphSpeed speed,
                                                   PolymorphAccel accel)
{
    if (!all_servos_is_ready())
    {
        nos::log::warn(
            "Tandem movement is not possible due to the unavailability "
            "of the servos.");
        return -1;
    }

    if (!_last_is_tandem_synchronized)
    {
        nos::log::warn("Tandem is not synced.");
        return -1;
    }

    if (_is_position_updated_before_moving)
        update_cnc_positions();

    for (auto &rec : records)
    {
        rec.servo->enable_impulse_control_mode();
    }

    setup_axes_accelvelocities();
    return cnc_driver->unsafe_relative_move_iu(
        symbols(),
        multipliers(),
        dist,
        speed.internalpulse_per_second(),
        accel.internalpulse_per_second_sqr());
}

void CNCTandemMovingClient::setup_axes_accelvelocities()
{
    std::vector<std::pair<std::string, double>> speeds;
    std::vector<std::pair<std::string, double>> accels;

    auto axspeed = current_speed();
    auto axaccel = current_accel();
    for (auto &record : records)
    {
        auto symbol = record.cncax_symbol;
        speeds.emplace_back(symbol, axspeed.internalpulse_per_second());
        accels.emplace_back(symbol, axaccel.internalpulse_per_second_sqr());
    }

    cnc_driver->set_max_velocities(speeds);
    cnc_driver->set_max_accelerations(accels);
}

double CNCTandemMovingClient::get_position_iu()
{
    if (use_cnc_feedback)
    {
        auto steps =
            cnc_driver->get_feedback_steps_position(records[0].cncax_symbol);
        auto scale = records[0].gear_coeff;
        return steps * scale;
    }

    auto command_pulses =
        main_servo().currentPosition(); // get pos as command pulses
    auto pos = command_pulses *
               main_servo().electronic_gear(); // and move it to native pulses
    return pos;
}

void CNCTandemMovingClient::set_use_cnc_feedback(bool en)
{
    use_cnc_feedback = en;
}

void CNCTandemMovingClient::nullify_position()
{
    for (auto &rec : records)
    {
        rec.servo->setNullPosition();
    }
    update_cnc_positions();
}

std::vector<std::string> CNCTandemMovingClient::symbols()
{
    auto view = records | std::views::transform(
                              [](auto &rec) { return rec.cncax_symbol; });
    return std::vector(view.begin(), view.end());
}

std::vector<double> CNCTandemMovingClient::multipliers()
{
    auto view = records |
                std::views::transform([](auto &rec) { return rec.multiplier; });
    auto vec = to_vector(view);
    return vec;
}

std::vector<Device *> CNCTandemMovingClient::get_devices()
{
    auto view =
        records | std::views::transform([](auto &rec) { return rec.servo; });
    auto vec = to_vector_type<Device *>(view);

    vec.push_back(cnc_driver);
    return vec;
}

void CNCTandemMovingClient::set_cnc_driver(RalgoCNC *driver)
{
    cnc_driver = driver;
}

void CNCTandemMovingClient::stop()
{
    nos::println("CNC TANDEM MOVING CLIENT STOP");
    cnc_driver->send_stop_command();
    operation_finish_callback();
}

double CNCTandemMovingClient::native_pulses_per_unit() const
{
    if (main_servo().is_fully_closed_mode())
    {
        return native_pulses_per_unit_by_fully_closed();
    }
    return native_pulses_per_unit_by_gear();
}

double CNCTandemMovingClient::native_pulses_per_unit_by_fully_closed() const
{
    double load_pulse_per_unit = _fully_closed_gear.load_pulse_per_userunit();
    return load_pulse_per_unit;
}

AbstractServo *CNCTandemMovingClient::servo_with_index(size_t index)
{
    for (auto &rec : records)
    {
        auto rec_index = cnc::symbol_to_index(rec.cncax_symbol[0]);
        if ((size_t)rec_index == index)
            return rec.servo;
    }
    return nullptr;
}

double CNCTandemMovingClient::native_pulses_per_unit_by_gear() const
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
    double native_pulse_per_round = main_servo().native_impulse_per_round();
    double native_pulse_per_unit = native_pulse_per_round * rounds_per_unit;
    return native_pulse_per_unit;
}

AbstractServo &CNCTandemMovingClient::main_servo()
{
    return *records[0].servo;
}
AbstractServo const &CNCTandemMovingClient::main_servo() const
{
    return *records[0].servo;
}

bool CNCTandemMovingClient::all_servos_in_fully_closed_mode()
{
    return std::ranges::all_of(records, [](const auto &record) {
        return record.servo->is_fully_closed_mode();
    });
}

bool CNCTandemMovingClient::any_servo_in_fully_closed_mode()
{
    return std::ranges::any_of(records, [](const auto &record) {
        return record.servo->is_fully_closed_mode();
    });
}

bool CNCTandemMovingClient::command(const nos::argv &args, nos::ostream &os)
{
    if (args[0] == "move_info")
    {
        nos::println_to(os, info());
        nos::println_to(os, "speed:", _current_speed.unit_per_second());
        nos::println_to(os, "speed.rot:", _current_speed.rotation_per_minut());
        nos::println_to(
            os, "speed.ip:", _current_speed.internalpulse_per_second());
        return true;
    }

    return false;
}

std::vector<std::pair<AbstractServo *, std::string>>
CNCTandemMovingClient::servos_and_symbols()
{
    auto ret = records | std::views::transform([](auto &rec) {
                   return std::pair<AbstractServo *, std::string>(
                       rec.servo, rec.cncax_symbol);
               });
    return std::vector(ret.begin(), ret.end());
}

std::string CNCTandemMovingClient::command_help()
{
    return "move_info - info about move client\n";
}

std::string CNCTandemMovingClient::info()
{
    std::string records_info = "";

    for (auto &r : records)
    {
        records_info += r.info();
        records_info += "\r\n";
    }

    nos::println("CNCTandemMovingClient::info()");
    return nos::format("fully_closed: {}\n"
                       "unitRatio: {}\n"
                       "unitRatio_by_gear: {}\n"
                       "unitRatio_by_fully_closed: {}\n"
                       "gain: {}\n"
                       "{}",
                       main_servo().is_fully_closed_mode(),
                       native_pulses_per_unit(),
                       native_pulses_per_unit_by_gear(),
                       native_pulses_per_unit_by_fully_closed(),
                       gain(),
                       records_info);
}

bool CNCTandemMovingClient::is_ready()
{
    return std::ranges::all_of(
        records, [](const auto &record) { return record.servo->is_ready(); });
}

bool CNCTandemMovingClient::is_unready()
{
    return std::ranges::any_of(
        records, [](const auto &record) { return record.servo->is_unready(); });
}

std::vector<AbstractServo *> CNCTandemMovingClient::servos()
{
    std::vector<AbstractServo *> vec;
    for (auto &rec : records)
    {
        vec.push_back(rec.servo);
    }
    return vec;
}

bool CNCTandemMovingClient::is_tandem_synchronized()
{
    return _last_is_tandem_synchronized;
}