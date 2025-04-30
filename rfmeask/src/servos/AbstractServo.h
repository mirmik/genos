#ifndef ABSTRACT_SERVO_CONTROL
#define ABSTRACT_SERVO_CONTROL

/**
  @file
    Класс AbstractServo реализует интерфейс к абстрактному
  сервоусилителю. Здесь объявляются виртуальные методы управления СУ, а также
  имплементируются методы для работы с любыми типами СУ (ввод системы
  размерности, например).

    Класс так-же реализует логику опроса абстрактного сервоусилителя через
  функцию void updaterThreadFunc(), запускаемую в специальном потоке.
*/

#include <AbstractAxis.h>
#include <FullyClosedGear.h>
#include <GearSettings.h>
#include <chrono>
#include <devices/Device.h>
#include <igris/util/bug.h>
#include <interrupt.h>
#include <modes.h>
#include <mutex>
#include <optional>

extern std::recursive_mutex updmtx;
extern bool prevent_updates;

class Device;
class AbstractSyncro;
class NotifyTheme;

class AbstractServo : public Device
{
    using parent = Device;

public:
    enum LSwitchStatus
    {
        LS_NOPE = 0,
        LS_LEFT = 1,
        LS_RIGHT = 2,
        LS_TWIN = 10, //Выдается при срабатывании обоих концевиков(ошибка???).
    };

    enum OperationStatus
    {
        STOPED = 0,
        MOVED = 1,
    };

protected:
    AbstractAxis *volatile _attached_axis = nullptr;
    std::optional<GearSettings> _gear_settings = std::nullopt;
    std::optional<FullyClosedGear> _fully_closed_gear = std::nullopt;
    int32_t fully_closed_numer = 1;
    int32_t fully_closed_denum = 1;
    uint8_t m_faultStatus = 0;
    int64_t m_currentPosition = 0;
    uint8_t m_limitSwitchStatus = 0;
    bool m_operationStatusForce = false;
    uint8_t m_operationStatus = 0;
    bool m_haveAbsoluteMove = false;
    bool m_haveSetReference = false;
    NotifyTheme torqueNotify = {};
    NotifyTheme limitSwitchNotify = {};
    NotifySmoothTheme nonCorrectedUnitPositionNotify = {};
    NotifySmoothTheme unitPositionNotify = {};
    NotifySmoothTheme rawPositionNotify = {};
    std::chrono::system_clock::time_point last_opstatus_timestamp =
        std::chrono::system_clock::now();

    int64_t update_iteration = 0;
    bool limit_switch_update_needed = false;

    virtual std::recursive_mutex &bus_mutex() = 0;

public:
    bool encoderType_external = false;
    bool fully_closed_mode = false;
    std::chrono::system_clock::time_point last_transaction_time =
        std::chrono::system_clock::now();
    StateCode last_state = {};

    rxcpp::subjects::behavior<int64_t> _position_subject =
        rxcpp::subjects::behavior<int64_t>(0);
    rxcpp::subjects::behavior<double> _unit_position_subject =
        rxcpp::subjects::behavior<double>(0);

    igris::multiple_delegate<int32_t> positionUpdated = {};
    igris::multiple_delegate<double> positionUpdatedUnit = {};
    igris::multiple_delegate<uint8_t> operationStatusSignal = {};
    igris::multiple_delegate<uint8_t> limitSwitchStatusSignal = {};
    igris::multiple_delegate<int32_t> torqueUpdated = {};
    igris::multiple_delegate<std::vector<int32_t>> multiTorqueUpdated = {};

    nos::trent_binder<double> *_velocity_multiplier = nullptr;

#if USE_LAST_ALARM
    nos::trent_binder<int> _last_alarm_runtime_binder;
#endif

    AbstractServo(const AbstractServo &) = delete;
    AbstractServo &operator=(const AbstractServo &) = delete;

protected:
    bool _correction_enabled = false;
    ndmath::cartesian_correction one_axis_correction_table = {};

public:
    virtual bool use_pulse_position_notification()
    {
        return false;
    }

