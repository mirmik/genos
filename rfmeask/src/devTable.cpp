/**
    @file
    Здесь заполняется информация о текущей конфигурации системы.
*/

#include <AbstractAxis.h>
#include <Axis.h>
#include <AxisSynchro.h>
#include <GearSettings.h>
#include <Synchronizer.h>
#include <config.h>
#include <devices/Device.h>
#include <groups/MitsubishiInterpolationGroup.h>
#include <idn.h>
#include <modes.h>
#include <nos/trace.h>
#include <servos/MitsubishiServo.h>
#include <tables.h>
#include <version.h>

extern MitsubishiCommunicator mrs;

#if MODBUS_ENABLED
#include <MitsuModbusServo.h>
extern ModbusCommunicator modbus;
#endif

extern RS232Listener drv;
extern NotifyBrocker brocker;

const char *api_version()
{
    return API_VERSION;
}

std::map<std::string, void (*)(const std::string &name, const nos::trent &dict)>
    device_initializers;

std::map<std::string, void (*)(const std::string &name, const nos::trent &dict)>
    axis_initializers;

std::map<std::string, void (*)(const std::string &name, const nos::trent &dict)>
    intgroups_initializers;

void initialize_mitsuservo(const std::string &name,
                           const nos::trent &dict,
                           const nos::trent &settings);
void initialize_mitsuservo_j5(const std::string &name, const nos::trent &dict);
void initialize_sync(const std::string &name, const nos::trent &dict);
void initialize_mitsuservo_board(const std::string &name,
                                 const nos::trent &dict);
void initialize_mitsuservo_board_tandem(const std::string &name,
                                        const nos::trent &dict);
void init_interpolation_groups();
void init_interpolation_groups_v2();
void initialize_axis(const nos::trent &p);
void initialize_sync_axis(const nos::trent &p);

std::set<std::string> axis_types{
    "simple",
    "sync_axis",
};

std::set<std::string> servo_types{

    "mitsuservo",
    "mitsuservo_type_A",
    "mitsuservo_A_modbus",
    "mitsuservo_board",
    "mitsuservo_type_B",
    "simulator_servo",
    "mitsuservo_board_tandem"};

std::set<std::string> servo_names;
std::vector<std::string> sync_types{"sync", "simulator_syncro"};
std::set<std::string> loglevel_set = {"debug", "trace", "info"};

std::list<std::string> optional_axis_opts{"reverse"};

std::vector<std::string> devices_types;

void initialize_devices(const nos::trent &devices_config,
                        const nos::trent &config)
{
    nos::log::info("Devices initialization");
    for (auto &p : devices_config.as_list_except())
    {
        nos::log::info("Device. Record: {}", p["name"].as_string_except());

        if (!p.is_dict())
        {
            nos::log::fault("Devices options has to be dictionary");
            exit(-1);
        }

        std::string type = p["type"].as_string_except();
        std::string name = p["name"].as_string_except();
        //Дальнейшая инициализации выполняется в зависимости от объявленного
        //типа.
        if (type == "mitsuservo" || type == "mitsuservo_type_A")
            initialize_mitsuservo(name, p, config);
        else if (type == "mitsuservo-MR_J5_A")
            initialize_mitsuservo_j5(name, p);
        else if (type == "sync")
            initialize_sync(name, p);
        else if (device_initializers.count(type))
        {
            device_initializers[type](name, p);
        }
        else
        {
            nos::log::fault("device: {0}. devtype {1} isn't found.",
                            p["name"].as_string_except(),
                            type);
            nos::log::fault("Available types:");
            for (auto &tp : devices_types)
                nos::log::fault(tp.c_str());
            exit(-1);
        }
    }
}

void initialize_axes(const nos::trent &axes_config)
{
    nos::log::info("Axes initialization");
    for (auto &p : axes_config.as_list_except())
    {
        std::string name = p["name"].as_string_default("unnamed");
        nos::log::info("Axis. Record: {}", name);

        std::string type = p["type"].as_string();
        if (type == "simple")
        {
            initialize_axis(p);
        }
        else if (type == "sync_axis")
        {
            initialize_sync_axis(p);
        }
        else if (axis_initializers.count(type))
        {
            axis_initializers[type](name, p);
        }
        else
        {
            nos::log::fault("Not found axis type: name:{} type:{}",
                            p["name"].as_string(),
                            p["type"].as_string());
            exit(-1);
        }
    }
}

void initialize_idn(const nos::trent &p)
{
    if (p.is_list())
    {
        std::array<std::string, 4> idn;
        for (int i = 0; i < 4; i++)
            idn[i] = p[i].as_string().c_str();
        idn[3] = api_version();
        set_idn_from_array(idn);
    }
    else if (p.is_string())
    {
        set_idn_from_string(p.as_string());
    }
    else
    {
        set_idn_from_array({"BAD", "IDN", "FORMAT", api_version()});
    }
}

void initialize_the_system_according_to_the_configuration()
{
    nos::log::info("System initialization");

    //Создаём список возможных типов устройств.
    std::merge(servo_types.begin(),
               servo_types.end(),
               sync_types.begin(),
               sync_types.end(),
               std::back_inserter(devices_types));

    initialize_idn(config_settings.node()["idn"]);
    initialize_devices(config_settings.node()["devices"],
                       config_settings.node());
    initialize_axes(config_settings.node()["axes"]);

    if (!config_settings.node().contains("intgroups"))
    {
        nos::println("WARNING: intgroups list not found");
    }
    else
    {
        init_interpolation_groups_v2();
    }
}
