/**
    @file
*/

#ifndef RFMEAS_INTERPOLATION_GROUP
#define RFMEAS_INTERPOLATION_GROUP

#include <AbstractAxis.h>
#include <LogicalController.h>
#include <LogicalDevice.h>
#include <ndmath/named_cartesian_correction.h>
#include <nos/shell/argv.h>
#include <optional>
#include <ralgo/linalg/linalg.h>
#include <ranges>
#include <servos/AbstractServo.h>
#include <utilxx/status.h>
#include <vector>

class AbstractAxis;
class InterpolationGroup_v2;

InterpolationGroup_v2 *get_interpolation_group(const std::string &groupname);
InterpolationGroup_v2 *get_interpolation_group(int intgroupno);

struct GrpServoMovePair
{
    AbstractServo *servo = 0;
    double value = 0;
    GrpServoMovePair(AbstractServo *_servo, double value) :
        servo(_servo), value(value)
    {
    }
    nos::expected<size_t, nos::output_error> print_to(nos::ostream &os) const
    {
        return nos::fprint_to(os, "{}:{}", servo->name(), value);
    }
};

struct GrpAxisMovePair
{
    AbstractAxis *axis = 0;
    double value = 0;
    GrpAxisMovePair(AbstractAxis *axis, double value) : axis(axis), value(value)
    {
    }

    nos::expected<size_t, nos::output_error> print_to(nos::ostream &os) const
    {
        return nos::fprint_to(os, "{}:{}", axis->name(), value);
    }
};

/**
    Класс - интерфейс интерполяционной группы.
    Работает строго с user единицами.
*/
class InterpolationGroup_v2 : public LogicalDevice, public LogicalController
{
protected:
    std::vector<AbstractAxis *> axes = {};
    std::vector<Operation> opstatus_array = {};
    Operation last_operation_status = Operation::NONE;

    std::optional<ndmath::cartesian_correction> ncorrection = {};
    bool correction_enabled = false;

    ndmath::vector _last_position;
    ndmath::vector _last_position_noncorrected;

    std::mutex multiblocks_mutex;
    std::thread multiblocks_sending_thread;
    volatile bool _prevent_multiblocks_sending = false;

private:
    void operation_status_analyze();

public:
    Operation operation_status();
    void on_axis_operation_status(AbstractAxis *axis, Operation operation);

    AbstractAxis *axis_pointer_by_igroup_index(size_t index);

    nos::trent correction_as_trent();
    void set_correction_by_trent(const nos::trent &trent);
    void trajectory_move_by_trent(const nos::trent &trent);

    void prevent_multiblocks_sending()
    {
        nos::println("prevent_multiblocks_sending");
        std::lock_guard<std::mutex> lock(multiblocks_mutex);
        _prevent_multiblocks_sending = true;
        if (multiblocks_sending_thread.joinable())
            multiblocks_sending_thread.join();

        nos::println("prevent_multiblocks_sending unjoining");
    }

    virtual void stop() = 0;

    InterpolationGroup_v2(const std::string &name) :
        LogicalDevice(name, "group")
    {
    }
    void load_correction_table();
    void save_correction_table();
    void init_correction_table();

    size_t index_of(AbstractAxis *ax);

    double update_position_and_reverse_correct_unit_position(AbstractAxis *ax,
                                                             double pos);

    int incmove_command(std::vector<double> vec)
    {
        return incmove(vec);
    }

    bool is_correction_enabled()
    {
        return correction_enabled;
    }

    int absmove_command(std::vector<double> vec)
    {
        return absmove(vec);
    }

    std::unordered_map<AbstractAxis *, double>
    last_position_uncorrected_as_dict()
    {
        std::unordered_map<AbstractAxis *, double> ret(axes.size());
        for (size_t idx = 0; idx < axes.size(); ++idx)
        {
            auto ax = axes[idx];
            double lpos = _last_position_noncorrected[idx];
            ret.emplace(ax, lpos);
        }
        return ret;
    }

