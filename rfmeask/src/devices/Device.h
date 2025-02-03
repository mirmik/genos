/**
    @file
*/

#ifndef DEVICE_H
#define DEVICE_H

#include <comm/Notify.h>
#include <comm/notifymap.h>
#include <exception.h>
#include <fstream>
#include <igris/event/multiple_delegate.h>
#include <include_helper.h>
#include <interrupt.h>
#include <iostream>
#include <map>
#include <nos/io/ostream.h>
#include <nos/shell/argv.h>
#include <nos/trent/binder.h>
#include <nos/trent/settings.h>
#include <rxcpp/rx.hpp>
#include <status.h>
#include <thread>
#include <vector>

class DevAlarmLogger;

/**
    @brief Базовый класс для драйверов физических устройств.
    Здесь реализуется логика проверки их доступности и реализуются сигналы для
    уведомления логических устройств.
*/
class Device
{
protected:
    rxcpp::subjects::behavior<std::pair<DeviceStatus, Device *>>
        device_status_subject =
            rxcpp::subjects::behavior<std::pair<DeviceStatus, Device *>>(
                std::make_pair(DeviceStatus::Init, this));
    DeviceStatus _last_device_status = DeviceStatus::Init;

public:
    ConnectionStatus m_connectionStatus = ConnectionStatus::__CON_INIT;
    int32_t m_updaterTimeout = 70;
    std::string _status_string = "undefined";

    NotifyTheme deviceStatusNotify = {};
    NotifyTheme alarm_notify = {};
    NotifyTheme ready_notify = {};

    bool forceAlarm = false;
    int number = -1;
    std::string _name = "unnamedDevice";
    nos::trent_syncer_slice *runtime_syncer = nullptr;

    // for configuration
    std::thread updater_thread = {};

public:
    void alarm_handle();
    void ready_handle();

    auto status_observable() const
    {
        return device_status_subject.get_observable();
    }

    virtual std::string declared_type() const = 0;
    virtual void set_zero_position()
    {
        throw FunctionIsNotSupported();
    }

    virtual double last_position()
    {
        return 0;
    }
    virtual double last_unit_position()
    {
        return 0;
    }

    auto get_device_status_observable() const
    {
        return device_status_subject.get_observable();
    }

    virtual int getLimitSwitchStatus()
    {
        return 0;
    }

    virtual void generate_trigger() {}
    virtual void manual_trigger_mode(int32_t en) {}

    Device(const std::string &name);

    // ConnectionStatus connectionStatus()
    //{
    //    return m_connectionStatus;
    //}
    DeviceStatus deviceStatus()
    {
        return device_status();
    }
    int32_t updaterTimeout()
    {
        return m_updaterTimeout;
    }
    void updaterTimeout(int32_t timeout)
    {
        m_updaterTimeout = timeout;
    }
    virtual void testAlarm(){FUNCTION_IS_NOT_RELEASED}

    DeviceStatus getStatus()
    {
        return device_status();
    }

    int32_t getStatus_as_int32()
    {
        return (int32_t)device_status();
    }

    enum SystemStatus
    {
        Normal,
        SystemFault,
    };

    virtual void relative_move(double dist)
    {
        throw FunctionIsNotSupported();
    }
    virtual void absolute_move(double dist)
    {
        throw FunctionIsNotSupported();
    }
    virtual void relative_unit_move(double dist)
    {
        throw FunctionIsNotSupported();
    }
    virtual void absolute_unit_move(double dist)
    {
        throw FunctionIsNotSupported();
    }

    virtual void stop()
    {
        throw FunctionIsNotSupported();
    }

    virtual void updaterThreadFunc() = 0;
    virtual void preset() = 0;

    /*  Следующие функции генерируют сигналы, оповещающие логические объекты
        о изменении статуса устройства.
        Сами функции должны вызываться после совершения транзакции с целью
        запуска обработки соотв событий.
    */
    void transactionRefuseError();
    void transactionConnectionSuccess();
    void transactionAlarmError(const std::string &description_or_code);
    void transactionReady();

    bool _first_connection_handler_passed = false;
    virtual void on_first_connection() {}

    bool isReady();
    bool isConnected();

    void alarm(std::pair<int, int> err)
    {
        std::string codestr = std::to_string(err.first);
        transactionAlarmError(codestr);
    }

    const std::string &status_string()
    {
        return _status_string;
    }

    //Функция-поток обработчика обновлений.
    void startUpdate();

    const std::string &name() const
    {
        return _name;
    };

    /*  Запрос Alarm кода устройства.
        Для расшифровки смотри документацию.
    */
    virtual int getAlarmStatus()
    {
        FUNCTION_IS_NOT_RELEASED
    }
    virtual std::string getAlarm(){FUNCTION_IS_NOT_RELEASED};

    static void setAlarmLogger(DevAlarmLogger *logger);

    void notifyreg(NotifyTheme &theme,
                   const char *str,
                   int32_t number,
                   const char *header,
                   int32_t tag);

    virtual void notifiesRegistry(NotifyBrocker &brocker);

    virtual std::string info()
    {
        return "info unreleased";
    }

    void serve_impl(){};

    virtual std::pair<std::string, std::string>
    get_parameter(const std::string &mnemo)
    {
        throw FunctionIsNotSupported();
    }

    virtual void set_parameter(const std::string &mnemo,
                               const std::string &value,
                               const std::string &format)
    {
        throw FunctionIsNotSupported();
    }

    void update_status(DeviceStatus status);
    void update_status_if_needed(DeviceStatus status);

    virtual void enable_torque_notification()
    {
        throw FunctionIsNotSupported();
    }
    virtual double current_torque()
    {
        return 0;
    }
    virtual ~Device() = default;

    virtual void command(const nos::argv &args, nos::ostream &os);
    virtual std::string command_help();

    virtual bool is_ready()
    {
        return device_status() == DeviceStatus::Ready;
    }

    virtual bool is_unready()
    {
        return device_status() != DeviceStatus::Ready;
    }

    DeviceStatus device_status();
    
    virtual void com_trigger_duration(int d) {}
    virtual void com_trigger_polarity(int d) {}

private:
    static DevAlarmLogger *alarmLogger;
};

extern std::vector<Device *> devices_list;
extern std::map<std::string, Device *> deviceTableMap;

void initialize_the_system_according_to_the_configuration();
void registry_device(Device *dev, NotifyBrocker &brocker);
Device *get_device(const std::string &name);
Device *get_device(int no);

#endif
