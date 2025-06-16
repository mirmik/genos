/**
    @file
*/

#include <AbstractSyncro.h>
#include <Axis.h>
#include <MitsuServoAParameter.h>
#include <exception.h>
#include <igris/result.h>
#include <igris/util/bits.h>
#include <modes.h>
#include <nos/print.h>
#include <nos/trace.h>
#include <servos/MitsubishiServo.h>

using namespace igris::result_type;

std::map<int, std::string> group_name{
    {0, "PA"},
    {1, "PB"},
    {2, "PC"},
    {3, "PD"},
    {4, "PE"},
    {5, "PF"},
};

std::map<std::string, int> group_index{
    {"PA", 0},
    {"PB", 1},
    {"PC", 2},
    {"PD", 3},
    {"PE", 4},
    {"PF", 5},
};

MitsubishiServo::MitsubishiServo(const char *name,
                                 MitsubishiCommunicator *mrs,
                                 uint8_t rs485addr) :
    AbstractServo(name), m_rs485addr(rs485addr), m_mrs(mrs)
{
    m_haveAbsoluteMove = true;
    m_haveSetReference = false;
}

/*  Метод проверяет возвращенный статус ошибки коммуникатора и в зависимости
    от них генерирует исключение.
    @critical - опция вызывает генерацию исключения в случае, если аппаратура
    сгенирировала сигнал Alarm. В общем случае исключение не генерируется,
    поскольку аварийная ситуация не есть повод к отказу от коммуникации.
    Опцию critical следует устанавливать для анализа запросов, должных привести
    к движению устройства.*/
void MitsubishiServo::errstatAnalyze(uint8_t &errstat, uint8_t critical)
{
    if (errstat & MRS_NOTCONNECTION)
    {
        transactionRefuseError();
        throw DeviceRefuseException("DeviceRefuse: errstatAnalyze");
    };
    transactionConnectionSuccess();

    if ((errstat & MRS_ALARMERROR) || forceAlarm)
    {
        uint16_t alarm_code = getAlarmStatus();
#if USE_LAST_ALARM
        AbstractServo::save_last_alarm_code((int)alarm_code);
#endif
        if (alarm_code < 0x90)
        {
            std::string alarm_code_str = std::to_string(alarm_code);
            transactionAlarmError(alarm_code_str);
            if (critical == CRITICAL)
                throw DeviceAlarmException();
        }
        else
            transactionReady();
    }
    else
    {
        transactionReady();
    }
}

void MitsubishiServo::servo_on() {}
void MitsubishiServo::servo_off() {}

void MitsubishiServo::enable_power_off_position_keeping(int64_t period)
{
    nos::log::trace("enable_power_off_position_keeping {}", period);
    // TODO: Нужно проверять права доступа к файлу.
    power_off_keeper_enabled = true;
    power_off_keeper_period = period;
}

/* Установка нулевой позиции. Ноль устанавливается в текущей точке.*/
void MitsubishiServo::setNullPosition()
{
    uint8_t errstat = 0;
    m_mrs->clear_signal(errstat, m_rs485addr);
    errstatAnalyze(errstat, CRITICAL);

    if (power_off_position_keeper)
    {
        power_off_position_keeper->update_stored_current_sector(0);
        power_off_position_keeper->reinit_with_start_position(
            request_pulse_position_in_command_units_without_keeping());
    }
}

/* Относительное перемещение */
void MitsubishiServo::relative_move(double _arg)
{
    last_opstatus_timestamp = std::chrono::system_clock::now();

    int32_t imp = _arg;
    uint8_t errstat = 0;
    m_mrs->impmode(errstat, m_rs485addr);
    errstatAnalyze(errstat, NONCRITICAL);
    DistanceUnit du = DistanceUnit::CommandUnit;
    nos::println("relative_move", imp, m_speed, m_accel_ms);
    m_mrs->epos(errstat,
                m_rs485addr,
                std::abs(imp),
                m_speed,
                m_accel_ms,
                imp > 0 ? FORWARD : BACKWARD,
                du);
    errstatAnalyze(errstat, CRITICAL);
    // m_operationStatusForce = true;
    m_mode = MODE_POS;
}

