#include <KollmorgenServo.h>
#include <chrono>
#include <exception.h>
#include <nos/fprint.h>
//#include <nos/log.h>

void KollmorgenServoControl::preset()
{
    nos::log::info("KollmorgenServoControl: preset");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    get_parameter("clrfault");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    /*if (_enable_servo_on_preset)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        try
        {
            servo_on();
        }
        catch (...)
        {
            nos::log::error("KollmorgenServoControl: servo_on failed");
            return;
        }
    }*/

    // Этот код хорошо устанавливает параметры, но с ним почему-то
    // нифига не работает.
    /*while (1)
    {
        nos::println("try to load parameters");
        // get_parameter("load");

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto in1mode_value = get_parameter("in1mode");
        auto in2mode_value = get_parameter("in2mode");
        auto in3mode_value = get_parameter("in3mode");
        auto in4mode_value = get_parameter("in4mode");
        nos::fprintln("inmodes1: {} {} {} {}",
                      in1mode_value,
                      in2mode_value,
                      in3mode_value,
                      in4mode_value);

        if (in3mode_value == 2 && in4mode_value == 3)
        {
            break;
        }

        while (1)
        {
            // read inmode parameters
            auto in1mode_value = get_parameter("in1mode");
            auto in2mode_value = get_parameter("in2mode");
            auto in3mode_value = get_parameter("in3mode");
            auto in4mode_value = get_parameter("in4mode");
            nos::fprintln("inmodes2: {} {} {} {}",
                          in1mode_value,
                          in2mode_value,
                          in3mode_value,
                          in4mode_value);

            if (in3mode_value != 2 || in4mode_value != 3)
            {
                // enable limit switches (SERCOS2 manual. section: 3.182)
                set_parameter("in3mode", 2);
                set_parameter("in4mode", 3);
            }
            else
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        nos::println("try to save parameters");
        get_parameter("save");

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }*/

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    set_parameter("opmode", 8);
    set_parameter("brake_disable_delay", 200); // default
    set_parameter("brake_enable_delay", -10);  // default

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    set_parameter("pgeari", 1048576);
    set_parameter("pgearo", 1);
    set_parameter("opmode", 8);
    set_parameter("o_p", 0);
    set_parameter("o_v", speedToSet(60));
    set_parameter("o_c", 1);

    set_parameter("maxaccel", 30);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    set_parameter("move", 0);
    setReference(savedPosition);

    // Непосредственная активация двигателя осуществляется в
    // updater функции.
    if (_enable_servo_on_preset)
        _is_automatic_servo_on_applied = false;

    presetted = true;
}

void KollmorgenServoControl::servo_on()
{
    get_parameter("en");
    setReference(savedPosition);
}

void KollmorgenServoControl::servo_off()
{
    get_parameter("dis");
}

void KollmorgenServoControl::errstatAnalyze(uint8_t &errstat)
{
    if (errstat)
    {
        transactionRefuseError();
        throw DeviceRefuseException();
    };
    transactionConnectionSuccess();
    transactionReady();
}

int32_t KollmorgenServoControl::speedToSet(int32_t spd)
{
    return spd * 4.6;
}

KollmorgenServoControl::KollmorgenServoControl(const char *name,
                                               uint8_t stantion) :
    AbstractServo(name), m_stantion(stantion)
{
    logger.set_level(nos::log::level::Info);
    m_haveAbsoluteMove = true;
    m_haveSetReference = true;
}

void KollmorgenServoControl::setNullPosition()
{
    setReference(0);
}

void KollmorgenServoControl::relative_move(double dist)
{
    nos::log::debug("relative_move: {}", dist);
    nos::log::debug("setted o_v value: {}", speedToSet(m_speed_rpm));
    set_parameter("o_v", speedToSet(m_speed_rpm));
    set_parameter("o_p", dist);
    set_parameter("move", 0);
}

void KollmorgenServoControl::absolute_move(double dist)
{
    auto curpos = request_position();
    auto delta = dist - curpos;
    nos::log::debug("use relative move, because absolute move not implemented");
    relative_move(delta);

    /*FUNCTION_IS_NOT_RELEASED
    // it is not work correct
    set_parameter("o_v", speedToSet(m_speed_rpm));
    set_parameter("o_p", dist);
    set_parameter("move", 1); // TODO*/
}

