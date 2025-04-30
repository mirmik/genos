#include <MitsubishiCommunicator.h>
#include <modes.h>
#include <servos/MRJ5Servo.h>
#include <servos/MitsubishiServo.h>
#include <tables.h>

// extern MitsubishiCommunicator mrs;
std::set<std::string> encoder_type_set = {"internal", "external"};
std::unordered_map<std::string, std::unique_ptr<MitsubishiCommunicator>>
    mitsu_communicator_for_port;

MitsubishiCommunicator *get_mitsuservo_communicator(std::string com_port_name)
{
    com_port_name = nos::trim(com_port_name);

    if (com_port_name.empty())
    {
        throw std::invalid_argument("com_port_name is empty");
    }

    MitsubishiCommunicator *mrs = nullptr;
    if (mitsu_communicator_for_port.find(com_port_name) ==
        mitsu_communicator_for_port.end())
    {
        mrs = new MitsubishiCommunicator(com_port_name.c_str());
        mitsu_communicator_for_port[com_port_name].reset(mrs);
    }
    else
    {
        mrs = mitsu_communicator_for_port[com_port_name].get();
    }
    return mrs;
}

void open_mitsuservo_communicator_ports()
{
    for (auto &pair : mitsu_communicator_for_port)
    {
        pair.second->open(pair.first.c_str());
    }
}

std::string port_converter(std::string port)
{
    if (port == "COM1")
    {
        return "/dev/ttyS0";
    }
    else if (port == "COM2")
    {
        return "/dev/ttyS1";
    }
    else if (port == "COM3")
    {
        return "/dev/ttyS2";
    }
    else if (port == "COM4")
    {
        return "/dev/ttyS3";
    }
    else
    {
        return port;
    }
    return port;
}

void initialize_mitsuservo(const std::string &name, const nos::trent &dict)
{
    ::have_mrs = true;

    int address = -1;
    int updateTime = 0;

    //Время опроса.
    address = dict["address"].as_numer_except();
    updateTime = dict["updateTime"].as_numer_default(50);

    std::string com_port_name =
        port_converter(dict["com"].as_string_default("/dev/ttyS3"));
    MitsubishiCommunicator *mrs = get_mitsuservo_communicator(com_port_name);

    //Создаем и инициализируем объект устройства.
    auto servo = new MitsubishiServo(name.c_str(), mrs, address);
    servo->updaterTimeout(updateTime);

    if (dict.contains("PA") &&
        dict["PA"][0]["value"].as_string_except() == "1010")
    {
        // TODO:
        // метод поиска мультипликатора на основе параметров не является
        // правильным. необходимо забирать этот множитель непосредственно с
        // сервоусилителя.
        servo->set_speed_multiplier(
            (double)atoi(dict["PE"][3]["value"].as_string_except().c_str()) /
            (double)atoi(dict["PE"][2]["value"].as_string_except().c_str()));
    }

    if (dict.contains("power_off_periodical_correction_period"))
    {
        if (dict["power_off_periodical_correction_period"].is_numer())
            servo->enable_power_off_position_keeping(
                dict["power_off_periodical_correction_period"].as_numer());
    }

    registry_device(servo, brocker);

    devices.insert(std::make_pair(name, servo));
    nos::log::info("Registred MitsubishiServo with name: {}, address: {} and "
                   "updateTime {} мс.",
                   servo->name(),
                   address,
                   updateTime);
}

void initialize_mitsuservo_j5(const std::string &name, const nos::trent &dict)
{
    ::have_mrs = true;

    int address = -1;
    int updateTime = 0;

    //Время опроса.
    address = dict["address"].as_numer_except();
    updateTime = dict["updateTime"].as_numer_default(50);

    std::string com_port_name =
        port_converter(dict["com"].as_string_default("/dev/tty3"));
    MitsubishiCommunicator *mrs = get_mitsuservo_communicator(com_port_name);

    //Создаем и инициализируем объект устройства.
    auto servo = new MRJ5Servo(name.c_str(), mrs, address);
    servo->updaterTimeout(updateTime);

    if (dict.contains("power_off_periodical_correction_period"))
    {
        if (dict["power_off_periodical_correction_period"].is_numer())
            servo->enable_power_off_position_keeping(
                dict["power_off_periodical_correction_period"].as_numer());
    }

    registry_device(servo, brocker);

    devices.insert(std::make_pair(name, servo));
    nos::log::info("Registred MitsubishiServo with name: {}, address: {} and "
                   "updateTime {} мс.",
                   servo->name(),
                   address,
                   updateTime);
}
