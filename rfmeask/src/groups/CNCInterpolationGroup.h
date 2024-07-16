#ifndef CNC_INTERPOLATION_GROUP_H
#define CNC_INTERPOLATION_GROUP_H

#include <groups/InterpolationGroup.h>
#include <servos/AbstractServo.h>
#include <string>

class CNCInterpolationGroup : public InterpolationGroup_v2
{
public:
    CNCInterpolationGroup(const std::string &name) : InterpolationGroup_v2(name)
    {
    }

    std::vector<AbstractServo *> servos_for_axis(AbstractAxis *axis)
    {

        auto *common_axis = dynamic_cast<CommonAxis *>(axis);
        if (common_axis == nullptr)
            throw std::runtime_error(
                "RalgoCncInterpolationGroup: axis is not CommonAxis");
        auto *move_api =
            dynamic_cast<CNCTandemMovingClient *>(common_axis->move_api());
        if (move_api == nullptr)
            throw std::runtime_error("RalgoCncInterpolationGroup: axis "
                                     "move_api is not CNCMovingClient");
        return move_api->servos();
    }

    std::vector<std::vector<GrpServoMovePair>>
    multipnts_axis_pair_task_to_servo_pair_task(
        std::vector<std::vector<GrpAxisMovePair>> &tasks)
    {
        std::vector<std::vector<GrpServoMovePair>> ret;
        for (auto &task : tasks)
        {
            auto r = axis_pair_task_to_servo_pair_task(task);
            ret.push_back(r);
        }
        return ret;
    }

    std::vector<GrpServoMovePair>
    axis_pair_task_to_servo_pair_task(std::vector<GrpAxisMovePair> &task)
    {
        std::vector<GrpServoMovePair> r;
        for (auto &pair : task)
        {
            auto *ax = pair.axis;
            auto &pos = pair.value;
            for (auto *servo : servos_for_axis(ax))
            {
                r.push_back({servo, pos});
            }
        }
        return r;
    }

    void update_cnc_position()
    {
        for (auto axis : axes)
        {
            auto *common_axis = dynamic_cast<CommonAxis *>(axis);
            auto *move_api =
                dynamic_cast<CNCTandemMovingClient *>(common_axis->move_api());
            move_api->update_cnc_positions();
        }
    }

    static double
    evaluate_external_accfeed_2(const std::vector<double> &direction,
                                double absolute_maximum,
                                const std::vector<double> &element_maximums)
    {
        if (ralgo::vecops::norm(element_maximums) == 0)
        {
            return absolute_maximum;
        }

        if (absolute_maximum == 0)
        {
            auto bounded = ralgo::vecops::ray_to_box<std::vector<double>>(
                direction, element_maximums);
            return ralgo::vecops::norm(bounded);
        }
        else
        {
            auto vec = ralgo::vecops::mul_vs(direction, absolute_maximum);
            auto bounded = ralgo::vecops::bound_to_box<std::vector<double>>(
                vec, element_maximums);
            return ralgo::vecops::norm(bounded);
        }
    }

    virtual void set_pause_planning(bool en) = 0;

    void absmove_multiblocks_servo_task(
        const std::vector<std::vector<GrpServoMovePair>> &task)
    {
        multiblocks_mutex.lock();
        update_cnc_position();

        _prevent_multiblocks_sending = false;
        setup_axes_accelvelocities();

        set_pause_planning(true);
        for (auto &block : task)
        {
            multiblocks_mutex.unlock();
            bool prevent = _prevent_multiblocks_sending;
            nos::println("is prevented?", prevent);
            if (prevent)
            {
                nos::println("absmove_multiblocks_servo_task prevented");
                break;
            }
            multiblocks_mutex.lock();
            evaluate_servo_task(block);
        }
        set_pause_planning(false);
        multiblocks_mutex.unlock();
    }

    void absmove_servo_task(const std::vector<GrpServoMovePair> &task)
    {
        update_cnc_position();

        setup_axes_accelvelocities();
        evaluate_servo_task(task);
    }

