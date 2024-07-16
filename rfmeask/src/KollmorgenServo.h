#ifndef KOLLMORGEN_SERVO_CONTROL
#define KOLLMORGEN_SERVO_CONTROL

/*
    Класс драйвера сервопривод Kollmorgen
*/

#include <cankollm/cankollm.h>
#include <cankollm/canopen.h>
#include <cankollm/kollmmap.h>
#include <fcntl.h>
#include <servos/AbstractServo.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

extern CanKollm cankollm;

class KollmorgenServoControl : public AbstractServo
{

    enum Mode
    {
        MODE_MANUAL,
        MODE_JOG,
        MODE_POS
    };

    const uint8_t m_stantion = 0;
    nos::log::logger logger = nos::log::logger("KollmorgenServoControl")
                                  .set_level(nos::log::level::Info);
    uint32_t m_speed_rpm = 0;
    uint32_t m_accel_ms = 0;
    int32_t savedPosition = 0;
    int savepos_fd = -1;
    int32_t speedToSet(int32_t spd);
    int last_status = 520;
    std::mutex kupdmtx;

    bool _last_voltage_status = false;
    bool _last_forward_hardware_limit = false;
    bool _last_backward_hardware_limit = false;

    // Состояние по умолчанию true, чтобы тормоз не отщёлкивался до preset.
    bool _is_automatic_servo_on_applied = true;

    // Флаг активирует сброс флага _is_automatic_servo_on_applied в функции
    // preset.
    bool _enable_servo_on_preset = true;

    bool inited = false;
    bool presetted = false;

public:
    KollmorgenServoControl(const char *name, uint8_t stantion);

    std::string declared_type() const override
    {
        return "kollmorgen_servo";
    }

    void set_enable_servo_on_preset(bool enable)
    {
        _enable_servo_on_preset = enable;
    }

    /*bool is_ready_to_switched_on(int status)
    {
        return status & (1 << 0);
    }

    bool is_switch_on_disabled(int status)
    {
        return status & (1 << 6);
    }

    bool is_switched_on(int status)
    {
        return status & (1 << 1);
    }*/

    bool is_voltage_enabled(int status)
    {
        return status & (1 << 4);
    }

    void hardware_limit_switches_update();

    void voltage_status_update(bool is_voltage_on);

    void errstatAnalyze(uint8_t &errstat);
    void setNullPosition() override;
    void relative_move(double dist) override;
    void absolute_move(double dist) override;
    int64_t request_position() override;
    uint8_t request_operation_status() override;
    void setReference(int32_t dist) override;
    void setSpeed_native(double spd) override
    {
        setSpeed_rpm(spd);
    }
    void setSpeed_rpm(uint32_t spd);
    void setSpeed_native_impl(double spd) override;
    void setAccel_ms(uint32_t ms) override;
    void preset() override;
    double settedSpeed_native() override
    {
        return settedSpeed_rpm();
    }
    uint32_t settedSpeed_rpm();
    uint32_t settedAccel_ms() override;
    void set_parameter(const char *str, int val);
    int get_parameter(const char *str, bool check_error = true);
    void setMode(const char *str);
    void servo_on() override;
    void servo_off() override;
    void stop() override;
    void init();
    int getAlarmStatus() override;
    double native_impulse_per_command_impulse() const override
    {
        return 1;
    }
    double drive_impulse_per_native_impulse() const override
    {
        return 1;
    }
    double drive_impulse_per_round() const override
    {
        return 4194304;
    }

    void updaterThreadFunc() override;

    uint16_t get_status_word();
    uint16_t get_control_word();

    bool is_fully_closed_mode() const override
    {
        return false;
    }
};

#endif
