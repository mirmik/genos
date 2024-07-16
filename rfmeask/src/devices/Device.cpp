/**
    @file
*/

#include "Device.h"
#include "DevAlarmLogger.h"
#include <comm/Notify.h>
#include <comm/notifymap.h>
#include <config.h>
#include <functional>
#include <nos/trace.h>
#include <tables.h>

std::vector<Device *> devices_list;
DevAlarmLogger *Device::alarmLogger = NULL;

Device::Device(const std::string &name) : m_name(name)
{
    if (!::runtime_syncer.path().empty())
        runtime_syncer = new nos::trent_syncer_slice(
            ::runtime_syncer, nos::format("devices/{}", name));

    device_status_subject.get_observable().subscribe(
        [this](std::pair<DeviceStatus, Device *> pair) {
            _last_device_status = pair.first;
        });
}

void Device::transactionRefuseError()
{
    if (m_connectionStatus == DISCONNECT)
        return;
    update_status(DeviceStatus::Refuse);
    m_connectionStatus = DISCONNECT;
}

void Device::transactionConnectionSuccess()
{
    if (m_connectionStatus == CONNECT)
        return;
    m_connectionStatus = CONNECT;

    if (_first_connection_handler_passed == false)
    {
        nos::println("FIRST CONNECTION", name());
        on_first_connection();
    }
    _first_connection_handler_passed = true;
}

void Device::transactionAlarmError(const std::string &alarm)
{
    if (device_status() == DeviceStatus::Alarm)
        return;
    update_status(DeviceStatus::Alarm);
    if (alarmLogger)
    {
        try
        {
            alarmLogger->logAlarm(alarm, this);
        }

        catch (const std::exception &ex)
        {
            nos::println("Exception in alarm handle (transactionAlarmError)");
        }
    }
}

void Device::setAlarmLogger(DevAlarmLogger *logger)
{
    alarmLogger = logger;
}

void Device::update_status_if_needed(DeviceStatus status)
{
    if (device_status() == status)
        return;
    update_status(status);
}

void Device::update_status(DeviceStatus status)
{
    device_status_subject.get_subscriber().on_next(
        std::make_pair(status, this));
    deviceStatusNotify.notify((int)status);
}

void Device::transactionReady()
{
    update_status_if_needed(DeviceStatus::Ready);
    transactionConnectionSuccess();
}

DeviceStatus Device::device_status()
{
    return _last_device_status;
}

bool Device::isReady()
{
    return device_status() == DeviceStatus::Ready;
}

bool Device::isConnected()
{
    return m_connectionStatus == CONNECT;
}

void Device::startUpdate()
{
    nos::log::debug("Device::startUpdate: dev: {}", name());
    std::thread &&threadBar = std::thread(&Device::updaterThreadFunc, this);
    updater_thread = std::move(threadBar);
}

void registry_device(Device *dev, NotifyBrocker &brocker)
{
    devices_list.push_back(dev);
    dev->number = devices_list.size() - 1;
    dev->notifiesRegistry(brocker);

    // TODO: Убрать условие после исключения этой строчки из файлов
    // инициализации.
    if (devices.count(dev->name()) == 0)
        devices.insert(std::make_pair(dev->name(), dev));
}

void Device::notifyreg(NotifyTheme &theme,
                       const char *str,
                       int32_t number,
                       const char *header,
                       int32_t tag)
{
    char buf[48];
    sprintf(buf, str, number);
    theme.setName(buf);
    theme.setNCPIHeader(header, std::vector<int32_t>({number}));
    brocker.themeRegistry(&theme, tag, std::vector<int32_t>({number}));
}

void Device::notifiesRegistry(NotifyBrocker &brocker)
{
    notifyreg(deviceStatusNotify,
              "DeviceStatusNotify(%d)",
              number,
              NPattern_DeviceStatus,
              Tag_DeviceStatus);
    notifyreg(alarm_notify,
              "DeviceAlarmNotify(%d)",
              number,
              NPattern_DeviceAlarm,
              Tag_DeviceAlarm);
    notifyreg(ready_notify,
              "DeviceReadyNotify(%d)",
              number,
              NPattern_DeviceReady,
              Tag_DeviceReady);
}

Device *get_device(const std::string &str)
{
    for (auto d : devices_list)
    {
        if (str == d->name())
            return d;
    }
    return nullptr;
}

Device *get_device(int no)
{
    return devices_list[no];
}

void Device::alarm_handle()
{
    alarm_notify.notify(status_string().c_str(), status_string().size());
}

void Device::ready_handle()
{
    ready_notify.notify();
}

void Device::command(const nos::argv &args, nos::ostream &os)
{
    if (args.size() == 0)
    {
        nos::println_to(os, command_help());
        return;
    }

    if (args[0] == "name")
    {
        nos::println_to(os, name());
        return;
    }
    else if (args[0] == "type")
    {
        nos::println_to(os, declared_type());
        return;
    }
    else if (args[0] == "help")
    {
        nos::println_to(os, command_help());
        return;
    }

    nos::println_to(os, "Undefined command");
    return;
}

std::string Device::command_help()
{
    return "name - get device name\n"
           "type - get device type\n"
           "help - get this help\n";
}