// void MitsubishiServo::prepareToMoveOperation()
//{
//    m_operationStatusForce = true;
//    m_mode = MODE_POS;
//}

void MitsubishiServo::enable_impulse_control_mode()
{
    uint8_t errstat = 0;
    m_mrs->impmode(errstat, m_rs485addr);
    errstatAnalyze(errstat, NONCRITICAL);
}

void MitsubishiServo::absolute_move(double _arg)
{
    nos::println("absolute_move", _arg);
    int32_t arg = _arg;
    auto pos = request_position();
    nos::println("absolute_move", arg, pos);
    relative_move(arg - pos);
}

void MitsubishiServo::relative_unit_move(double dist)
{
    double cp_per_uu = command_pulse_per_userunit();
    relative_move(dist * cp_per_uu);
}

void MitsubishiServo::absolute_unit_move(double dist)
{
    nos::println("absolute_unit_move", dist);

    if (_correction_enabled)
        dist = unit_position_correct(dist);

    nos::println("absolute_unit_move", dist);

    // double cp_per_uu = command_pulse_per_userunit();
    // absolute_move(dist * cp_per_uu);
    absolute_move(userunit_to_command_pulse(dist));
}

double MitsubishiServo::userunit_to_command_pulse(double dist)
{
    if (_attached_axis == nullptr)
    {
        return dist * command_pulse_per_userunit();
    }

    auto &gear = gear_settings();
    if (is_fully_closed_mode())
    {
        double command_pulse_per_userunit =
            (fully_closed_gear().load_pulse_per_userunit() /
             native_impulse_per_command_impulse());
        return dist * command_pulse_per_userunit;
    }
    else
    {
        if (gear.has_setted_value())
        {
            return dist * gear.get_forced_fullcycle_value();
        }

        double rounds = gear.units_to_rounds(dist);
        nos::println("userunit_to_command_pulse", rounds);
        return rounds * native_impulse_per_round() /
               native_impulse_per_command_impulse();
    }
}

void MitsubishiServo::setReference(int32_t dist)
{
    throw FunctionIsNotSupported();
}

int64_t
MitsubishiServo::request_pulse_position_in_command_units_without_keeping()
{
    uint8_t errstat = 0;
    DistanceUnit du = DistanceUnit::CommandUnit;
    auto pulses = m_mrs->abspos_encoder_side(errstat, m_rs485addr, du);
    errstatAnalyze(errstat, NONCRITICAL);
    return pulses;
}

int64_t MitsubishiServo::request_pulse_position_in_command_units()
{
    auto pulses = request_pulse_position_in_command_units_without_keeping();
    if (power_off_position_keeper)
        pulses = power_off_position_keeper->process_position(pulses);
    return pulses;
}

int64_t MitsubishiServo::request_pulse_position_in_servo_side_drive_units()
{
    uint8_t errstat = 0;
    DistanceUnit du = DistanceUnit::ServoSideEncoderUnit;
    auto ret = m_mrs->abspos_encoder_side(errstat, m_rs485addr, du);
    errstatAnalyze(errstat, NONCRITICAL);
    return ret;
}

int64_t MitsubishiServo::request_position()
{
    return request_pulse_position_in_command_units();
}

int64_t MitsubishiServo::request_pulse_position_in_native_units()
{
    return request_pulse_position_in_command_units() * _elgear;
}

int64_t MitsubishiServo::request_pulse_position_in_load_side_units()
{
    throw std::runtime_error("Not implemented");
}