    std::unordered_map<AbstractAxis *, double> last_position_as_dict()
    {
        std::unordered_map<AbstractAxis *, double> ret(axes.size());
        for (size_t idx = 0; idx < axes.size(); ++idx)
        {
            auto ax = axes[idx];
            double lpos = _last_position[idx];
            ret.emplace(ax, lpos);
        }
        return ret;
    }

    std::unordered_map<AbstractAxis *, double> merge_position_dictionary(
        const std::unordered_map<AbstractAxis *, double> &dict,
        const std::vector<GrpAxisMovePair> &dict2)
    {
        std::unordered_map<AbstractAxis *, double> ret = dict;
        for (auto &[key, value] : dict2)
        {
            ret[key] = value;
        }
        return ret;
    }

    ndmath::point to_ordered_vector_as_ndpoint(
        const std::unordered_map<AbstractAxis *, double> &dict)
    {
        ndmath::point ret;
        for (auto *ax : axes)
        {
            if (dict.find(ax) == dict.end())
                throw std::runtime_error(
                    "RalgoCncInterpolationGroup: axis not found in dict");

            ret.push_back(dict.at(ax));
        }
        return ret;
    }

    virtual void
    absmove_by_blocks(const std::vector<GrpAxisMovePair> &axis_pair_task) = 0;

    virtual void absmove_vectored(const std::vector<ndmath::point> &task) = 0;

    std::vector<ndmath::point>
    points_to_task(const std::vector<std::vector<double>> &pnts)
    {
        std::vector<ndmath::point> result;
        for (auto &p : pnts)
        {
            ndmath::point npnt((double *)p.data(), p.size());
            result.emplace_back(npnt);
        }
        return result;
    }

    void absmove_command(const std::vector<std::vector<double>> &pnts)
    {
        auto task = points_to_task(pnts);
        absmove_vectored(task);
    }

    virtual void absmove_as_one_block(
        const std::vector<GrpAxisMovePair> &axis_pair_task) = 0;

    std::vector<std::pair<AbstractAxis *, double>> unit_speed_per_second_list()
    {
        auto ret = std::views::transform(
            axes, [](auto *ax) { return std::pair(ax, ax->getSpeed_ups()); });
        return {ret.begin(), ret.end()};
    }

    std::vector<std::pair<AbstractAxis *, double>>
    unit_accel_per_second_sqr_list()
    {
        auto ret = std::views::transform(axes, [](auto *ax) {
            return std::pair<AbstractAxis *, double>(ax, ax->getAccel_ups2());
        });
        return {ret.begin(), ret.end()};
    }

    virtual void setup_axes_accelvelocities() = 0;

    std::vector<ndmath::point>
    correct_points_if_correction_enabled(std::vector<ndmath::point> &points)
    {
        auto &correction = *ncorrection;
        std::vector<ndmath::point> ret = points;
        if (correction_enabled)
            ret = correction.corrected_points(points);
        return ret;
    }

    std::vector<GrpAxisMovePair>
    correct_point_to_axis_task_with_reverse_applying(const ndmath::point &point)
    {
        std::vector<GrpAxisMovePair> task;
        for (size_t j = 0; j < axes.size(); j++)
        {
            auto *axis = axes[j];
            auto val = axis->is_reversed() ? -point[j] : point[j];
            task.push_back({axis, val});
        }
        return task;
    }

    std::vector<std::vector<GrpAxisMovePair>>
    corrected_points_to_axis_task_with_reverse_applying(
        const std::vector<ndmath::point> &points)
    {
        auto ret =
            std::views::transform(points, [&](const ndmath::point &point) {
                return correct_point_to_axis_task_with_reverse_applying(point);
            });
        return {ret.begin(), ret.end()};
    }

    ndmath::point correct_point_if_correction_enabled(ndmath::point &point)
    {
        auto &correction = *ncorrection;
        auto ret = point;
        if (correction_enabled)
            ret = correction.corrected_point(point);
        return ret;
    }

    virtual void
    absmove_without_correction(const std::vector<GrpAxisMovePair> &task) = 0;

