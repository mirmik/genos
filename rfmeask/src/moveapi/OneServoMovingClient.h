#ifndef ONE_SERVO_CLIENT_H
#define ONE_SERVO_MOVING_CLIENT_H

#include <FullyClosedGear.h>
#include <GearSettings.h>
#include <moveapi/MoveApiClient.h>
#include <RalgoCNC.h>
#include <ralgo/cnc/util.h>
#include <servos/MitsubishiServo.h>
#include <nos/fprint.h>

using namespace nos::argument_literal;

class CommonAxis;
class OneServoMovingClient : public MoveApiClient 
{
    AbstractServo * _servo = nullptr;
    GearSettings _gear_settings = {};

public:
    FullyClosedGear _fully_closed_gear = {};
    //int _last_opcode = 0;

public:
    GearSettings * gear_settings() override { return &_gear_settings; }
    FullyClosedGear *fully_closed_gear() override { return &_fully_closed_gear; }
    void set_servo(AbstractServo * servo) { _servo = servo; }
    std::vector<Device*> get_devices() override { return {_servo}; }
    
    void stop() override;
    void nullify_position () override { _servo->set_zero_position(); }

    bool is_ready() override { return _servo->is_ready(); }
    bool is_unready() override { return _servo->is_unready(); }

    void notifyCurrentPosition(int32_t arg);


    double get_position_iu() override { return _servo->currentPosition(); }

    //void init(CommonAxis *parent) override {};

    int unsafe_absolute_move_iu(double, PolymorphSpeed, PolymorphAccel) override;
    int unsafe_relative_move_iu(double, PolymorphSpeed, PolymorphAccel) override;
    int unsafe_absolute_move_units(double dist, PolymorphSpeed, PolymorphAccel) override;

    ~OneServoMovingClient() override = default;

    void preset();
    void init(CommonAxis *parent) override;
    void init_scale_multipliers();
    double native_pulses_per_unit() const;
    double native_pulses_per_unit_by_fully_closed() const;
    double native_pulses_per_unit_by_gear() const;
void operation_status_signal(uint8_t) ;

protected:
    bool command(const nos::argv &args, nos::ostream &os) override { return false; }
    std::string info() override 
    { 
        std::string ret = MoveApiClient::info();
        ret += nos::format(
            "servo: {servo}\r\n"
            "servo_epos: {servoepos}\r\n"
            "native_impulse_per_command_impulse: {native_impulse_per_command_impulse}\r\n"
            "drive_impulse_per_native_impulse: {drive_impulse_per_native_impulse}\r\n"
            "drive_impulse_per_round: {drive_impulse_per_round}\r\n",
            "servo"_a = _servo->name(),
            "servoepos"_a = _servo->currentPosition(),
            "native_impulse_per_command_impulse"_a = _servo->native_impulse_per_command_impulse(),
            "drive_impulse_per_native_impulse"_a = _servo->drive_impulse_per_native_impulse(),
            "drive_impulse_per_round"_a = _servo->drive_impulse_per_round());

        return ret;
    }
    std::string command_help() override { return {};}
};

#endif