    virtual void
    evaluate_servo_task(const std::vector<GrpServoMovePair> &task) = 0;

    CNCTandemMovingClient *cnc_moveapi_from_common_axis(CommonAxis *axis)
    {
        auto *moveapi = dynamic_cast<CNCTandemMovingClient *>(axis->move_api());
        if (!moveapi)
            throw std::runtime_error(
                "RalgoCncInterpolationGroup: axis moveapi is "
                "not CNCTandemMovingClient");
        return moveapi;
    }

    std::vector<GrpServoMovePair>
    axis_pair_task_to_servo_pair_task(const std::vector<GrpAxisMovePair> &task)
    {
        std::vector<GrpServoMovePair> out;
        for (auto &t : task)
        {
            auto *axis = dynamic_cast<CommonAxis *>(t.axis);
            auto *moveapi = cnc_moveapi_from_common_axis(axis);
            auto servos_and_symbols = moveapi->servos_and_symbols();
            for (auto &[servo, symbol] : servos_and_symbols)
            {
                out.emplace_back(servo, t.value);
            }
        }
        return out;
    }

    std::vector<ndmath::point> root_points_from_to(const ndmath::point &from,
                                                   const ndmath::point &to)
    {
        return ncorrection->line_and_grid_collisions(from, to, true);
    }

    void
    start_block_sending_thread(const std::vector<std::vector<GrpServoMovePair>>
                                   &corrected_servo_task_blocks)
    {

        if (multiblocks_sending_thread.joinable())
            multiblocks_sending_thread.join();
        multiblocks_sending_thread =
            std::thread([this, corrected_servo_task_blocks]() {
                absmove_multiblocks_servo_task(corrected_servo_task_blocks);
            });
    }

    void notify_axes_about_operation_start(
        const std::vector<GrpAxisMovePair> &axis_pair_task)
    {
        nos::println("NOTIFY ABOUT OPERATION START");
        for (auto p : axis_pair_task)
        {
            p.axis->start_operation_notify();
        }
    }

    void absmove_by_blocks(const std::vector<GrpAxisMovePair> &axis_pair_task)
    {
        auto current_position = last_position_as_dict();
        auto merged_target_position =
            merge_position_dictionary(current_position, axis_pair_task);

        auto points = root_points_from_to(
            to_ordered_vector_as_ndpoint(current_position),
            to_ordered_vector_as_ndpoint(merged_target_position));

        auto corrected_points = correct_points_if_correction_enabled(points);
        auto corrected_axtask =
            corrected_points_to_axis_task_with_reverse_applying(
                corrected_points);
        auto corrected_servo_task_blocks =
            multipnts_axis_pair_task_to_servo_pair_task(corrected_axtask);

        // notify_axes_about_operation_start(axis_pair_task);
        start_block_sending_thread(corrected_servo_task_blocks);
    }

    void absmove_vectored(const std::vector<ndmath::point> &task) override
    {
        auto corrected_axtask =
            corrected_points_to_axis_task_with_reverse_applying(task);
        auto corrected_servo_task_blocks =
            multipnts_axis_pair_task_to_servo_pair_task(corrected_axtask);

        start_block_sending_thread(corrected_servo_task_blocks);
    }

    void absmove_as_one_block(
        const std::vector<GrpAxisMovePair> &axis_pair_task) override
    {
        auto current_position = last_position_uncorrected_as_dict();
        auto target_position = current_position;
        auto merged_target_position =
            merge_position_dictionary(target_position, axis_pair_task);

        auto curpos_vec = to_ordered_vector_as_ndpoint(current_position);
        auto target_vec = to_ordered_vector_as_ndpoint(merged_target_position);

        auto point = target_vec;
        auto correct_point = correct_point_if_correction_enabled(point);

        auto correct_axtask =
            correct_point_to_axis_task_with_reverse_applying(correct_point);
        auto correct_servo_task_block =
            axis_pair_task_to_servo_pair_task(correct_axtask);

        absmove_servo_task(correct_servo_task_block);
    }
};

#endif