int64_t KollmorgenServoControl::request_position()
{
    uint8_t errstat = 0;
    auto ret = cankollm.sdoQuery(errstat, 1, 0x40, 0x35C8, 0x01, 0);
    auto status = get_status_word();
    bool voltage_enabled = is_voltage_enabled(status);

    errstatAnalyze(errstat);

    if (!presetted)
        return ret;

    if (errstat == 0 && isConnected() && voltage_enabled)
    {
        savedPosition = ret;
        int sts0 = lseek(savepos_fd, 0, SEEK_SET);
        if (sts0 < 0)
        {
            nos::log::error("lseek error: {}", strerror(errno));
        }
        int sts1 = write(savepos_fd, &savedPosition, 4);
        if (sts1 < 0)
        {
            nos::log::error("write error: {}", strerror(errno));
        }
        int sts2 = fsync(savepos_fd);
        if (sts2 < 0)
        {
            nos::log::error("fsync error: {}", strerror(errno));
        }
    }
    return ret;
}

uint8_t KollmorgenServoControl::request_operation_status()
{
    int ret = get_parameter("inpos") == 0;
    return ret;
}

void KollmorgenServoControl::setReference(int32_t dist)
{
    set_parameter("roffs", dist);
    get_parameter("setref");
    printf("setReference: %d\r\n", dist);
}

void KollmorgenServoControl::setSpeed_rpm(uint32_t spd)
{
    m_speed_rpm = spd;
}

void KollmorgenServoControl::setSpeed_native_impl(double spd)
{
    setSpeed_rpm(spd);
}

void KollmorgenServoControl::setAccel_ms(uint32_t ms)
{
    m_accel_ms = ms;
}

uint32_t KollmorgenServoControl::settedSpeed_rpm()
{
    return m_speed_rpm;
}

uint32_t KollmorgenServoControl::settedAccel_ms()
{
    return m_accel_ms;
}

/*	Установка параметра, перечень которых дан в файле kollmap.cpp
 */
void KollmorgenServoControl::set_parameter(const char *str, int val)
{
    if (kollmmap.find(str) == kollmmap.end())
        nos::fprintln("not contains {}", str);
    auto r = kollmmap[str];

    if (r.access != KollmAccess::rw && r.access != KollmAccess::w)
    {
        printf("ReadOnly\r\n");
        return;
    }

    uint8_t errstat = 0;
    cankollm.sdoQuery(
        errstat, m_stantion, kollm_wcode(r.type), r.index, r.subindex, val);
    errstatAnalyze(errstat);
    return;
}

/*	Запрос параметра (а так же вызов функции).
 */
int KollmorgenServoControl::get_parameter(const char *str, bool checkerror)
{
    if (kollmmap.find(str) == kollmmap.end())
        nos::fprintln("not contains {}", str);
    std::string key = str;
    auto r = kollmmap[key];

    if (r.type == KollmType::INVALID)
    {
        nos::log::warn("try to get invalid parameter");
        return -1;
    }

    if (r.access != KollmAccess::rw && r.access != KollmAccess::ro)
    {
        printf("WriteOnly\r\n");
        return 0;
    }

    uint8_t errstat = 0;
    auto ret =
        cankollm.sdoQuery(errstat, m_stantion, 0x40, r.index, r.subindex, 0);
    if (checkerror)
        errstatAnalyze(errstat);
    return ret;
}

void KollmorgenServoControl::setMode(const char *str)
{
    uint8_t errstat = 0;
    if (!strcmp("off", str))
    {
        cankollm.sdoQuery(errstat, m_stantion, 0x2B, 0x6040, 0x00, 0x06);
        return;
    }
    if (!strcmp("on", str))
    {
        cankollm.sdoQuery(errstat, m_stantion, 0x2B, 0x6040, 0x00, 0x07);
        return;
    }
    if (!strcmp("disv", str))
    {
        cankollm.sdoQuery(errstat, m_stantion, 0x2B, 0x6040, 0x00, 0x00);
        return;
    }
    if (!strcmp("en", str))
    {
        cankollm.sdoQuery(errstat, m_stantion, 0x2B, 0x6040, 0x00, 0x0F);
        return;
    }
    if (!strcmp("dis", str))
    {
        cankollm.sdoQuery(errstat, m_stantion, 0x2B, 0x6040, 0x00, 0x07);
        return;
    }
    if (!strcmp("res", str))
    {
        cankollm.sdoQuery(errstat, m_stantion, 0x2B, 0x6040, 0x00, 0x010F);
        return;
    }

    errstatAnalyze(errstat);
    return;
}

void KollmorgenServoControl::stop()
{
    get_parameter("stop");
    set_parameter("o_p", 0);
    set_parameter("move", 0);
}

