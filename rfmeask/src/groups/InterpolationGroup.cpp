/**
    @file
*/

#include <AbstractAxis.h>
#include <groups/InterpolationGroup.h>
#include <ndmath/util.h>
#include <nos/print.h>
#include <nos/print/stdtype.h>
#include <optional>
#include <rabbit/intersect.h>
#include <rabbit/util.h>
#include <ralgo/cnc/util.h>
#include <ralgo/linalg/linalg.h>
#include <tables.h>

InterpolationGroup_v2 *get_interpolation_group(const std::string &str)
{
    for (auto a : get_group_list())
    {
        if (str == a->mnemo())
            return a;
    }
    return nullptr;
}

void init_interpolation_groups()
{
    // pass
}

size_t InterpolationGroup_v2::index_of(AbstractAxis *ax)
{
    for (size_t i = 0; i < axes.size(); i++)
        if (axes[i] == ax)
            return i;
    throw std::runtime_error("wrong axis");
}

double InterpolationGroup_v2::update_position_and_reverse_correct_unit_position(
    AbstractAxis *ax, double pos)
{
    assert(!isnan(pos));
    assert(!isinf(pos));

    int index = index_of(ax);
    _last_position_noncorrected[index] = pos;
    if (correction_enabled)
        _last_position =
            ncorrection->inverted_corrected_point(_last_position_noncorrected);
    else
    {
        _last_position = _last_position_noncorrected;
    }
    return _last_position[index];
}

int nos_print(nos::ostream &os, const Operation &s)
{
    switch (s)
    {
    case Operation::LINEARSCAN:
        return *nos::print_to(os, "LINEARSCAN");
    case Operation::TESTCTR:
        return *nos::print_to(os, "TESTCTR");
    case Operation::USERMOVEINIT:
        return *nos::print_to(os, "MOVEINIT");
    case Operation::USERMOVE:
        return *nos::print_to(os, "MOVE");
    case Operation::NONE:
        return *nos::print_to(os, "NONE");
    }
    return 0;
}

void InterpolationGroup_v2::on_axis_operation_status(AbstractAxis *axis,
                                                     Operation operation)
{
    size_t index = 0;
    for (size_t i = 0; i < axes.size(); ++i)
    {
        if (axes[i] == axis)
        {
            index = i;
            break;
        }
    }

    opstatus_array[index] = operation;
    nos::println("GROUPSTATUS:", opstatus_array);

    operation_status_analyze();
}

void InterpolationGroup_v2::operation_status_analyze()
{
    bool all_is_none =
        std::all_of(opstatus_array.begin(),
                    opstatus_array.end(),
                    [](Operation op) { return op == Operation::NONE; });
    Operation result = all_is_none ? Operation::NONE : Operation::USERMOVE;
    nos::println("INTERPOLATION GROUP STATUS:", result);
    if (last_operation_status != result)
    {
        last_operation_status = result;
        for (auto *ax : axes)
            ax->send_operation_status_notify_for_interpolation_mode(result);
    }
}

void InterpolationGroup_v2::set_axes(const std::vector<AbstractAxis *> &ref)
{
    axes = ref;
    opstatus_array.resize(axes.size());
    for (auto *ax : axes)
    {
        ax->set_igcontroller(this);
    }
    _last_position.resize(ref.size());
    _last_position_noncorrected.resize(ref.size());

    ralgo::vecops::fill(_last_position_noncorrected, 0);
    ralgo::vecops::fill(_last_position, 0);
}

void InterpolationGroup_v2::init_correction_table()
{
    load_correction_table();
}

int InterpolationGroup_v2::incmove_to_border(std::vector<double> vec)
{
    throw FunctionIsNotSupported();
}

void InterpolationGroup_v2::load_correction_table()
{
    auto &root = runtime_syncer->node();
    if (root.contains("correction") == false)
    {
        ncorrection = ndmath::cartesian_correction();
        save_correction_table();
        return;
    }

    auto &correction = root["correction"].as_dict();
    correction_enabled = correction["enable"].as_bool_default(false);

    if (root["correction"].contains("table") == false)
        return;

    auto cc = multi_axes_cartesian_correction::trent_to_correction(
        correction["table"]);

    ncorrection = cc;
}

void InterpolationGroup_v2::save_correction_table()
{
    auto &root = runtime_syncer->node();
    auto &correction = root["correction"].as_dict();
    correction["enable"] = correction_enabled;

    if (ncorrection.has_value())
    {
        correction["table"] =
            multi_axes_cartesian_correction::correction_to_trent(*ncorrection);
    }
    else
    {
        ncorrection = ndmath::cartesian_correction();
        correction["table"] =
            multi_axes_cartesian_correction::correction_to_trent(*ncorrection);
    }

    runtime_syncer->save();
}

std::pair<double, double> InterpolationGroup_v2::parted_limits(int idx)
{
    return parted_limits(axes[idx]);
}

std::pair<double, double>
InterpolationGroup_v2::parted_limits(const AbstractAxis *ax)
{
    return {ax->unitBackwardLimit(), ax->unitForwardLimit()};
}

void InterpolationGroup_v2::set_speeds_ups(const std::vector<double> &spds)
{
    std::vector<int> indexes;
    std::iota(indexes.begin(), indexes.end(), 0);
    return set_speeds_ups_parted(indexes, spds);
}

