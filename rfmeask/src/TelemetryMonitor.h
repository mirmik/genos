/*#ifndef MONITOR_H
#define MONITOR_H

#include <chrono>
#include <jsoncpp/json/json.h>
#include <thread>

#include <crow/address.h>
#include <crow/nodes/publisher_node.h>

#include <Axis.h>
#include <devices/Device.h>
#include <tables.h>

using namespace std::chrono_literals;

class TelemetryMonitor
{
    crow::hostaddr crowker = crow::address(".12.127.0.0.1:10009");
    crow::publisher_node system_theme_publisher{crowker, "rfmeas/system"};

    std::vector<crow::publisher_node> axis_themes = {};
    std::vector<crow::publisher_node> device_themes = {};

    std::thread spin = {};

public:
    TelemetryMonitor() = default;

    void start_spin()
    {
        spin = std::thread(&TelemetryMonitor::spin_handle, this);
    }

    static std::string to_compact_string(const Json::Value &json)
    {
        Json::StreamWriterBuilder builder;
        builder["indentation"] = ""; // If you want whitespace-less output
        const std::string output = Json::writeString(builder, json);
        return output;
    }

    void send_system_information()
    {
        Json::Value json;

        json["axes_total"] = (Json::UInt64)axes_list.size();
        json["devices_total"] = (Json::UInt64)devices_list.size();

        system_theme_publisher.publish(to_compact_string(json));
    }

    void send_axes_telemetry()
    {
        for (unsigned int i = 0; i < axes_list.size(); ++i)
        {
            Json::Value ax;
            ax["pos"] = axes_list[i]->getUnitPosition();
            // ax["alarm"] = axes_list[i]->alarm_status();
            axis_themes[i].publish(to_compact_string(ax));
        }
    }

    void send_devices_telemetry()
    {
        for (unsigned int i = 0; i < devices_list.size(); ++i)
        {
            auto &dev = devices_list[i];
            Json::Value json;
            (void)dev;
            // json["alarm"] = dev->alarm_status();
            // devices_themes[i].publish(to_compact_string(json));
        }
    }

    void spin_handle()
    {
        send_system_information();

        while (1)
        {
            send_axes_telemetry();
            send_devices_telemetry();

            std::this_thread::sleep_for(100ms);
        }
    }
};

#endif*/