    void save_one_axis_correction_to_runtime();
    void load_one_axis_correction_from_runtime();
    nos::trent one_axis_correction_as_trent() const;

    void clear_one_axis_correction()
    {
        one_axis_correction_table = {};
        _correction_enabled = true; // default state is true
        save_one_axis_correction_to_runtime();
    }

    double inverse_unit_position_correct(double pos);
    double unit_position_correct(double pos);

    // double inverse_impulse_position_correct(double pos);
    // double impulse_position_correct(double pos);

    bool is_correction_enabled() const
    {
        return _correction_enabled;
    }

    const ndmath::cartesian_correction &get_one_axis_correction() const
    {
        return one_axis_correction_table;
    }

    void set_one_axis_correction(const ndmath::cartesian_correction &cor,
                                 bool correction_enabled)
    {
        one_axis_correction_table = cor;
        //_has_correction = true;
        _correction_enabled = correction_enabled;
        save_one_axis_correction_to_runtime();
    }

    virtual bool is_absolute_moving_recomended()
    {
        return false;
    }

    void enable_limit_switch_requests(bool en)
    {
        limit_switch_update_needed = en;
    }

    virtual bool has_limit_switch_forw()
    {
        return false;
    }

    virtual bool has_limit_switch_back()
    {
        return false;
    }

    auto unit_position_observable()
    {
        return _unit_position_subject.get_observable();
    }
    auto position_observable()
    {
        return _position_subject.get_observable();
    }

    const auto &currentPosition()
    {
        return m_currentPosition;
    }
    const auto &operationStatus()
    {
        return m_operationStatus;
    }
    const auto &limitSwitchStatus()
    {
        return m_limitSwitchStatus;
    }
    const auto &haveAbsoluteMove()
    {
        return m_haveAbsoluteMove;
    }
    const auto &haveSetReference()
    {
        return m_haveSetReference;
    }

    // Read control impulses multiplier.
    virtual double electronic_gear()
    {
        BUG();
    }

    virtual double feedback_electronic_gear()
    {
        BUG();
    }

    int getLimitSwitchStatus() override
    {
        return m_limitSwitchStatus;
    }

    bool alarm_simulation = false;
    void testAlarm() override
    {
        alarm_simulation = !alarm_simulation;
    }

    AbstractServo(const std::string &name);

    double last_position() override;
    double last_unit_position() override;

    double last_unit_position_invert_corrected_if_need()
    {
        double noncorrected = last_unit_position();
        if (_correction_enabled)
            return inverse_unit_position_correct(noncorrected);
        else
            return noncorrected;
    }

    virtual std::string alarm_status()
    {
        return "not implemented";
    }

    void updaterThreadFunc() override;
    virtual bool is_tandem()
    {
        return false;
    }

    virtual void setOrSyncElectronicGear(int gear)
    {
        FUNCTION_IS_NOT_RELEASED
    }

    virtual void setNullPosition()
    {
        FUNCTION_IS_NOT_RELEASED
    }
    virtual void setAccel_ms(uint32_t ms)
    {
        FUNCTION_IS_NOT_RELEASED
    }

    virtual void jog(uint8_t direction)
    {
        FUNCTION_IS_NOT_RELEASED
    }

#if USE_LAST_ALARM
    void save_last_alarm_code(int code);
#endif

    virtual void relative_move(double dist) = 0;
    virtual void absolute_move(double dist) = 0;
    // virtual void relative_unit_move(double dist) = 0;
    // virtual void absolute_unit_move(double dist) = 0;

    void notifiesRegistry(NotifyBrocker &brocker) override;