/*  Запрос статуса концевых выключаетелей. */
uint8_t MitsubishiServo::request_limit_switch_status()
{
    uint8_t errstat = 0;
    uint8_t ret;
    int32_t input = m_mrs->input_signals(errstat, m_rs485addr);
    uint8_t lsn = bits_mask(input, (1 << 2)) == 0;
    uint8_t lsp = bits_mask(input, (1 << 1)) == 0;

    if (lsn && lsp)
        ret = 10;
    else if (lsn)
        ret = 1;
    else if (lsp)
        ret = 2;
    else
        ret = 0;

    errstatAnalyze(errstat, NONCRITICAL);
    return ret;
}

/*  Запрос статуса движения выключаетелей. */
uint8_t MitsubishiServo::request_operation_status()
{
    uint8_t errstat = 0;
    uint8_t ret;
    uint8_t out = (m_mrs->output_signals(errstat, m_rs485addr) & (1 << 5)) == 0;

    if (out)
        ret = MOVED;
    else
        ret = STOPED;

    errstatAnalyze(errstat, NONCRITICAL);

    return ret;
}

/*  Установить скорость. В данной процедуре скорость сохраняется в настройках
   класса. И передается СУ при вызове функции движения.
*/
void MitsubishiServo::setSpeed_native_impl(double spd)
{
    if (spd < 0)
    {
        throw std::runtime_error("Speed < 0");
    }

    if (spd < 1)
        spd = 1;
    m_speed = spd;
}

/*  Установить ускорение. */
void MitsubishiServo::setAccel_ms(uint32_t ms)
{
    m_accel_ms = ms;
}

void MitsubishiServo::preset()
{
    std::lock_guard<std::recursive_mutex> lock(preset_protector_mutex);
    nos::println("Preset:", name());
    stop();

    auto gen_numer = get_gen_numer();
    auto gen_denum = get_gen_denum();

    _elgear = (double)gen_numer / (double)gen_denum;
    assert(!isinf(_elgear));
    double encnum = (double)get_enc_numer();
    double encden = (double)get_enc_denum();
    _feedback_elgear = encnum / encden;
    assert(!isinf(_feedback_elgear));
    std::tie(_has_load_side_encoder, _load_side_multiplier) =
        read_load_gain_settings();

    _has_physical_limit_switches = has_physical_limit_switches_request();
    if (_has_physical_limit_switches)
        enable_limit_switch_requests(true);

    if (power_off_keeper_enabled && is_fully_closed_mode())
    {
        power_off_position_keeper.emplace("/var/poskeeper/" + name(),
                                          power_off_keeper_period);

        power_off_position_keeper->reinit_with_start_position(
            request_pulse_position_in_command_units_without_keeping());
    }

#if USE_LAST_ALARM
    int last_alarm_code = _last_alarm_runtime_binder.get();
    if (last_alarm_code == 0x25)
    {
        nos::log::info("SET ZERO POSITION because LAST_ALARM_CODE == 0x25");
        set_zero_position();
        _last_alarm_runtime_binder.update(0);
    }
#endif
}

void MitsubishiServo::on_first_connection()
{
    _has_physical_limit_switches = has_physical_limit_switches_request();
    if (_has_physical_limit_switches)
        enable_limit_switch_requests(true);
}

double MitsubishiServo::electronic_gear()
{
    return _elgear;
}

double MitsubishiServo::feedback_electronic_gear()
{
    return _feedback_elgear;
}

/*А остановка выражается в переводе объекта в мануал мод.*/
void MitsubishiServo::stop()
{
    uint8_t errstat;
    // m_mrs->impmode(errstat, m_rs485addr);
    m_mrs->temporary_stop(errstat, m_rs485addr);

    errstatAnalyze(errstat, NONCRITICAL);

    m_mode = MODE_MANUAL;
}

/*Запрос устанавливаемой скорости*/
double MitsubishiServo::settedSpeed_native()
{
    return m_speed;
}

/*Запрос устанавливаемоего ускорения*/
uint32_t MitsubishiServo::settedAccel_ms()
{
    return m_accel_ms;
}

