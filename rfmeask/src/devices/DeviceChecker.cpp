/**
    @file
*/

#include <LogicalDevice.h>
#include <comm/Notify.h>
#include <devices/Device.h>
#include <devices/DeviceChecker.h>
#include <nos/print.h>

void DeviceChecker::statusUpdate(Device *dev)
{
    uint8_t ready = 0;
    uint8_t alarm = 0;
    uint8_t refuse = 0;
    std::lock_guard<std::mutex> lock(mtx);

    devmap[dev] = dev->deviceStatus();
    for (std::pair<Device *, DeviceStatus> p : devmap)
    {
        switch (p.second)
        {
        case DeviceStatus::Init:
        case DeviceStatus::Refuse:
            refuse++;
            break;
        case DeviceStatus::Alarm:
            alarm++;
            break;
        case DeviceStatus::Ready:
            ready++;
            break;
        }
    }

    if (ready + alarm + refuse != (signed)devtbl.size())
    {
        return;
    };

    if (ready == devtbl.size() && m_status != Ready)
    {
        statusNotify.notify(0);
        m_status = Ready;
        std::thread([parent = this->parent]() {
            if (parent)
                parent->on_ready_status_changed(true);
        }).detach();
        return;
    }
    else if (m_status != Unready)
    {
        statusNotify.notify(1);
        m_status = Unready;
        std::thread([parent = this->parent]() {
            if (parent)
                parent->on_ready_status_changed(false);
        }).detach();
        return;
    }
}

void DeviceChecker::setDevices(const std::vector<Device *> &val)
{
    devtbl = val;
    devmap.clear();
    for (auto dev : devtbl)
    {
        if (dev != nullptr)
        {
            devmap.insert(std::make_pair(dev, dev->deviceStatus()));
            dev->get_device_status_observable().subscribe(
                [this](auto pair) { this->statusUpdate(pair.second); });
        }
    }
}

DeviceStatus DeviceChecker::status()
{
    std::lock_guard<std::mutex> lock(mtx);
    if (m_status == Ready)
    {
        return DeviceStatus::Ready;
    }
    else if (m_status == Unready)
    {
        return DeviceStatus::Alarm;
    }
    else
    {
        return DeviceStatus::Init;
    }
}
