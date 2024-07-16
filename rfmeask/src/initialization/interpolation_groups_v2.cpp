#include <config.h>
#include <logging.h>
#include <tables.h>

#include <groups/InterpolationGroup.h>

void init_interpolation_groups_v2()
{
    nos::log::info("init_interpolation_groups_v2");
    const auto &root = config_settings.node()["intgroups"];

    for (auto tr : root.as_list())
    {
        nos::println(tr);

        auto name = tr["name"].as_string();
        auto type = tr["type"].as_string();

        if (!intgroups_initializers.count(type))
        {
            nos::println("unresolved intgroup type");
            exit(0);
        }

        intgroups_initializers[type](name, tr);
    }
}
