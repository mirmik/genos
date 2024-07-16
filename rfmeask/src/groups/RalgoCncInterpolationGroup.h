#ifndef RALGO_CNC_INTERPOLATION_GROUP_H
#define RALGO_CNC_INTERPOLATION_GROUP_H

#include <AbstractAxis.h>
#include <moveapi/CNCMovingClient.h>
#include <RalgoCNC.h>
#include <groups/CNCInterpolationGroup.h>

class RalgoCNC;
class CNCTandemMovingClient;
class RalgoCncInterpolationGroup : public CNCInterpolationGroup
{
    double _speed_ups = 1;
    double _accel_ups2 = 1;
    double &_decel_ups2 = _accel_ups2;

    RalgoCNC *cnc;
    std::unordered_map<AbstractServo *, std::string> servo_symbols;
    std::vector<AbstractServo *> servos;

public:
    RalgoCncInterpolationGroup(const std::string &name, RalgoCNC *cnc) :
        CNCInterpolationGroup(name)
    {
        set_driver(cnc);
    }

    void on_ready_status_changed(bool ready) override;
    std::string number_in_task_to_symbol(int number);

    void init();
    std::unordered_map<AbstractServo *, std::string>
    collect_symbols(std::vector<CNCTandemMovingClient *> clients);
    bool check_axes_type(std::vector<AbstractAxis *> axes);
    void load_correction_table();

    void set_driver(RalgoCNC *cnc)
    {
        this->cnc = cnc;
    }

    void preset() override;
    double speed() override;
    double acceleration() override;
    int dim();

    std::vector<double> axis_pair_task_to_uspd_list(
        const std::vector<GrpAxisMovePair> &axis_pair_task);

    ndmath::point to_ordered_vector_as_ndpoint(
        const std::unordered_map<AbstractAxis *, double> &dict);
    std::unordered_map<AbstractAxis *, double> current_position_as_dict();

    void
    evaluate_servo_task(const std::vector<GrpServoMovePair> &task) override;

    int incmove(std::vector<double> vec) override;
    // int absmove(std::vector<double> vec) override;
    void absmove_with_correction(std::vector<double> vec);

    double setted_speed_ups() override;
    double setted_accel_ups2() override;

    void set_common_speed_ups(double spd) override;
    void set_common_accel_ups2(double acc) override;
    void set_common_decel_ups2(double dcc) override;

    void set_pause_planning(bool en) override;

    void setup_axes_accelvelocities()
    {
        assert(cnc != nullptr);
        std::vector<std::pair<std::string, double>> speeds;
        std::vector<std::pair<std::string, double>> accels;

        for (auto &axis : axes)
        {
            auto axmoveapi =
                cnc_moveapi_from_common_axis(dynamic_cast<CommonAxis *>(axis));
            auto axspeed = axmoveapi->current_speed();
            auto axaccel = axmoveapi->current_accel();
            for (auto &servo : axmoveapi->servos())
            {
                auto symbol = symbol_for_servo(servo);
                speeds.emplace_back(symbol, axspeed.internalpulse_per_second());
                accels.emplace_back(symbol,
                                    axaccel.internalpulse_per_second_sqr());
            }
        }
        nos::println("speeds:", speeds);
        nos::println("accels:", accels);
        cnc->set_max_velocities(speeds);
        cnc->set_max_accelerations(accels);
    }

    std::vector<std::pair<std::string, double>>
    axis_accelspeed_list_to_symbol_geared_accelspeed_list(
        std::vector<std::pair<AbstractAxis *, double>> pairs)
    {
        std::vector<std::pair<std::string, double>> ret;
        for (auto &pair : pairs)
        {
            CommonAxis *axis = dynamic_cast<CommonAxis *>(pair.first);
            auto accelspeed = pair.second;
            auto gain = axis->unitRatio();
            auto *moveapi = cnc_moveapi_from_common_axis(axis);
            for (auto &servo : moveapi->servos())
            {
                auto gearmul = servo->electronic_gear() / 1000;
                auto symbol = symbol_for_servo(servo);
                ret.push_back({symbol, accelspeed * gain / gearmul});
            }
        }
        return ret;
    }

    void set_speeds_ups_parted(const std::vector<int> &indexes,
                               const std::vector<double> &speed) override;

    void absmove_without_correction(
        const std::vector<GrpAxisMovePair> &task) override;

    void absmove(const std::vector<GrpAxisMovePair> &task);
    void absmove_servo_task(const std::vector<GrpServoMovePair> &task);

    void stop() override;
    ndmath::vector current_position();

    CommonAxis *axis_by_index(size_t index);

    void command(const nos::argv &args, nos::ostream &out) override;
    std::string command_help() override;
    std::string info() override;

    std::string declared_type() const override
    {
        return "ralgo_cnc_interpolation_group";
    }

    AbstractServo *servo_by_servo_index(size_t index);
    std::vector<AbstractServo *>
    servos_for_servo_pair_task(const std::vector<GrpServoMovePair> &task);

    std::string symbol_for_servo(AbstractServo *servo);
    const std::vector<AbstractServo *> &servos_list();

    std::vector<std::pair<std::string, double>>
    axis_speed_list_to_symbol_speed_list(
        std::vector<std::pair<AbstractAxis *, double>> pairs);

    std::vector<std::pair<AbstractServo *, double>>
    all_servos_max_velocities_native_per_second();
    std::vector<std::pair<AbstractServo *, double>>
    all_servos_max_velocities_ups();
};

#endif