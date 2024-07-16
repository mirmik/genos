/**
    @file
*/

#include <SimulatorSyncro.h>

void SimulatorSyncro::updaterThreadFunc()
{
    while (1)
    {
        if (inscan)
        {
            double dist = std::abs(ax2local(binded_ax->last_unit_position()) -
                                   start_position);

            if (nextpos < dist)
            {
                nextpos = nextpos + step;
                point_notify();

                idx++;
                if (idx == scanpoints)
                {
                    descan();
                }
            }
        }

        try
        {
            test();
        }
        catch (const DeviceRefuseException &)
        {
            goto end;
        }
        catch (const std::exception &ex)
        {
            nos::println("std::exception");
            nos::println(ex.what());
            BUG();
        }
        catch (...)
        {
            BUG();
        }

    end:
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
