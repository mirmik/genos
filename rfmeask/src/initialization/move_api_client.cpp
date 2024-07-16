#include <moveapi/CNCMovingClient.h>
#include <moveapi/OneServoMovingClient.h>
#include <initialization/move_api_client.h>
#include <logging.h>

void parse_default_and_maximum_speed(auto &client, auto &dict)
{
    if (dict.contains("defaultSpeed_rpm"))
    {
        auto vel = dict.get_as_numer_ex("defaultSpeed_rpm");
        assert(vel != 0);
        client->set_default_speed_rpm(vel);
    }
    else if (dict.contains("defaultSpeed_ups"))
    {
        auto vel = dict.get_as_numer_ex("defaultSpeed_ups");
        assert(vel != 0);
        client->set_default_speed_ups(vel);
    }
    else
    {
        BUG();
    }

    if (dict.contains("maxSpeed_rpm"))
    {
        auto vel = dict.get_as_numer_ex("maxSpeed_rpm");
        assert(vel != 0);
        assert(vel != 0);
        client->set_maximum_speed_rpm(vel);
    }
    else if (dict.contains("maxSpeed_ups"))
    {
        auto vel = dict.get_as_numer_ex("maxSpeed_ups");
        assert(vel != 0);
        client->set_maximum_speed_ups(vel);
    }
    else
    {
        BUG();
    }
}

void parse_default_and_maximum_accel(auto &client, auto &dict)
{
    if (dict.contains("defaultAccelTime_ms"))
    {
        auto acc_ms = dict.get_as_numer_ex("defaultAccelTime_ms");
        assert(acc_ms != 0);
        client->set_default_accel_ms(acc_ms);
    }
    else if (dict.contains("defaultAccel_rpmm"))
    {
        auto acc_rpmm = dict.get_as_numer_ex("defaultAccel_rpmm");
        assert(acc_rpmm != 0);
        client->set_default_accel_rpmm(acc_rpmm);
    }
    else if (dict.contains("defaultAccel_upss"))
    {
        auto acc_upss = dict.get_as_numer_ex("defaultAccel_upss");
        assert(acc_upss != 0);
        client->set_default_accel_upss(acc_upss);
    }
    else
    {
        BUG();
    }

    if (dict.contains("minAccelTime_ms"))
    {
        auto acc_ms = dict.get_as_numer_ex("minAccelTime_ms");
        assert(acc_ms != 0);
        client->set_maximum_accel_ms(acc_ms);
    }
    else if (dict.contains("maxAccel_rpmm"))
    {
        auto acc_rpmm = dict.get_as_numer_ex("maxAccel_rpmm");
        assert(acc_rpmm != 0);
        client->set_maximum_accel_rpmm(acc_rpmm);
    }
    else if (dict.contains("maxAccel_upss"))
    {
        auto acc_upss = dict.get_as_numer_ex("maxAccel_upss");
        assert(acc_upss != 0);
        client->set_maximum_accel_upss(acc_upss);
    }
    else
    {
        BUG();
    }
}

void parse_subaxes(auto &client, auto &dict, const std::string &name)
{
    std::vector<AbstractServo *> servos;
    std::vector<std::string> cnc_symbols;
    std::vector<double> multipliers;

    if (dict.contains("servos") && dict.contains("cnc_symbols"))
    {
        // Вариант для обратной совместимости.
        for (auto &tr : dict["servos"].as_list())
        {
            AbstractServo *servo = dynamic_cast<AbstractServo *>(
                get_device(tr.as_string_except()));
            servos.push_back(servo);
        }

        for (auto &tr : dict["cnc_symbols"].as_list())
        {
            cnc_symbols.push_back(tr.as_string_except());
        }

        multipliers.resize(servos.size(), 1);
    }
    else
    {
        // Текущий вариант.
        auto &subaxes = dict["subaxes"].as_list();
        for (auto &subax : subaxes)
        {
            AbstractServo *servo = dynamic_cast<AbstractServo *>(
                get_device(subax["servo_name"].as_string_except()));
            if (servo == nullptr)
                BUG();
            servos.push_back(servo);
            cnc_symbols.push_back(subax["symbol"].as_string());
            if (subax.contains("multiplier"))
                multipliers.push_back(subax["multiplier"].as_numer());
            else
                multipliers.push_back(1);
        }
    }

    if (servos.size() != cnc_symbols.size())
    {
        nos::log::fault("CNCMovingClient {} : vector sizes is not same.", name);
    }

    for (unsigned int i = 0; i < servos.size(); ++i)
        client->add_axis_record({servos[i], cnc_symbols[i], multipliers[i]});
}