    virtual void
    interpolationRelativeMove(int32_t arg,
                              int32_t spd,
                              const std::vector<std::pair<int, int32_t>> &subax)
    {
        FUNCTION_IS_NOT_RELEASED;
    }
    virtual void
    interpolationAbsoluteMove(int32_t arg,
                              int32_t spd,
                              const std::vector<std::pair<int, int32_t>> &subax)
    {
        FUNCTION_IS_NOT_RELEASED;
    }

    virtual void setReference(int32_t reference)
    {
        FUNCTION_IS_NOT_RELEASED
    }

    virtual int32_t requestCurrent()
    {
        FUNCTION_IS_NOT_RELEASED
    }

    virtual int64_t request_position() = 0;

    virtual int64_t request_pulse_position_in_native_units()
    {
        return request_position();
    }

    virtual int64_t request_pulse_position_in_command_units()
    {
        return request_position();
    }

    virtual uint8_t request_operation_status() = 0;
    virtual uint8_t request_limit_switch_status()
    {
        return 0;
    }

    virtual uint32_t settedAccel_ms()
    {
        FUNCTION_IS_NOT_RELEASED
    }
    virtual void stop()
    {
        FUNCTION_IS_NOT_RELEASED
    }

    virtual bool hasInterpolation()
    {
        return false;
    }
    virtual bool canInterpolationWith(AbstractServo *oth)
    {
        return false;
    }

    virtual void presetTandem(){};

    virtual void servo_on(){FUNCTION_IS_NOT_RELEASED};

    virtual void servo_off(){FUNCTION_IS_NOT_RELEASED};

    virtual void optional_setRawUnitSpeed(double arg) {}
    virtual int updaterThreadFunc_local()
    {
        return 0;
    }

    enum class SpeedScale
    {
        RotationPerMinute,
        CommandUnitPerSecond,
        CommandUnitPerMinute,
    };

    double maxspd = 0;
    double maxspd_native = 0;

    virtual SpeedScale speedScale()
    {
        return SpeedScale::RotationPerMinute;
    }

    double protect_speed(double spd);
    virtual void setSpeed_native(double spd)
    {
        setSpeed_native_impl(protect_speed(spd));
    }
    virtual void setSpeed_native_impl(double spd) = 0;

    virtual double settedSpeed_native() = 0;
    virtual double settedSpeed_machine()
    {
        return settedSpeed_native();
    }

    // command_impulse - использует интерфейс верхнего уровня.
    //    для mitsubishi servo command_impulse = native_impulse / elgear.
    // native_impulse - реальные импульсы управления.
    //    для обычного режима равны drive_impulse.
    //    для fully_closed - равны fully_closed_impulse.
    // drive_impulse - импульсы встроенного датчика сервоусилителя.
    virtual double native_impulse_per_command_impulse() const = 0;
    virtual double drive_impulse_per_native_impulse() const = 0;
    virtual double drive_impulse_per_round() const = 0;

    double drive_impulse_per_command_impulse() const
    {
        return native_impulse_per_command_impulse() *
               drive_impulse_per_native_impulse();
    }

    double rot2drive() const
    {
        return drive_impulse_per_round();
    }

    double native_impulse_per_round() const
    {
        return drive_impulse_per_round() / drive_impulse_per_native_impulse();
    }

    double command_unit_per_round() const
    {
        return drive_impulse_per_round() / drive_impulse_per_native_impulse() /
               native_impulse_per_command_impulse();
    }

    double command_pulse_per_userunit();

    // Has fully closed system sensor
    virtual bool is_fully_closed_mode() const = 0;
    virtual double load_side_per_servo_side_multiplier() const
    {
        return 1;
    }

    double spdToRpmKoeff()
    {
        switch (speedScale())
        {
        case SpeedScale::RotationPerMinute:
            return 1.0;
        case SpeedScale::CommandUnitPerMinute:
            return 1.0 / command_unit_per_round();
        case SpeedScale::CommandUnitPerSecond:
            return 1.0 / command_unit_per_round() * 60.0;
        default:
            BUG();
        }
        return 0;
    }

