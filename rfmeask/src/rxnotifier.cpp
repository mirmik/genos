#include <comm/jsonsocket.h>
#include <devices/Device.h>
#include <rxnotifier.h>
#include <utilxx/range.h>

void RxNotifier::set_devices(const std::vector<Device *> &val)
{
    auto observables = val | std::views::transform([](auto dev) {
                           return dev->get_device_status_observable();
                       });

    auto observable = rxcpp::observable<>::iterate(observables);

    observable.merge().subscribe([](auto pair) {
        auto &[sts, dev] = pair;
        // JsonNotification::instance().device_status_notify(dev->name(), sts);
        (void)sts;
        (void)dev;
    });
}