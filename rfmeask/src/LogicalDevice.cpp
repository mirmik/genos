/**
    @file
*/

#include <LogicalDevice.h>
#include <devices/Device.h>
#include <devices/DeviceChecker.h>
#include <nos/trent/settings.h>

extern std::vector<Device *> devices_list;
extern nos::trent_syncer_slice runtime_syncer;

LogicalDevice::LogicalDevice(const std::string &name,
                             const std::string &syncer_prefix) :

    _name(name),
    runtime_syncer(new nos::trent_syncer_slice(
        ::runtime_syncer, nos::format("{}/{}", syncer_prefix, name)))
{
}

void LogicalDevice::setDevices(const std::vector<Device *> &devs)
{
    //Инициализация объекта checker.
    _devices = devs;
    checker.setDevices(_devices);
}

void LogicalDevice::setDevices(const std::set<Device *> &devs)
{
    std::vector<Device *> devices;
    for (auto ptr : devs)
    {
        devices.push_back(ptr);
    }
    setDevices(devices);
}

std::vector<int> LogicalDevice::deviceNumbers()
{
    std::vector<int> ret;
    for (auto d : _devices)
    {
        ret.push_back(d->number);
    }
    return ret;
}

bool LogicalDevice::is_ready()
{
    return checker.status() == DeviceStatus::Ready;
}

bool LogicalDevice::is_unready()
{
    return checker.status() != DeviceStatus::Ready;
}