/*Движение с постоянной скоростью.*/
void MitsubishiServo::jog(uint8_t direction)
{
    uint32_t speed = m_speed;
    uint32_t accel = m_accel_ms;
    uint8_t errstat = 0;
    m_mrs->jog(errstat, m_rs485addr, speed, accel, direction);

    errstatAnalyze(errstat, CRITICAL);

    m_mode = MODE_JOG;
}

/*Получение статуса аврийного состояния. TODO*/
int MitsubishiServo::getAlarmStatus()
{
    int ret;
    uint8_t errstat;
    try
    {
        ret = m_mrs->read_lastAlarm(errstat, m_rs485addr);
    }
    catch (const std::exception &ex)
    {
        return -1;
    }
    // errstatAnalyze(errstat, NONCRITICAL);
    return ret;
}

std::string MitsubishiServo::getAlarm()
{
    int alarmStatus = getAlarmStatus();
    std::string alarmStr;

    char hexErrCode[8];
    snprintf(hexErrCode, sizeof(hexErrCode), "0x%02X", alarmStatus);

    if (alarmStatus != 0xFF)
    {
        /*    alarmStr += "addr: ";
            alarmStr += std::to_string(m_rs485addr);
            alarmStr += " axis: ";
            alarmStr += axis->name();
            alarmStr += " error: ";*/
        alarmStr += hexErrCode;
    }
    else
        alarmStr += "No errors.";
    return alarmStr;
}

std::pair<bool, double> MitsubishiServo::read_load_gain_settings()
{
    uint8_t errstat;

    //Установка Group A
    m_mrs->parametr_group_set(errstat, m_rs485addr, 0);
    // SystemType (FullyClosed or Not)
    int rrr = m_mrs->read_parametr(errstat, m_rs485addr, 1);
    uint32_t stype = (rrr & 0x00F0) >> 4;

    if (!(stype == 0 || stype == 1))
        throw std::runtime_error("Unknown load gain type");

    if (0 == stype)
    {
        return {false, 1};
    }

    else
    {
        // Group E
        m_mrs->parametr_group_set(errstat, m_rs485addr, 4);

        // Load Side Multiplier
        double numer = m_mrs->read_parametr(errstat, m_rs485addr, 4);
        double denum = m_mrs->read_parametr(errstat, m_rs485addr, 5);

        double numer2 = m_mrs->read_parametr(errstat, m_rs485addr, 34);
        double denum2 = m_mrs->read_parametr(errstat, m_rs485addr, 35);

        double multiplier = (numer * numer2) / (denum * denum2);

        // TODO: nonclean parameter setup
        speed_multiplier = multiplier;

        return {true, multiplier};
    }
}

void MitsubishiServo::bindSyncro(AbstractSyncro *syncro)
{
    uint8_t errstat;

    m_mrs->parametr_group_set(errstat, m_rs485addr, 0);
    uint32_t elg_numer = m_mrs->read_parametr(errstat, m_rs485addr, 6);
    uint32_t elg_denum = m_mrs->read_parametr(errstat, m_rs485addr, 7);
    uint32_t enc_numer = m_mrs->read_parametr(errstat, m_rs485addr, 15);
    uint32_t enc_denum = m_mrs->read_parametr(errstat, m_rs485addr, 16);

    nos::println("elg_denum:", elg_denum);
    nos::println("elg_numer:", elg_numer);
    nos::println("enc_denum:", enc_denum);
    nos::println("enc_numer:", enc_numer);
    syncro->set_enc_numer(enc_numer * elg_denum);
    syncro->set_enc_denum(enc_denum * elg_numer);
}

bool MitsubishiServo::is_fully_closed_mode() const
{
    return _has_load_side_encoder;
}

