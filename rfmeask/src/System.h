/**
    @file
    Логическое устройство, отвечающее за проверку
    работоспособности всех устройств системы
*/

#ifndef SYSTEM_DEVICE_H
#define SYSTEM_DEVICE_H

#include <LogicalDevice.h>

class System : public LogicalDevice
{

    static System *_instance;

public:
    std::vector<LogicalDevice *> deps = {};
    System() : LogicalDevice("System", "system"){};
    virtual void notifiesRegistry();
    void preset() override;

    void on_ready_status_changed(bool ready) override
    {
        if (ready)
        {
            nos::println("System is ready. All devices nominal.");
        }
        else
        {
            nos::println("System is unready.");
        }
    }

    std::string declared_type() const override
    {
        return "system";
    }

    static System *instance()
    {
        if (_instance == nullptr)
            _instance = new System();
        return _instance;
    }
};

#endif