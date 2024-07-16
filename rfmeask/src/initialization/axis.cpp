
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

extern std::vector<std::string> sync_types;

void read_gear_config(Axis *axis, const nos::trent &tr)
{
    if (tr.contains("gear") && tr.contains("unitRatio"))
    {
        nos::fprintln(
            "Fault: Axis {} has 'gear' and 'unitRatio' options together",
            axis->name());
        exit(-1);
    }

    if (!tr.contains("gear") && !tr.contains("unitRatio"))
    {
        nos::fprintln("Fault: Axis {} has not 'gear' or 'unitRatio' options",
                      axis->name());
        exit(-1);
    }

    auto *servo = axis->mover;
    std::string motionType = tr["motionType"].as_string_except();

    if (tr.contains("unitRatio"))
    {
        if (servo->fully_closed_mode && !tr["unitRatio_clossed_mode"].is_nil())
        {
            nos::println(axis->name(),
                         "use unitRatio_clossed_mode",
                         tr["unitRatio_clossed_mode"].as_numer());
            axis->gearSettings.set_forced_fullcycle_value(
                tr["unitRatio_clossed_mode"].as_numer());
        }
        else
            axis->gearSettings.set_forced_fullcycle_value(
                tr["unitRatio"].as_numer());
    }

    if (tr.contains("gear"))
    {
        GearSettings settings;

        settings.set_is_linear(motionType == "linear");
        settings.parse_trent(tr["gear"]);

        axis->gearSettings = std::move(settings);
        axis->gearSettingsMode = true;
    }
}

void read_fully_closed_config(Axis *axis, const nos::trent &tr)
{
    if (tr.contains("fully_closed_sensor"))
    {
        FullyClosedGear settings;
        settings.set_configuration_by_name(
            tr["fully_closed_sensor"].as_string());
        axis->fullyClosedGear = settings;
    }
}

void initialize_axis(const nos::trent &p)
{
    //Ассоциируем сервоусилитель
    std::string servo_name = p["servo"].as_string();
    if (devices.count(servo_name) == 0)
    {
        nos::log::fault("Unregistred device {}", servo_name);
        exit(-1);
    }

    AbstractServo *servo = dynamic_cast<AbstractServo *>(devices[servo_name]);

    Axis *axis;
    if (devices.count(p["servo"].as_string()))
    {
        double defaultSpeed_rpm = p["defaultSpeed_rpm"].as_numer();
        double defaultAccel_ms = p["defaultAccel_ms"].as_numer();
        double max_speed_rpm = p["maxSpeed_rpm"].as_numer();
        double min_accel_ms = p["minAccel_ms"].as_numer();
        bool reverse = p["reverse"].as_numer();
        axis = new Axis(p["name"].as_string().c_str(), servo);
        axis->m_loadGain = 1;
        axis->m_needread_load_gain = axis->mover->encoderType_external;
        axis->m_defaultSpeed_rpm = defaultSpeed_rpm;
        axis->m_defaultAccel_ms = defaultAccel_ms;
        axis->mover->maxspd = max_speed_rpm;
        axis->m_min_accel_ms = min_accel_ms;
        axis->set_reverse(reverse);

        if (p["one_axis_correction"].is_string())
        {
            std::string path = p["one_axis_correction"].as_string();
            bool enabled =
                p["one_axis_correction"]["enabled"].as_bool_default(true);
            auto tr = nos::json::parse_file(path);
            ndmath::named_cartesian_correction ncorrection(tr);
            ndmath::cartesian_correction correction =
                ncorrection.make_correction_table();
            axis->set_one_axis_correction(correction, enabled);
            nos::println("ONE AXIS CORRECTION TABLE FOUND");

            nos::println("correction:");
            nos::println("griddim:", correction.griddim());
        }

        read_gear_config(axis, p);
        read_fully_closed_config(axis, p);

        registry_axis(axis);
    }
    else
    {
        nos::log::fault("Device {} is not registred",
                        p["servo"].as_string_except());
        exit(-1);
    }

    nos::log::info("Axis was registred {}", p["name"].as_string_except());
}

void initialize_sync_axis(const nos::trent &p)
{
    //Ассоциируем сервоусилитель
    std::string servo_name = p["servo"].as_string_except();
    if (devices.count(servo_name) == 0)
    {
        nos::log::fault("Unregistred device {}", servo_name);
        exit(-1);
    }

    AbstractServo *servo = dynamic_cast<AbstractServo *>(devices[servo_name]);
    if (servo == nullptr)
    {
        nos::log::fault("servo device has invalid type: {}", servo_name);
        nos::log::fault("{}", typeid(devices[servo_name]).name());
        exit(-1);
    }

    std::string sync_name = p["syncro"].as_string_except();
    if (devices.count(sync_name) == 0)
    {
        nos::log::fault("Unregistred device {}", sync_name);
        exit(-1);
    }

    Device *sync = devices[sync_name];
    if (sync_types.end() ==
        find(sync_types.begin(), sync_types.end(), sync->declared_type()))
    {
        nos::log::fault("sync cactus is not a bush: {}. Unregistred "
                        "syncronizer type. {}",
                        sync_name,
                        sync->declared_type());
        exit(-1);
    }

    AxisSynchro *axis;
    if (devices.count(p["servo"].as_string_except()) == 0)
    {
        nos::log::fault("Device {} isn't registered",
                        p["servo"].as_string_except());
        exit(-1);
    }

    if (devices.count(p["syncro"].as_string_except()) == 0)
    {
        nos::log::fault("Device {} isn't registered",
                        p["syncro"].as_string_except());
        exit(-1);
    }

    double defaultSpeed_rpm = p["defaultSpeed_rpm"].as_numer_except();
    double defaultAccel_ms = p["defaultAccel_ms"].as_numer_except();
    double max_speed_rpm = p["maxSpeed_rpm"].as_numer_except();
    double min_accel_ms = p["minAccel_ms"].as_numer_except();
    bool reverse = p["reverse"].as_numer_default(0);

    double gen_numer = p["gen_numer"].as_numer_default(1000);
    double gen_denum = p["gen_denum"].as_numer_default(1);

    axis = new AxisSynchro(p["name"].as_string().c_str(),
                           dynamic_cast<AbstractServo *>(servo),
                           dynamic_cast<AbstractSyncro *>(sync),
                           true);
    axis->m_loadGain = 1;
    axis->m_needread_load_gain = axis->mover->encoderType_external;
    axis->m_defaultSpeed_rpm = defaultSpeed_rpm;
    axis->m_defaultAccel_ms = defaultAccel_ms;
    axis->mover->maxspd = max_speed_rpm;
    axis->m_min_accel_ms = min_accel_ms;

    axis->set_reverse(reverse);

    axis->gen_denum = gen_denum;
    axis->gen_numer = gen_numer;

    read_gear_config(axis, p);
    read_fully_closed_config(axis, p);

    registry_axis(axis);
    nos::log::info("Create axis {}", p["name"].as_string());
}