CNCTandemMovingClient *create_cnc_tandem_move_client(const nos::trent &dict,
                                                     const nos::trent &axdict,
                                                     const std::string &name)
{
    auto client = new CNCTandemMovingClient(nullptr); // ось биндится позднее

    parse_subaxes(client, dict, name);
    parse_default_and_maximum_speed(client, dict);
    parse_default_and_maximum_accel(client, dict);

    client->gear_settings()->parse_trent(dict["gear"]);

    std::string motionType = axdict["motionType"].as_string_except();
    client->gear_settings()->set_is_linear(motionType == "linear");

    RalgoCNC *driver =
        dynamic_cast<RalgoCNC *>(get_device(dict["cnc_driver"].as_string()));
    if (!driver)
    {
        nos::log::warn("CNCMovingClient {} : driver is not found.", name);
        return nullptr;
    }

    client->set_cnc_driver(driver);

    if (dict.contains("use_cnc_feedback"))
        client->set_use_cnc_feedback(dict["use_cnc_feedback"].as_bool());

    if (dict.contains("update_position_before_moving"))
        client->set_is_position_updated_before_moving(
            dict["update_position_before_moving"].as_bool());

    if (dict.contains("tandemSyncLimit"))
        client->set_tandem_synchronization_limit(
            dict["tandemSyncLimit"].as_numer());

    // In different api versions option fully closed sensor can be placed
    // in root dict or in root[gear]
    if (dict.contains("fully_closed_sensor"))
    {
        FullyClosedGear settings;
        settings.set_configuration_by_name(
            dict["fully_closed_sensor"].as_string());
        client->_fully_closed_gear = settings;
    }
    if (dict["gear"].contains("fully_closed_sensor"))
    {
        FullyClosedGear settings;
        settings.set_configuration_by_name(
            dict["gear"]["fully_closed_sensor"].as_string());
        client->_fully_closed_gear = settings;
    }

    return client;
}


OneServoMovingClient *create_one_servo_move_client(const nos::trent &dict,
                                                     const nos::trent &axdict,
                                                     const std::string &name)
{
    auto client = new OneServoMovingClient();
    parse_default_and_maximum_speed(client, dict);
    parse_default_and_maximum_accel(client, dict);

    client->gear_settings()->parse_trent(dict["gear"]);
    std::string motionType = axdict["motionType"].as_string_except();
    client->gear_settings()->set_is_linear(motionType == "linear");

    AbstractServo *servo = dynamic_cast<AbstractServo *>(
        get_device(dict["servo"].as_string()));
    client->set_servo(servo);

    if (dict.contains("fully_closed_sensor"))
    {
        FullyClosedGear settings;
        settings.set_configuration_by_name(
            dict["fully_closed_sensor"].as_string());
        client->_fully_closed_gear = settings;
    }
    if (dict["gear"].contains("fully_closed_sensor"))
    {
        FullyClosedGear settings;
        settings.set_configuration_by_name(
            dict["gear"]["fully_closed_sensor"].as_string());
        client->_fully_closed_gear = settings;
    }
    
    return client;
}

MoveApiClient *create_move_api_client_from_dict(const nos::trent &dict,
                                                const nos::trent &axdict,
                                                const std::string &name)
{
    std::string type = dict["type"].as_string_except();

    if (type == "cnc_tandem")
        return create_cnc_tandem_move_client(dict, axdict, name);
    else if (type == "simple")
        return create_one_servo_move_client(dict, axdict, name);

    throw std::runtime_error("Unresolved move client api type");

    return nullptr;
}
