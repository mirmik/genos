#ifndef REACTIVE_X_NOTIFIER_H
#define REACTIVE_X_NOTIFIER_H

#include <rxcpp/rx.hpp>

class RxNotifier
{
private:
    RxNotifier() = default;

public:
    static RxNotifier &instance()
    {
        static RxNotifier instance;
        return instance;
    }

    RxNotifier(RxNotifier const &) = delete;
    void operator=(RxNotifier const &) = delete;

    void set_devices(const std::vector<Device *> &val);
};

#endif