    void absmove(const std::vector<GrpAxisMovePair> &task)
    {
        if (correction_enabled)
            absmove_by_blocks(task);
        else
            absmove_as_one_block(task);
    }

    std::vector<GrpAxisMovePair> to_indexes_and_coords(const nos::argv &args);
    void command_absmove_by_blocks(const nos::argv &args);

    void set_axes(const std::vector<AbstractAxis *> &ref);

    std::pair<double, double> parted_limits(int idx);
    std::pair<double, double> parted_limits(const AbstractAxis *ax);

    std::vector<int> get_axes_indexes()
    {
        auto view = std::views::transform(
            axes, [](auto *ax) { return ax->scpi_api_index(); });
        return {view.begin(), view.end()};
    }

    virtual void preset() = 0;
    virtual double speed() = 0;
    virtual double acceleration() = 0;

    virtual double setted_accel_ups2() = 0;
    virtual double setted_speed_ups() = 0;

    [[deprecated]] int set_speed_ups(double spd)
    {
        set_common_speed_ups(spd);
        return 0;
    }

    [[deprecated]] virtual int set_accel_ups2(double acc)
    {
        set_common_accel_ups2(acc);
        return 0;
    }

    [[deprecated]] virtual int set_decel_ups2(double dcc)
    {
        set_common_decel_ups2(dcc);
        return 0;
    }

    const std::vector<AbstractAxis *> &axis_list();

    virtual int incmove(std::vector<double> vec) = 0;
    virtual int absmove(std::vector<double> vec)
    {
        std::vector<GrpAxisMovePair> indexes_and_coords;
        for (size_t i = 0; i < axes.size(); ++i)
        {
            indexes_and_coords.emplace_back(GrpAxisMovePair{axes[i], vec[i]});
        }
        absmove(indexes_and_coords);
        return 0;
    }

    int incmove_to_border(std::vector<double> vec);
    int absmove_to_border(std::vector<double> vec);

    // Установить вектора скоростей/ускорений для всего множества осей
    // интерполяционной группы.
    virtual void set_speeds_ups(const std::vector<double> &spds);
    virtual void set_accels_ups(const std::vector<double> &accs);
    virtual void set_decels_ups(const std::vector<double> &dccs);

    // Установить скорости/ускорения перемещения по осям. Изделие будет
    // перемещаться сообрано ограничению скорости мажорной оси заданного
    // перемещения.
    virtual void set_speeds_ups_parted(const std::vector<int> &indexes,
                                       const std::vector<double> &spds)
    {
        BUG();
    }
    virtual void set_accels_ups2_parted(const std::vector<int> &indexes,
                                        const std::vector<double> accs) &
    {
        BUG();
    }
    virtual void set_decels_ups2_parted(const std::vector<int> &indexes,
                                        const std::vector<double> dccs) &
    {
        BUG();
    }

    void command_absmove(const nos::argv &args);

    // Установить взвешенную скорость/ускорение перемещения по всей совокупности
    // осей.
    virtual void set_common_speed_ups(double) = 0;
    virtual void set_common_accel_ups2(double) = 0;
    virtual void set_common_decel_ups2(double) = 0;
    virtual int dim() = 0;

    size_t axis_dim()
    {
        return axes.size();
    }

    const ndmath::vector &last_position_uncorrected()
    {
        return _last_position_noncorrected;
    }

    std::string idn()
    {
        return mnemo();
    }

    void absmove_parted(const std::vector<std::pair<int, double>> &args);

    std::vector<linalg::vec<double, 2>> realtime_border2d();

    void move_parted(AbstractAxis *ax, double arg);

    virtual void command(const nos::argv &args, nos::ostream &out);
    virtual std::string command_help();
    virtual std::string info();

    size_t axis_index_by_mnemo(const std::string &axmnemo) const;
};

InterpolationGroup_v2 *find_group(const std::string &name);
void add_interpolation_group(InterpolationGroup_v2 *igroup);
void init_interpolation_groups();

#endif
