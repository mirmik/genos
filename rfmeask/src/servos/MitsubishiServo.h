#ifndef MITSUBISHI_SERVO_CONTROL
#define MITSUBISHI_SERVO_CONTROL

/**
    @file
    Драйвер сервоусилителей Митсубисши. Для управления используется канал RS485.
*/

#include <PowerOffPositionKeeper.h>
#include <optional>
#include <ralgo/oldheimer/axisctr.h>
#include <ralgo/oldheimer/stub_axis.h>
#include <servos/AbstractServo.h>
#include <string>
#include <unordered_map>

#define NONCRITICAL 0
#define CRITICAL 1

class MitsubishiCommunicator;

class MitsubishiServo : public AbstractServo
{
    enum Mode
    {
        MODE_MANUAL,
        MODE_JOG,
        MODE_POS,
    };

    enum Scan
    {
        SCAN_TRUE,
        SCAN_FALSE,
    };

    std::mutex internal_mutex;
    uint8_t m_rs485addr = 0;
    MitsubishiCommunicator *m_mrs = nullptr;
    Mode m_mode = MODE_MANUAL;
    double m_speed = 100;
    uint32_t m_accel_ms = 3000;
    double _elgear = 1;
    double _feedback_elgear = 1;
    bool _has_load_side_encoder = false;
    double _load_side_multiplier = 1; // < load_side / servo_side
    double speed_multiplier = 1;
    static constexpr int c_drive_pulse_per_rotation = 4194304; //< 2**22
    bool _has_physical_limit_switches = false;
    std::optional<PowerOffPositionKeeper> power_off_position_keeper = {};
    bool power_off_keeper_enabled = false;
    double power_off_keeper_period = 0;

public:
    void set_speed_multiplier(double _speed_mul)
    {
        speed_multiplier = _speed_mul;
    }

    bool has_physical_limit_switches()
    {
        return _has_physical_limit_switches;
    }

    bool has_limit_switch_back() override
    {
        return _has_physical_limit_switches;
    }

    bool has_limit_switch_forw() override
    {
        return _has_physical_limit_switches;
    }

    std::string declared_type() const override
    {
        return "mitsuservo";
    }

    std::recursive_mutex &bus_mutex() override;

    void enable_power_off_position_keeping(int64_t period,
                                           const std::string &name);

    void enable_impulse_control_mode() override;

    void enable_power_off_position_keeping(int64_t period);

    void set_zero_position() override
    {
        setNullPosition();
    }

    double get_elgear()
    {
        return _elgear;
    }

    void set_elgear(double elgear)
    {
        _elgear = elgear;
    }

    int rs485addr()
    {
        return m_rs485addr;
    }

    MitsubishiServo(const char *name,
                    MitsubishiCommunicator *mrs,
                    uint8_t rs485addr);
    MitsubishiServo(const MitsubishiServo &) = delete;
    MitsubishiServo &operator=(const MitsubishiServo &) = delete;

    double electronic_gear() override;
    double feedback_electronic_gear() override;
    void errstatAnalyze(uint8_t &errstat, uint8_t critical);
    bool is_fully_closed_mode() const override;
    double load_side_per_servo_side_multiplier() const override
    {
        return _load_side_multiplier;
    }

    void setNullPosition() override;
    void servo_on() override;
    void servo_off() override;
    void setReference(int32_t dist) override;

    void relative_move(double imp) override;
    void absolute_move(double dist) override;

    void relative_unit_move(double dist) override;
    void absolute_unit_move(double dist) override;

    bool has_physical_limit_switches_request();

    // void prepareToMoveOperation();

    int64_t request_pulse_position_in_command_units_without_keeping();
    int64_t request_pulse_position_in_servo_side_drive_units();
    int64_t request_pulse_position_in_load_side_units();

    // native unit - это drive pulse для штатного режима работы сервоусилителя
    // и load side pulse для режима работы с абсолютным датчиком
    int64_t request_pulse_position_in_native_units() override;

    int64_t request_pulse_position_in_command_units() override;
    int64_t request_position() override;

    uint8_t request_limit_switch_status() override;
    uint8_t request_operation_status() override;

    void setSpeed_native_impl(double spd) override;
    void setAccel_ms(uint32_t ms) override;
    double settedSpeed_native() override;
    uint32_t settedAccel_ms() override;

    void preset() override;
    // void init() override;

    void stop() override;
    int getAlarmStatus() override;
    std::string getAlarm() override;

    std::pair<bool, double> read_load_gain_settings();

    void jog(uint8_t direction) override;

    void on_first_connection() override;

    uint32_t get_gen_numer();
    uint32_t get_gen_denum();
    uint32_t get_enc_numer();
    uint32_t get_enc_denum();
    double current_torque() override;
    double native_impulse_per_command_impulse() const override;
    double drive_impulse_per_native_impulse() const override;
    double drive_impulse_per_round() const override;
    double userunit_to_command_pulse(double userunit);

    int updateParameters(const nos::trent &tr) override;

    std::string getAddressAsString() override
    {
        return std::to_string(m_rs485addr);
    }

    void bindSyncro(AbstractSyncro *) override;

    std::pair<std::string, std::string>
    get_parameter(const std::string &mnemo) override;
    void set_parameter(const std::string &mnemo,
                       const std::string &value,
                       const std::string &format) override;

    void command(const nos::argv &args, nos::ostream &os) override;
    std::string command_help() override;
    std::string info();

    bool is_absolute_moving_recomended() override
    {
        return true;
    }

    bool use_pulse_position_notification() override
    {
        return true;
    }
};

#endif