bool MitsubishiServo::has_physical_limit_switches_request()
{
    uint8_t errstat;
    m_mrs->parametr_group_set(errstat, m_rs485addr, 3);
    auto par = m_mrs->read_parametr(errstat, m_rs485addr, 1);
    nos::println("HAS PHYS LIMIT", name(), nos::ihex(par));
    return (par & 0x0C00) != 0x0C00;
}

uint32_t MitsubishiServo::get_enc_numer()
{
    std::lock_guard lock(internal_mutex);
    uint8_t errstat;
    m_mrs->parametr_group_set(errstat, m_rs485addr, 0);
    return m_mrs->read_parametr(errstat, m_rs485addr, 15);
}

uint32_t MitsubishiServo::get_enc_denum()
{
    std::lock_guard lock(internal_mutex);
    uint8_t errstat;
    m_mrs->parametr_group_set(errstat, m_rs485addr, 0);
    return m_mrs->read_parametr(errstat, m_rs485addr, 16);
}

uint32_t MitsubishiServo::get_gen_numer()
{
    std::lock_guard lock(internal_mutex);
    uint8_t errstat;
    m_mrs->parametr_group_set(errstat, m_rs485addr, 0);
    return m_mrs->read_parametr(errstat, m_rs485addr, 6);
}

uint32_t MitsubishiServo::get_gen_denum()
{
    std::lock_guard lock(internal_mutex);
    uint8_t errstat;
    m_mrs->parametr_group_set(errstat, m_rs485addr, 0);
    return m_mrs->read_parametr(errstat, m_rs485addr, 7);
}

const nos::trent &get_named_trent(const nos::trent &ltr, std::string name)
{
    for (const nos::trent &tr : ltr.as_list())
    {
        if (tr["name"].as_string() == name)
            return tr;
    }
    throw std::runtime_error("No trent with name " + name);
}

int MitsubishiServo::updateParameters(const nos::trent &devtr)
{
    std::string tmpstr;
    uint8_t errstat;
    int needReset = UPDATE_PARAMETR_DONTNEEDED_RESTART;

    int stantion = devtr["address"].as_integer_except();

    if (stantion != this->m_rs485addr)
    {

        return UPDATE_PARAMETR_ERROR_STATION_CODE;
    }

    MitsubishiCommunicator::read_parameter_s rets;
    MitsubishiCommunicator::write_parameter_s wr;

    std::lock_guard<std::recursive_mutex> lock(m_mrs->bus_mutex);

    bool automatic_shaft_resonanse_supress =
        atoi(get_named_trent(devtr["PB"], "PB17")["value"]
                 .as_string()
                 .c_str()) == 0;

    bool fully_clossed_mode =
        get_named_trent(devtr["PA"], "PA01")["value"].as_string() ==
        std::string("1010");

    if (fully_clossed_mode)
    {
        nos::println("FULLY CLOSSED MODE");
    }
    else
    {
        nos::println(
            get_named_trent(devtr["PA"], "PA01")["value"].as_string().c_str());
    }

    uint8_t filter_mode =
        atoi(get_named_trent(devtr["PA"], "PA08")["value"].as_string().c_str());

    for (const std::string &grpname :
         std::vector<std::string>{"PA", "PB", "PC", "PD", "PE", "PF"})
    {
        for (const nos::trent &tr : devtr[grpname].as_list_except())
        {
            MitsuServoAParameter param(tr);

            m_mrs->parametr_group_set(errstat, stantion, param.gridx);
            m_mrs->read_parameter_2(errstat, stantion, param.num, &rets);

            MitsuServoAParameter readed_param(param.gridx, param.num, rets);
            nos::fprintln("readparam: {} {} {}",
                          param.gridx,
                          param.num,
                          param == readed_param);

            if (param != readed_param)
            {
                if (!fully_clossed_mode)
                {
                    std::string name = tr["name"].as_string();
                    if (name == "PE04")
                        continue;
                    if (name == "PE05")
                        continue;
                    if (name == "PE34")
                        continue;
                    if (name == "PE35")
                        continue;
                    if (name == "PC45")
                        continue;
                }

                if (automatic_shaft_resonanse_supress &&
                    tr["name"].as_string() == "PB17")
                    continue;

                if (filter_mode == 1 || filter_mode == 2)
                {
                    tmpstr = tr["name"].as_string();

                    if (filter_mode == 1)
                    {
                        if (tmpstr == "PB06")
                            continue;
                    }

                    if (tmpstr == "PB07" || tmpstr == "PB08" ||
                        tmpstr == "PB09" || tmpstr == "PB10")
                        continue;
                }

                wr = param.genwrite(stantion, rets.dpoint);
                m_mrs->parametr_group_set(errstat, stantion, param.gridx);
                m_mrs->write_parameter_2(&wr);

                if (rets.wrtype)
                    needReset = UPDATE_PARAMETR_NEED_RESTART;
            }
        }
    }
    return needReset;
}

