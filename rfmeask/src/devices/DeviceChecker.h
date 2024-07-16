#ifndef DEVICE_CHECKER_H
#define DEVICE_CHECKER_H

/**
    @file DeviceChecker.h
    Класс реализует учет готовности к работе устройств
    участвующих в работе логических устройств.
*/

#include <comm/notifymap.h>
#include <igris/event/multiple_delegate.h>
#include <map>
#include <mutex>
#include <rxcpp/rx.hpp>
#include <status.h>
#include <vector>

class NotifyTheme;
class LogicalDevice;
class Device;

class DeviceChecker
{
public:
    enum Status
    {
        None = -1,
        Ready = 0,
        Unready = 1,
    };

    std::mutex mtx = {};
    uint8_t m_status = None;
    std::vector<Device *> devtbl = {};
    std::map<Device *, DeviceStatus> devmap = {};
    LogicalDevice *parent = nullptr;

public:
    // rxcpp::subjects::subject<bool> is_ready_subject = {};
    NotifyTheme statusNotify = {};

    DeviceChecker(LogicalDevice *parent) : parent(parent) {}
    DeviceChecker(const DeviceChecker &) = delete;
    DeviceChecker &operator=(const DeviceChecker &) = delete;

    const auto &getStatus()
    {
        return m_status;
    }

    void setDevices(const std::vector<Device *> &val);
    void statusUpdate(Device *dev);

    DeviceStatus status();
};

#endif
