#include <CommonAxis.h>
#include <tables.h>

#include <AbstractAxis.h>
#include <AbstractSyncro.h>
#include <servos/AbstractServo.h>

#include <moveapi/MoveApiClient.h>
#include <SyncScanApiClient.h>

#include <logging.h>
#include <nos/trace.h>

#include <initialization/move_api_client.h>
#include <initialization/scan_api_client.h>
#include <nos/trent/trent.h>

void common_axis_move_api_parse(CommonAxis *axis,
                                const nos::trent &dict,
                                const nos::trent &axdict)
{
    MoveApiClient *client =
        create_move_api_client_from_dict(dict, axdict, axis->name());
    axis->move_client.reset(client);
    client->set_common_axis(axis);
}

void common_axis_initializer(const std::string &name, const nos::trent &dict)
{
    auto *axis = new CommonAxis(name);

    if (!dict.contains("moveapi"))
    {
        nos::log::fault("CommonAxis without moveapi");
        exit(0);
    }
    common_axis_move_api_parse(axis, dict["moveapi"], dict);

    if (dict.contains("scanapi"))
    {
        common_axis_scan_api_parse(axis, dict["scanapi"]);
    }

    if (dict.contains("reverse"))
    {
        axis->set_reverse(dict["reverse"].as_bool());
    }

    registry_axis(axis);
    axis->init();
    // axes_list.push_back(axis);
}

void init_common_axis()
{
    axis_types.insert("common_axis");
    axis_initializers.insert(
        std::make_pair("common_axis", common_axis_initializer));
}