double MitsubishiServo::native_impulse_per_command_impulse() const
{
    return _elgear;
}

double MitsubishiServo::drive_impulse_per_native_impulse() const
{
    // native impulse is a servo side impulse in common mode and
    // an load side impulse in fully_closed_mode

    if (is_fully_closed_mode())
        return 1.0 / _load_side_multiplier; // < servo_side / load_side
    else
        return 1.0;
}

double MitsubishiServo::drive_impulse_per_round() const
{
    return c_drive_pulse_per_rotation;
}

double MitsubishiServo::current_torque()
{
    uint8_t errstat;

    std::string str = m_mrs->Query(errstat, m_rs485addr, 0x01, 0x8A);

    if (errstat)
        return 0;

    return m_mrs->Parse8(str, 7);
}

std::pair<std::string, std::string>
MitsubishiServo::get_parameter(const std::string &mnemo)
{
    std::lock_guard<std::recursive_mutex> lk(m_mrs->bus_mutex);

    uint8_t errstat;
    MitsubishiCommunicator::read_parameter_s param;

    if (mnemo.size() != 4 || mnemo[0] != 'P' ||
        strchr("ABCDEFGH", mnemo[1]) == nullptr)
    {
        return {"unresolved", ""};
    }

    int num = atoi(mnemo.data() + 2);

    m_mrs->parametr_group_set(errstat, m_rs485addr, mnemo[1] - 'A');
    m_mrs->read_parameter_2(errstat, m_rs485addr, num, &param);

    auto parsed = mitsuparam_readed_value_format(
        param.data, param.dpoint, param.disptype);

    return parsed;
}

std::recursive_mutex &MitsubishiServo::bus_mutex()
{
    return m_mrs->bus_mutex;
}

void MitsubishiServo::set_parameter(const std::string &mnemo,
                                    const std::string &value,
                                    const std::string &format)
{
    MitsubishiCommunicator::read_parameter_s param;
    std::lock_guard<std::recursive_mutex> lk(m_mrs->bus_mutex);

    uint8_t errstat;

    if (mnemo.size() != 4 || mnemo[0] != 'P' ||
        strchr("ABCDEFGH", mnemo[1]) == nullptr)
    {
        return;
    }

    int num = atoi(mnemo.data() + 2);

    m_mrs->parametr_group_set(errstat, m_rs485addr, mnemo[1] - 'A');
    m_mrs->read_parameter_2(errstat, m_rs485addr, num, &param);

    auto wpar = mitsuparam_value_format(value, format, param.dpoint);

    m_mrs->write_parametr(
        errstat, m_rs485addr, num, true, wpar.decimal_point, wpar.data);
}

