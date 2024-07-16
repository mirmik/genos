#include <modes.h>
#include <servos/MitsubishiServo.h>
#include <servos/MRJ5Servo.h>
#include <tables.h>

extern MitsubishiCommunicator mrs;
std::set<std::string> encoder_type_set = {"internal", "external"};

void initialize_mitsuservo(const std::string &name, const nos::trent &dict)
{
    ::have_mrs = true;

    int address = -1;
    int updateTime = 0;

    //Время опроса.
    address = dict["address"].as_numer_except();
    updateTime = dict["updateTime"].as_numer_default(50);

    //Создаем и инициализируем объект устройства.
    auto servo = new MitsubishiServo(name.c_str(), &mrs, address);
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

    //Создаем и инициализируем объект устройства.
    auto servo = new MRJ5Servo(name.c_str(), &mrs, address);
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
