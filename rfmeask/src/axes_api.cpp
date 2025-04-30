
#include <axes_api.h>
#include <filesystem>
#include <tables.h>
#include <util.h>

python_script_hook hook_is_global_move_allowed;
python_script_hook hook_is_global_move_allowed2;

void open_global_hooks()
{
    nos::println("Open global hooks");

    std::string hooks_dir = "/home/rfmeas/.rfmeask/hooks";
    std::string hook_file = hooks_dir + "/is_global_move_allowed.py";
    std::string hook2_file = hooks_dir + "/is_global_move_allowed2.py";

    hook_is_global_move_allowed.init(hook_file, "result = True\n");
    hook_is_global_move_allowed2.init(hook2_file, "result = True\n");
}

void open_hooks()
{
    auto &axesList = axis_list_ref();

    for (auto &axis : axesList)
    {
        axis->init_hooks();
    }
}

bool GlobalMoveAllowed(std::vector<std::pair<int, double>> args)
{
    nos::trent indata, outdata;
    nos::trent system_state = AbstractAxis::compile_system_state_to_trent();

    indata["system_state"] = system_state;
    indata["args"].init(nos::trent_type::list);

    for (const std::pair<int, double> &pair : args)
    {
        nos::trent arg;
        arg["number"] = pair.first;
        arg["value"] = pair.second;
        indata["args"].as_list().push_back(arg);
    }

    hook_is_global_move_allowed.execute(indata, outdata);

    if (PyErr_Occurred())
    {
        PyErr_Print();
        return false;
    }

    if (!outdata["result"].is_bool())
    {
        return false;
    }

    return outdata["result"].as_bool();
}

void GlobalAbsmoveParted(std::vector<std::pair<int, double>> args)
{
    bool allowed = GlobalMoveAllowed(args);
    if (!allowed)
    {
        throw AxisLimitException();
    }

    auto &axesList = axis_list_ref();

    for (const std::pair<int, double> &pair : args)
    {
        auto &axis = axesList[pair.first];
        auto target = pair.second;

        axis->absoluteUnitMove(target);
    }
}