#include <RalgoCNC.h>
#include <comm/Notify.h>
#include <crow/address.h>
#include <ndmath/named_cartesian_correction.h>
#include <nos/trent/json.h>
#include <nos/trent/trent.h>
#include <string>
#include <tables.h>

extern NotifyBrocker brocker;

void ralgocnc_initializer(const std::string &name, const nos::trent &dict)
{
    RalgoCNC *dev = new RalgoCNC(name.c_str());

    {
        crow::hostaddr crowker_address =
            crow::address(dict["crowker_address"].as_string_except());
        std::string theme = dict["prefix"].as_string();
        dev->set_crowker_address(crowker_address, theme);
    }

    registry_device(dev, brocker);
    devices.insert(std::make_pair(name, dev));
}

void init_ralgocnc_device()
{
    devices_types.push_back("ralgocnc");
    device_initializers.insert(
        std::make_pair("ralgocnc", ralgocnc_initializer));
}
