#include <KollmorgenServo.h>
#include <tables.h>

void initialize_kollmorgen(const std::string &name, const nos::trent &dict)
{
    have_can = true;

    int updateTime = dict["updateTime"].as_numer_default(50);
    int address = dict["address"].as_numer_except();
    bool automatic_servo_on = dict["EnableServoOnPreset"].as_bool_default(true);

    auto servo = new KollmorgenServoControl(name.c_str(), address);
    servo->set_enable_servo_on_preset(automatic_servo_on);
    servo->updaterTimeout(updateTime);
    servo->init();

    registry_device(servo, brocker);
}

void init_kollmorgen_servo()
{
    devices_types.push_back("kollmorgen_servo");
    device_initializers.insert(
        std::make_pair("kollmorgen_servo", initialize_kollmorgen));
}
