
#include <Synchronizer.h>
#include <modes.h>
#include <tables.h>

void initialize_sync(const std::string &name, const nos::trent &dict)
{
    ::have_drv = true;

    int address;
    int updateTime;

    updateTime = dict["updateTime"].as_numer_default(50);
    address = dict["address"].as_numer_except();

    //Создаем и инициализируем объект */устройства.
    auto sync = new Synchronizer(name.c_str(), &drv, address);
    sync->updaterTimeout(updateTime);

    registry_device(sync, brocker);

    devices.insert(std::make_pair(name, sync));
    nos::log::info(
        "Registred Syncronizer with name {}, address {} and updateTime {}ms.",
        name,
        address,
        updateTime);
}
