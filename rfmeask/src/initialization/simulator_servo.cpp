#include <servos/SimulatorServo.h>
#include <tables.h>
#include <vector>

#include <comm/Notify.h>

extern NotifyBrocker brocker;

void simulator_servo_initializer(const std::string &name,
                                 const nos::trent &dict)
{
    SimulatorServo *dev = new SimulatorServo(name.c_str());
    registry_device(dev, brocker);
    devices.insert(std::make_pair(name, dev));
}

void init_simulator_servo()
{
    devices_types.push_back("simulator_servo");
    device_initializers.insert(
        std::make_pair("simulator_servo", simulator_servo_initializer));
}