void MitsubishiServo::command(const nos::argv &args, nos::ostream &os)
{
    if (args.size() == 0)
    {
        nos::println_to(os, command_help());
        return;
    }

    if (args[0] == "pos")
    {
        nos::println_to(os, request_position());
        return;
    }

    else if (args[0] == "incmov")
    {
        relative_move(args[1].to_double());
        return;
    }

    else if (args[0] == "absmov")
    {
        absolute_move(args[1].to_double());
        return;
    }

    else if (args[0] == "info")
    {
        nos::println_to(os, info());
        return;
    }

    else if (args[0] == "setspd")
    {
        setSpeed_native_impl(args[1].to_double());
        return;
    }

    else if (args[0] == "setaccms")
    {
        setAccel_ms(args[1].to_double());
        return;
    }

    else if (args[0] == "input_signals")
    {
        uint8_t errstat = 0;
        auto ret = m_mrs->input_signals(errstat, m_rs485addr);
        nos::println_to(os, nos::ibin(ret));
        return;
    }

    else if (args[0] == "torque")
    {
        nos::println_to(os, current_torque());
        return;
    }

    else if (args[0] == "has_limit_switches")
    {
        nos::println_to(os, has_physical_limit_switches_request());
        return;
    }

    else if (args[0] == "setzer")
    {
        setNullPosition();
        return;
    }

    else if (args[0] == "son")
    {
        uint8_t errstat = 0;
        auto ret = m_mrs->input_signals(errstat, m_rs485addr);
        m_mrs->set_input_signals(errstat, m_rs485addr, ret | 0x01);
        return;
    }

    else if (args[0] == "soff")
    {
        uint8_t errstat = 0;
        auto ret = m_mrs->input_signals(errstat, m_rs485addr);
        m_mrs->set_input_signals(errstat, m_rs485addr, ret & (~(int32_t)0x01));
        return;
    }

    else if (args[0] == "output_signals")
    {
        uint8_t errstat = 0;
        auto ret = m_mrs->output_signals(errstat, m_rs485addr);
        nos::println_to(os, nos::ibin(ret));
        return;
    }

    else if (args[0] == "keeping_sector")
    {
        auto ncorpulses =
            request_pulse_position_in_command_units_without_keeping();
        auto sector =
            power_off_position_keeper->sectorno_of_position(ncorpulses);
        nos::println_to(os, sector);
        return;
    }

    else if (args[0] == "non_keeped_pos")
    {
        auto pos = request_pulse_position_in_command_units_without_keeping();
        nos::println_to(os, pos);
        return;
    }

    else if (args[0] == "parameter")
    {
        std::string parameter = args[1];
        auto val_and_type = get_parameter(parameter);
        nos::println_to(
            os,
            nos::format("{} : {}", val_and_type.first, val_and_type.second));
        return;
    }

    else if (args[0] == "setparameter")
    {
        std::string parameter = args[1];
        std::string value = args[2];
        std::string format = args[3];
        set_parameter(parameter, value, format);
        return;
    }

    AbstractServo::command(args, os);
}

std::string MitsubishiServo::info()
{
    return nos::format("Mitsubishi servo:\n"
                       "spd:    {}\n"
                       "acc_ms: {}\n"
                       "fully_closed: {}\n"
                       "elgear: {}\n"
                       "feedback_elgear: {}\n",
                       m_speed,
                       m_accel_ms,
                       is_fully_closed_mode(),
                       _elgear,
                       _feedback_elgear);
}

std::string MitsubishiServo::command_help()
{
    std::string ret = AbstractServo::command_help();
    ret += "pos - Request current position from servo\n";
    ret += "info - Print servo info\n";
    ret += "incmov - Relative move\n";
    ret += "absmov - Absolute move\n";
    ret += "setspd - Set speed\n";
    ret += "setaccms - Set acceleration in ms\n";
    ret += "input_signals - Print input signals\n";
    ret += "output_signals - Print output signals\n";
    ret += "torque - Print current torque\n";
    ret += "setzer - Set zero position\n";
    ret += "son - Set servo on\n";
    ret += "soff - Set servo off\n";
    ret += "non_keeped_pos - Request current position from servo without "
           "keeping\n";
    ret += "keeping_sector - get current sector if power_off_keeper is "
           "active";
    return ret;
}
