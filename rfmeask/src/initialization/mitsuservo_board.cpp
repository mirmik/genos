#include <tables.h>

#if (HAVE_MITSUBISHI_BOARD)

#include <MitsubishiBoard.h>
#include <MitsubishiBoardServo.h>
#include <MitsubishiTandem.h>

extern MitsubishiBoard mboard;

void initialize_mitsuservo_board(const std::string &name,
                                 const nos::trent &dict)
{
    int address;
    int channel;
    int updateTime;

    have_mitsu_board_servos = true;

    address = dict["address"].as_numer_except();
    channel = dict["channel"].as_numer_except();
    int igroup = dict["igroup"].as_numer_default(0);
    updateTime = dict["updateTime"].as_numer_default(50);

    //Создаем и инициализируем объект устройства.
    auto servo =
        new MitsubishiBoardServo(name.c_str(), &mboard, channel, address);
    servo->adjustment = MitsuAdjustment(dict["adjustment"]);
    if (!dict["fully_closed"].is_nil())
    {
        servo->fully_closed_mode =
            dict["fully_closed"]["enable"].as_numer_critical();
        servo->fully_closed_numer =
            dict["fully_closed"]["numer"].as_numer_critical();
        servo->fully_closed_denum =
            dict["fully_closed"]["denum"].as_numer_critical();
        servo->fully_closed_reverse.push_back(
            dict["fully_closed"]["reverse"].as_list()[0].as_numer());
    }
    else
    {
        servo->fully_closed_mode = false;
    }

    servo->updaterTimeout(updateTime);
    servo->declared_type = "mitsuservo_board";
    servo->igroup = igroup;
    servo->electronicGear_numer =
        dict["electronicGear"]["numer"].as_numer_default(1);
    servo->electronicGear_denum =
        dict["electronicGear"]["denum"].as_numer_default(1);

    if (servo->fully_closed_mode)
    {
        servo->electronicGear_numer = 1;
        servo->electronicGear_denum = 1;
    }

    servo->torqLimitForw = dict["torqLimitForw"].as_numer_except();
    servo->torqLimitBack = dict["torqLimitBack"].as_numer_except();
    servo->init();

    nos::log::info("Try register");
    registry_device(servo, brocker);

    devices.insert(std::make_pair(name, servo));
    nos::log::info("Registered MitsubishiServo with name {}, address {} and "
                   "updateTime {}ms.",
                   name,
                   address,
                   updateTime);
}

void initialize_mitsuservo_board_tandem(const std::string &name,
                                        nos::trent &dict)
{

    have_mitsu_board_servos = true;

    nos::log::debug("check options dict");

    auto &nodes = dict["servos"].as_list_except();
    auto &nreverse = dict["native_reverse"].as_list_except();

    std::vector<int> servos;
    for (auto &n : nodes)
    {
        servos.emplace_back(n.as_numer());
    }

    std::vector<int> reverse;
    for (auto &n : nreverse)
    {
        reverse.emplace_back(n.as_numer());
    }

    MitsuAdjustment adjustment(dict["adjustment"]);

    int group = dict["group"].as_numer_critical();
    int igroup = dict["igroup"].as_numer_default(0);
    int channel = dict["channel"].as_numer_default(1);
    int updateTime = dict["updateTime"].as_numer_default(50);

    auto servo = new MitsubishiTandem(
        name.c_str(), &mboard, channel, servos, reverse, group);
    servo->adjustment = adjustment;
    servo->updaterTimeout(updateTime);
    if (!dict["fully_closed"].is_nil())
    {
        servo->fully_closed_mode =
            dict["fully_closed"]["enable"].as_numer_critical();
        servo->fully_closed_numer =
            dict["fully_closed"]["numer"].as_numer_critical();
        servo->fully_closed_denum =
            dict["fully_closed"]["denum"].as_numer_critical();
        for (auto &t : dict["fully_closed"]["reverse"].as_list())
        {
            servo->fully_closed_reverse.push_back(t.as_numer());
        }
    }
    else
        servo->fully_closed_mode = false;
    servo->declared_type = "mitsuservo_board_tandem";
    servo->igroup = igroup;
    servo->electronicGear_numer =
        dict["electronicGear"]["numer"].as_numer_default(1);
    servo->electronicGear_denum =
        dict["electronicGear"]["denum"].as_numer_default(1);

    if (servo->fully_closed_mode)
    {
        servo->electronicGear_numer = 1;
        servo->electronicGear_denum = 1;
    }

    servo->torqLimitForw = dict["torqLimitForw"].as_numer_except();
    servo->torqLimitBack = dict["torqLimitBack"].as_numer_except();
    servo->init();

    registry_device(servo, brocker);
    devices.insert(std::make_pair(name, servo));
    nos::log::info(
        "Registred MitsubishiTandem with name {}, and updateTime {} ms.",
        name,
        updateTime);
}
#else  // HAVE_MITSUBISHI_BOARD
void initialize_mitsuservo_board(const std::string &name,
                                 const nos::trent &dict)
{

    BUG();
}

void initialize_mitsuservo_board_tandem(const std::string &name,
                                        const nos::trent &dict)
{

    BUG();
}
#endif // HAVE_MITSUBISHI_BOARD