void KollmorgenServoControl::init()
{
    nos::log::info("KollmorgenServoControl::init()");
    int32_t pos;
    savepos_fd = open("/home/rfmeas/.Kollm0", O_RDONLY);
    if (savepos_fd > 0)
    {
        int ret = read(savepos_fd, &pos, 4);
        if (ret == 0)
            pos = 0;
        savedPosition = pos;
        nos::println("Kollmorgen: Restore position data", savedPosition);
        close(savepos_fd);
        savepos_fd = open("/home/rfmeas/.Kollm0", O_WRONLY);
    }

    else
    {
        savepos_fd = open("/home/rfmeas/.Kollm0", O_WRONLY | O_CREAT, 0666);
        if (savepos_fd <= 0)
        {
            nos::log::error("second open savepos_fd error");
            perror("second open");
        }
        int32_t zero_position = 0;
        write(savepos_fd, &zero_position, 4);
        close(savepos_fd);
        savepos_fd = open("/home/rfmeas/.Kollm0", O_WRONLY);
    }

    inited = true;
}

int KollmorgenServoControl::getAlarmStatus()
{
    throw FunctionIsNotSupported();
}

void KollmorgenServoControl::hardware_limit_switches_update()
{

    bool forward_hardware_limit = !get_parameter("in3");
    bool backward_hardware_limit = !get_parameter("in4");

    if (_last_forward_hardware_limit != forward_hardware_limit)
    {
        if (forward_hardware_limit)
            nos::log::info("forward hardware limit switch is on");
        else
            nos::log::info("forward hardware limit switch is off");
    }

    if (_last_backward_hardware_limit != backward_hardware_limit)
    {
        if (backward_hardware_limit)
            nos::log::info("backward hardware limit switch is on");
        else
            nos::log::info("backward hardware limit switch is off");
    }

    _last_forward_hardware_limit = forward_hardware_limit;
    _last_backward_hardware_limit = backward_hardware_limit;
}

void KollmorgenServoControl::voltage_status_update(bool is_voltage_on)
{
    if (_last_voltage_status == is_voltage_on)
        return;

    nos::log::info("KollmorgenServo: voltage_status_update {}", is_voltage_on);

    if (is_voltage_on == false)
    {
        transactionRefuseError();
    }
    else
    {
        transactionReady();
    }

    _last_voltage_status = is_voltage_on;
}

uint16_t KollmorgenServoControl::get_status_word()
{
    return (uint16_t)get_parameter("status", false);
}

uint16_t KollmorgenServoControl::get_control_word()
{
    return (uint16_t)get_parameter("control", false);
}

void KollmorgenServoControl::updaterThreadFunc()
{
    while (1)
    {
        try
        {
            auto status = get_status_word();
            auto voltage_enabled = is_voltage_enabled(status);

            // Проверяем, подано ли силовое напряжение на сервоусилитель
            voltage_status_update(voltage_enabled);
            if (!voltage_enabled)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            if (voltage_enabled && _is_automatic_servo_on_applied == false)
            {
                nos::log::info("KollmorgenServo: On preset switch on");
                servo_on();
                _is_automatic_servo_on_applied = true;
            }

            uint8_t l_operationStatus;

            if (prevent_updates)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            kupdmtx.lock();

            // if (updaterThreadFunc_local())
            //    goto end;

            l_operationStatus = m_operationStatus;

            if (alarm_simulation)
            {
                transactionAlarmError("simulation");
                goto end;
            }

            m_currentPosition = request_position();

            kupdmtx.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            kupdmtx.lock();

            m_operationStatus = request_operation_status();
            positionUpdated(m_currentPosition);

            hardware_limit_switches_update();

            if (torqueNotify.has_subscribers())
            {
                auto torque = current_torque();
                torqueNotify.notify(torque);
            }

            if ((m_operationStatus != l_operationStatus) ||
                m_operationStatusForce ||
                (std::chrono::system_clock::now() - last_opstatus_timestamp >
                 std::chrono::milliseconds(500)))
            {
                last_opstatus_timestamp = std::chrono::system_clock::now();
                operationStatusSignal(m_operationStatus);
                m_operationStatusForce = false;
            }
        }
        catch (const DeviceRefuseException &ex)
        {
            nos::log::error("KollmorgenServoControl::updaterThreadFunc: {}",
                            ex.what());
        }
        catch (const std::exception &ex)
        {
            nos::log::error("KollmorgenServoControl::updaterThreadFunc: {}",
                            ex.what());
        }
        catch (...)
        {
            nos::log::error(
                "KollmorgenServoControl::updaterThreadFunc: unknown exception");
        }

    end:
        kupdmtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
