#include <SimulatorSyncro.h>
#include <tables.h>
#include <vector>

extern NotifyBrocker brocker;

void simulator_syncro_initializer(const std::string &name,
                                  const nos::trent &dict)
{
    SimulatorSyncro *dev = new SimulatorSyncro(name.c_str());

    registry_device(dev, brocker);
    devices.insert(std::make_pair(name, dev));
    dev->init();
}

void init_simulator_syncro()
{
    devices_types.push_back("simulator_syncro");
    device_initializers.insert(
        std::make_pair("simulator_syncro", simulator_syncro_initializer));
}
