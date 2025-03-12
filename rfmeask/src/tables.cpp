#include <groups/InterpolationGroup.h>
#include <tables.h>

bool have_can = false;
bool have_mrs = false;
bool have_mrs_modbus = false;
bool have_drv = false;
bool have_mitsu_board_servos = false;

std::map<std::string, Device *> devices;
std::vector<AbstractAxis *> axis_list;
std::vector<InterpolationGroup_v2 *> group_list;

AbstractAxis *get_axis(size_t num)
{
    if (num >= axis_list.size())
        return nullptr;

    return axis_list[num];
}

AbstractAxis *get_axis(const std::string_view &name)
{
    for (auto *ax : axis_list)
    {
        if (name == ax->mnemo())
        {
            return ax;
        }
    }
    return nullptr;
}

std::vector<InterpolationGroup_v2 *> get_group_list()
{
    return group_list;
}

void add_group_to_list(InterpolationGroup_v2 *group)
{
    group_list.push_back(group);
}

size_t groups_total()
{
    return group_list.size();
}

InterpolationGroup_v2 *find_group(int number)
{
    return group_list[number];
}

size_t axes_total()
{
    return axis_list.size();
}

void add_axis_to_list(AbstractAxis *axis)
{
    axis_list.push_back(axis);
}

std::vector<AbstractAxis *> get_axis_list()
{
    return axis_list;
}

std::vector<AbstractAxis *> &axis_list_ref()
{
    return axis_list;
}

void delete_axis(size_t axno)
{
    if (axno >= axis_list.size())
        return;

    delete axis_list[axno];
    axis_list.erase(std::next(axis_list.begin(), axno));
}

AbstractAxis *find_axis(const std::string &name)
{
    if (isdigit(name[0]))
    {
        int no = std::stoi(name);
        return axis_list[no];
    }
    else
    {
        auto it = std::find_if(
            axis_list.begin(), axis_list.end(), [&name](AbstractAxis *ax) {
                return ax->name() == name;
            });
        if (it != axis_list.end())
            return *it;
        return nullptr;
    }
}

InterpolationGroup_v2 *find_group(const std::string &name)
{
    auto it = std::find_if(
        group_list.begin(),
        group_list.end(),
        [&name](InterpolationGroup_v2 *gr) { return gr->name() == name; });
    if (it != group_list.end())
        return *it;
    return nullptr;
}

AbstractServo *get_servo(std::string name)
{
    auto it = devices.find(name);
    if (it != devices.end())
    {
        return dynamic_cast<AbstractServo *>(it->second);
    }
    return nullptr;
}