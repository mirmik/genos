#ifndef LOGICAL_DEVICE_H
#define LOGICAL_DEVICE_H

/**
@file
    Объект логической абстракции, удобной для управления. Может ссылаться и
    взаимодействовать с несколькими физическими устройствами. Проверяет их
   работоспособность, вычисляя собственную.

    От LogicalDevice наследуют такие классы, как Axis, System.
*/

#include <LogicalController.h>
#include <devices/DeviceChecker.h>
#include <include_helper.h>
#include <logging.h>
#include <nos/trent/settings.h>
#include <set>
#include <vector>

class Device;

class LogicalDevice
{
    std::string m_name = "undefinedLogicalDevice";
    std::vector<Device *> _devices = {};

public:
    /**
     * Объект, проверяющий готовность логического объекта оси к работе.
     * DeviceChecker подписывается на сигналы Device объектов, содержащихся в
     * LogicalDevice::devices
     */
    nos::trent_syncer_slice *runtime_syncer = nullptr;
    DeviceChecker checker{this};
    LogicalController *controller = nullptr;
    LogicalDevice(const std::string &name, const std::string &syncer_prefix);

    virtual bool is_ready();
    virtual bool is_unready();

    const auto &name() const
    {
        return m_name;
    }
    const char *mnemo()
    {
        return m_name.c_str();
    }
    virtual void on_ready_status_changed(bool ready) = 0;

    void setDevices(const std::vector<Device *> &devs);
    void setDevices(const std::set<Device *> &devs);
    virtual void preset() = 0;
    std::vector<int> deviceNumbers();
    virtual ~LogicalDevice() = default;
    LogicalDevice(const LogicalDevice &) = delete;
    LogicalDevice &operator=(const LogicalDevice &) = delete;

    DeviceStatus status()
    {
        return checker.status();
    }

    virtual std::string declared_type() const = 0;
};

#endif
