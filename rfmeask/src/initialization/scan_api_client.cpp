#include <AbstractSyncro.h>
#include <RalgoCNC.h>
#include <SyncScanApiClient.h>
#include <devices/Device.h>
#include <initialization/scan_api_client.h>
#include <nos/trent/json.h>
#include <nos/trent/json_print.h>
#include <servos/AbstractServo.h>
#include <tables.h>

void common_axis_scan_api_parse(CommonAxis *axis, const nos::trent &dict)
{
    std::string sync_name = dict.get_as_string_ex("driver");

    if (sync_name == "None")
        return;

    std::string scansrv_name = dict.get_as_string_ex("srv");
    int scanreverse = dict.get_as_numer_def("reverse", 0);

    auto *sync = dynamic_cast<AbstractSyncro *>(get_device(sync_name));
    auto *scansrv = dynamic_cast<AbstractServo *>(get_device(scansrv_name));

    if (sync == nullptr)
    {
        nos::log::fault("helix_axis: syncronizer device not found name: {}",
                        sync_name);
        exit(0);
    }

    if (scansrv == nullptr)
    {
        nos::log::fault("helix_axis: scansrv device not found name: {}",
                        scansrv_name);
        exit(0);
    }
    auto ptr = new SyncScanApiClient(axis, sync);

    ptr->syncsrv = scansrv;

    nos::println(nos::json::to_string(dict));
    nos::println("gain: {}", dict.get_as_string_ex("gain"));
    ptr->evaluate_gain(dict.get_as_string_ex("gain"));

    axis->scan_client.reset(ptr);

    if (scanreverse)
        axis->scan_client->set_reverse(true);
}