void InterpolationGroup_v2::set_accels_ups(const std::vector<double> &accs)
{
    std::vector<int> indexes;
    std::iota(indexes.begin(), indexes.end(), 0);
    return set_accels_ups2_parted(indexes, accs);
}

void InterpolationGroup_v2::set_decels_ups(const std::vector<double> &dccs)
{
    std::vector<int> indexes;
    std::iota(indexes.begin(), indexes.end(), 0);
    return set_decels_ups2_parted(indexes, dccs);
}

void InterpolationGroup_v2::move_parted(AbstractAxis *ax, double arg)
{
    std::vector<int> idxs = get_axes_indexes();
    int num = ax->number();
    int subidx = -1;

    for (size_t i = 0; i < idxs.size(); ++i)
    {
        if (idxs[i] == num)
            subidx = idxs[i];
    }

    absmove_parted(
        std::vector<std::pair<int, double>>{std::make_pair(subidx, arg)});
}

void InterpolationGroup_v2::absmove_parted(
    const std::vector<std::pair<int, double>> &args)
{
    std::vector<GrpAxisMovePair> indexes_and_coords;
    for (auto arg : args)
    {
        AbstractAxis *ax = get_axis(arg.first);
        double val = arg.second;
        indexes_and_coords.emplace_back(GrpAxisMovePair{ax, val});
    }
    absmove(indexes_and_coords);
}

nos::trent InterpolationGroup_v2::correction_as_trent()
{
    if (ncorrection.has_value() == false)
        return nos::trent();
    auto trent =
        multi_axes_cartesian_correction::correction_to_trent(*ncorrection);
    trent["enable"] = correction_enabled;
    return trent;
}

std::vector<GrpAxisMovePair>
InterpolationGroup_v2::to_indexes_and_coords(const nos::argv &args)
{
    std::vector<GrpAxisMovePair> indexes_and_coords;
    for (size_t i = 1; i < args.size(); ++i)
    {
        size_t index;
        double coord;
        auto lst = nos::split(args[i], ':');
        if (lst.size() != 2)
            throw std::runtime_error("invalid argument");
        auto axmnemo = lst[0];
        bool is_string_mnemo = isalpha(axmnemo[0]);
        index =
            is_string_mnemo ? axis_index_by_mnemo(axmnemo) : std::stoi(axmnemo);
        coord = std::stod(lst[1]);
        auto *axis = axis_pointer_by_igroup_index(index);
        indexes_and_coords.emplace_back(axis, coord);
    }
    return indexes_and_coords;
}

void InterpolationGroup_v2::command_absmove(const nos::argv &args)
{
    auto indexes_and_coords = to_indexes_and_coords(args);
    absmove_as_one_block(indexes_and_coords);
}

void InterpolationGroup_v2::command_absmove_by_blocks(const nos::argv &args)
{
    auto indexes_and_coords = to_indexes_and_coords(args);
    absmove_by_blocks(indexes_and_coords);
}

AbstractAxis *InterpolationGroup_v2::axis_pointer_by_igroup_index(size_t index)
{
    return axes[index];
}

void InterpolationGroup_v2::command(const nos::argv &args, nos::ostream &out)
{
    nos::println("InterpolationGroup_v2::command");
    auto cmd = args[0];

    if (cmd == "help")
    {
        nos::println_to(out, command_help());
        return;
    }

    if (cmd == "info")
    {
        nos::println_to(out, info());
        return;
    }

    if (cmd == "absmove")
    {
        command_absmove(args);
        return;
    }

    if (cmd == "absmove_blocks")
    {
        command_absmove_by_blocks(args);
        return;
    }

    if (cmd == "correction")
    {
        auto tr = correction_as_trent();
        std::string str = nos::format("{}", tr);
        nos::println_to(out, str);
        return;
    }

    if (cmd == "stop")
    {
        stop();
        return;
    }

    nos::println_to(out, "unknown command");
}

const std::vector<AbstractAxis *> &InterpolationGroup_v2::axis_list()
{
    return axes;
}

std::string InterpolationGroup_v2::command_help()
{
    return "info - show info\n"
           "help - show help\n"
           "absmove - move to absolute position\n"
           "absmove_blocks - move to absolute position with blocks mechanic "
           "applyed \n"
           "stop - stop interpolation\n";
}

std::string InterpolationGroup_v2::info()
{
    std::string out;
    out += "axes:\r\n";
    for (size_t i = 0; i < axes.size(); ++i)
    {
        out += nos::format("{}:{}", i, axes[i]->mnemo());
    }
    return out;
}

size_t
InterpolationGroup_v2::axis_index_by_mnemo(const std::string &axmnemo) const
{
    for (size_t i = 0; i < axes.size(); ++i)
    {
        if (axes[i]->mnemo() == axmnemo)
            return i;
    }
    return -1;
}

void InterpolationGroup_v2::set_correction_by_trent(const nos::trent &trent)
{
    correction_enabled = trent["enable"].as_bool_default(false);
    ncorrection = multi_axes_cartesian_correction::trent_to_correction(trent);
    save_correction_table();
}

void InterpolationGroup_v2::trajectory_move_by_trent(const nos::trent &trent)
{
    std::vector<ndmath::point> points;
    for (auto &pnt : trent["points"].as_list())
    {
        size_t dim = pnt.as_list().size();
        ndmath::point npnt(dim);
        for (size_t i = 0; i < dim; ++i)
            npnt[i] = pnt.as_list()[i].as_numer();
    }
    absmove_vectored(points);
}