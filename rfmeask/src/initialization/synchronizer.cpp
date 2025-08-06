
#include <Synchronizer.h>
#include <initialization/util.h>
#include <modes.h>
#include <nos/trent/json_settings.h>
#include <tables.h>

extern nos::json_settings config_settings;

std::unordered_map<std::string, std::unique_ptr<RS232Listener>>
    rs232_listener_for_port;

RS232Listener *get_rs232_listener(std::string com_port_name)
{
    com_port_name = nos::trim(com_port_name);
    if (com_port_name.empty())
    {
        throw std::invalid_argument("com_port_name is empty");
    }

    RS232Listener *rs232_listener = nullptr;
    if (rs232_listener_for_port.find(com_port_name) ==
        rs232_listener_for_port.end())
    {
        rs232_listener = new RS232Listener();
        rs232_listener_for_port[com_port_name].reset(rs232_listener);
        rs232_listener->open(com_port_name.c_str());

        rs232_listener->start_listener_thread();
    }
    else
    {
        rs232_listener = rs232_listener_for_port[com_port_name].get();
    }
    return rs232_listener;
}

void initialize_sync(const std::string &name, const nos::trent &dict)
{
    ::have_drv = true;

    int updateTime = dict["updateTime"].as_numer_default(50);
    int address = dict["address"].as_numer_except();

    std::string default_com =
        config_settings.node()["paths"]["drv"].as_string_except().c_str();
    std::string com_port_name =
        port_converter(dict["com"].as_string_default(default_com));

    auto com = get_rs232_listener(com_port_name);

    //Создаем и инициализируем объект */устройства.
    auto sync = new Synchronizer(name.c_str(), com, address);
    sync->updaterTimeout(updateTime);

    registry_device(sync, brocker);

    devices.insert(std::make_pair(name, sync));
    nos::log::info(
        "Registred Syncronizer with name {}, address {} and updateTime {}ms.",
        name,
        address,
        updateTime);
}