    double spdToCupmKoeff()
    {
        switch (speedScale())
        {
        case SpeedScale::RotationPerMinute:
            return command_unit_per_round();
        case SpeedScale::CommandUnitPerMinute:
            return 1.0;
        case SpeedScale::CommandUnitPerSecond:
            return 1.0 * 60.0;
        default:
            BUG();
        }
        return 0;
    }

    double spdToCupsKoeff()
    {
        switch (speedScale())
        {
        case SpeedScale::RotationPerMinute:
            return command_unit_per_round() / 60.0;
        case SpeedScale::CommandUnitPerMinute:
            return 1.0 / 60.0;
        case SpeedScale::CommandUnitPerSecond:
            return 1.0;
        default:
            BUG();
        }
        return 0;
    }

    double rpmToSpdKoeff()
    {
        return 1.0 / spdToRpmKoeff();
    }
    double cupsToSpdKoeff()
    {
        return 1.0 / spdToCupsKoeff();
    }
    double cupmToSpdKoeff()
    {
        return 1.0 / spdToCupmKoeff();
    }

    void setSpeed_rpm(double arg)
    {
        setSpeed_native(arg * rpmToSpdKoeff() * velocity_multiplier());
    }

    void setSpeed_cups(double arg)
    {
        setSpeed_native(arg * cupsToSpdKoeff() * velocity_multiplier());
    }

    void setSpeed_cupm(double arg)
    {
        setSpeed_native(arg * cupmToSpdKoeff() * velocity_multiplier());
    }

    double settedSpeed_rpm()
    {
        return settedSpeed_native() * spdToRpmKoeff() / velocity_multiplier();
    }

    double settedSpeed_cups()
    {
        return settedSpeed_native() * spdToCupsKoeff() / velocity_multiplier();
    }

    double settedSpeed_cupm()
    {
        return settedSpeed_native() * spdToCupmKoeff() / velocity_multiplier();
    }

    virtual void bindSyncro(AbstractSyncro *syncro) {}

    void resetSpeed()
    {
        nos::println("resetSpeed");
        // What is it?
        setSpeed_native(settedSpeed_native());
    }

    virtual int updateParameters(const nos::trent &tr)
    {
        FUNCTION_IS_NOT_RELEASED
    }

    double velocity_multiplier();

    virtual std::string getAddressAsString()
    {
        return "undefined";
    }

    virtual int enc_numer()
    {
        return 50;
    }
    virtual int enc_denum()
    {
        return 1;
    }

    std::string info() override
    {
        return "AbstractServo::info";
    }

    virtual double get_input_gain()
    {
        return 1;
    }

    virtual void enable_impulse_control_mode(){FUNCTION_IS_NOT_RELEASED};

    const GearSettings &gear_settings() const;
    const FullyClosedGear &fully_closed_gear() const;
    void attach_axis(AbstractAxis *axis);

    double native_pulses_per_unit() const
    {
        if (is_fully_closed_mode())
        {
            return native_pulses_per_unit_by_fully_closed();
        }
        return native_pulses_per_unit_by_gear();
    }

    double native_pulses_per_unit_by_fully_closed() const
    {
        if (!is_fully_closed_mode())
        {
            return 0;
        }

        double load_pulse_per_unit =
            fully_closed_gear().load_pulse_per_userunit();
        if (load_pulse_per_unit == 0)
            throw std::runtime_error("load_pulse_per_unit == 0");

        return load_pulse_per_unit;
    }

    double native_pulses_per_unit_by_gear() const
    {
        if (gear_settings().has_setted_value())
        {
            return gear_settings().get_forced_fullcycle_value();
        }

        double rounds_per_unit = gear_settings().rounds_per_unit();
        double native_pulse_per_round = native_impulse_per_round();
        double native_pulse_per_unit = native_pulse_per_round * rounds_per_unit;
        return native_pulse_per_unit;
    }
};

#endif
