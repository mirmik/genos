/**
    @file
    Драйвер сервоусилителей Митсубисши. Для управления используется канал RS485.
*/

#ifndef SIMULATOR_SERVO_CONTROL
#define SIMULATOR_SERVO_CONTROL

#include <ralgo/heimer/axis_state.h>
#include <ralgo/heimer/axis_stub_processor.h>
#include <ralgo/heimer/axisctr.h>
#include <ralgo/trajectory/LineTrajectory.h>
#include <servos/AbstractServo.h>

class SimulatorServo : public AbstractServo
{
    using parent = AbstractServo;
    std::string node_name;
    // heimer::axis_state simulator_state;
    // heimer::axis_controller simulator;
    // heimer::axis_stub_processor stub_processor;

    double m_speed = 0;
    uint32_t m_accel_ms = 50000;
    double gear = 1;

    PolymorphSpeed speed =
        PolymorphSpeed(PositionUnit::EXTERNAL, TimeUnit::SECONDS, 1);
    ralgo::LinePositionFunction<int64_t, double> position_function;

public:
    bool is_absolute_moving_recomended() override
    {
        return true;
    }

    SimulatorServo(const std::string &name) : AbstractServo(name), node_name()
    {
    }
    // simulator_state(name + ":state"),
    // simulator(name + ":sim", {&simulator_state}),
    // stub_processor(name + ":term", {&simulator_state}){}

    void enable_impulse_control_mode() override
    {
        // pass
    }

    void absolute_unit_move(double dist) override;

    std::string declared_type() const override
    {
        return "simulator_servo";
    }

    double electronic_gear() override
    {
        return 1000;
    }
    void preset() override;

    void setSpeed_native_impl(double spd) override;
    void setAccel_ms(uint32_t acc) override;
    double settedSpeed_native() override;

    uint32_t settedAccel_ms() override
    {
        return m_accel_ms;
    }

    void errstatAnalyze(uint8_t &errstat, uint8_t critical);
    double current_torque() override;

    static constexpr int c_impulsePerRotation = 4194304;

    void relative_move(double dist) override;
    void absolute_move(double dist) override;

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
        return c_impulsePerRotation;
    }

    int64_t request_position() override;
    uint8_t request_operation_status() override;

    void updaterThreadFunc() override;
    std::string getAddressAsString() override
    {
        return "simulator";
    }

    void stop() override;

    std::pair<std::string, std::string>
    get_parameter(const std::string &mnemo) override
    {
        return {"0", "0"};
    }

    void set_parameter(const std::string &mnemo,
                       const std::string &value,
                       const std::string &format) override
    {
        // pass
    }

    bool is_fully_closed_mode() const override
    {
        return false;
    }
};

#endif
