#include <moveapi/CNCMovingClient.h>
#include <CommonAxis.h>
#include <groups/RalgoCncInterpolationGroup.h>
#include <ndmath/named_cartesian_correction.h>
#include <ranges>
#include <tables.h>
#include <utilxx/trent_parse_helper.h>

bool is_name_of_common_axis_with_ralgo_moving_part(std::string name)
{
    auto *ax = get_axis(name);
    if (ax == nullptr)
        return false;

    auto *rax = dynamic_cast<CommonAxis *>(ax);
    if (rax == nullptr)
        return false;

    auto *mov = rax->move_client.get();
    if (mov == nullptr)
        return false;

    auto *tandem = dynamic_cast<CNCTandemMovingClient *>(mov);
    if (tandem == nullptr)
        return false;

    return true;
}

static inline auto parse_axes(const nos::trent &dict)
{
    auto axes_names = trent_get_string_vector(dict["axes"]);

    for (auto &axname : axes_names)
    {
        if (!is_name_of_common_axis_with_ralgo_moving_part(axname))
        {
            throw std::runtime_error("Axis " + axname +
                                     " is not a common axis");
        }
    }

    auto view = std::ranges::views::transform(
        axes_names, [](const std::string &name) { return get_axis(name); });

    return std::vector<AbstractAxis *>(view.begin(), view.end());
}

static inline auto parse_driver(const nos::trent &dict)
{
    auto name = dict["driver"].as_string();
    auto *device = dynamic_cast<RalgoCNC *>(get_device(name));
    return device;
}

static inline std::optional<ndmath::named_cartesian_correction>
parse_correction_table(const nos::trent &dict)
{
    std::optional<ndmath::named_cartesian_correction> ret;
    return ret;
}

void ralgocnc_igroup_initializer(const std::string &name,
                                 const nos::trent &dict)
{
    auto *cnc = parse_driver(dict);
    assert(cnc != nullptr);
    auto *group = new RalgoCncInterpolationGroup(dict["name"].as_string(), cnc);

    group->set_axes(parse_axes(dict));
    // group->set_driver();
    // group->set_named_cartesian_correction(parse_correction_table(dict));

    group->init();
    add_group_to_list(group);
}

void init_ralgocnc_igroup()
{
    intgroups_initializers.insert(
        std::make_pair("ralgocnc_igroup", &ralgocnc_igroup_initializer));
}
