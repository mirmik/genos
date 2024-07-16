/**
    @file
*/

#include <AbstractAxis.h>
#include <System.h>
#include <comm/Notify.h>
#include <comm/notifymap.h>
#include <logging.h>
#include <tables.h>

#include <devices/DeviceChecker.h>

System *System::_instance = nullptr;

void System::notifiesRegistry()
{
    checker.statusNotify.setName("SystemStatus");
    checker.statusNotify.setNCPIHeader(NPattern_SystemStatus,
                                       std::vector<int32_t>({}));
    brocker.themeRegistry(
        &checker.statusNotify, Tag_SystemStatus, std::vector<int32_t>({}));
}

void System::preset()
{
    nos::log::trace("system preset");
    for (auto a : get_axis_list())
    {
        a->preset();
    }
}