#ifndef RALGO_CNC_FEEDBACK_H
#define RALGO_CNC_FEEDBACK_H

#include <algorithm>
#include <igris/container/span.h>
#include <igris/container/static_vector.h>
#include <igris/event/delegate.h>
#include <nos/shell/executor.h>
#include <nos/util/string.h>
#include <ralgo/cnc/planner.h>
#include <ralgo/cnc/util.h>

const constexpr size_t MAXIMUM_TANDEM_MISTAKE = 256000;

namespace cnc
{
    class feedback_guard_tandem
    {
        igris::static_vector<size_t, NMAX_AXES> _nums = {};
        igris::static_vector<int64_t, NMAX_AXES> _multipliers = {};
        cnc_float_type _maximum_tandem_mistake;

    public:
        bool in_operation = false;

    public:
        feedback_guard_tandem(igris::static_vector<size_t, NMAX_AXES> nums,
                              igris::static_vector<int64_t, NMAX_AXES> muls,
                              cnc_float_type mistake)
            : _nums(nums), _multipliers(muls), _maximum_tandem_mistake(mistake)
        {
            assert(_nums.size() == _multipliers.size());
        }

        feedback_guard_tandem(const std::vector<size_t> &nums,
                              const std::vector<int64_t> &muls,
                              cnc_float_type mistake)
            : _nums(nums.begin(), nums.end()),
              _multipliers(muls.begin(), muls.end()),
              _maximum_tandem_mistake(mistake)
        {
            assert(_nums.size() == _multipliers.size());
        }

        feedback_guard_tandem &
        operator=(const feedback_guard_tandem &) = default;
        feedback_guard_tandem &operator=(feedback_guard_tandem &&) = default;
        feedback_guard_tandem(const feedback_guard_tandem &) = default;
        feedback_guard_tandem(feedback_guard_tandem &&) = default;

        cnc_float_type maximum_tandem_mistake() const
        {
            return _maximum_tandem_mistake;
        }

        const igris::static_vector<size_t, NMAX_AXES> &nums() const
        {
            return _nums;
        }

        const igris::static_vector<int64_t, NMAX_AXES> &muls() const
        {
            return _multipliers;
        }

        std::string info() const
        {
            return nos::format("{}: muls:{} max_mistake:{}",
                               nos::ilist(_nums),
                               nos::ilist(_multipliers),
                               _maximum_tandem_mistake);
        }
    };

    class feedback_guard
    {
    private:
        igris::delegate<void, igris::span<int64_t>>
            _set_feedback_position_callback;

        igris::delegate<void, size_t, int64_t>
            _set_feedback_position_by_axis_callback;

        igris::static_vector<feedback_guard_tandem, NMAX_AXES> _tandems = {};

        std::array<cnc_float_type, NMAX_AXES> feedback_to_drive = {};
        std::array<cnc_float_type, NMAX_AXES> control_to_drive =
            {}; //< этот массив равен gears

        // максимальное значение drop_pulses, после которого вызывается
        // planner->alarm_stop()
        std::array<cnc_float_type, NMAX_AXES> maximum_drop_pulses = {};
        size_t total_axes;

    public:
        feedback_guard(size_t total_axes) : total_axes(total_axes)
        {
            cnc_float_type default_max_drop = 6000000;
            for (size_t i = 0; i < NMAX_AXES; ++i)
            {
                maximum_drop_pulses[i] = default_max_drop;
                feedback_to_drive[i] = 1;
                control_to_drive[i] = 1;
            }
        }

        std::string guard_info()
        {
            return nos::format("feed_to_drive: {}\r\nctrl_to_drive: {}\r\n",
                               nos::ilist(feedback_to_drive),
                               nos::ilist(control_to_drive));
        }

        int guard_info(nos::ostream &os)
        {
            nos::println_to(os, guard_info());
            return 0;
        }

        const auto &tandems() const
        {
            return _tandems;
        }

        void set_set_feedback_position_by_axis_callback(
            igris::delegate<void, size_t, int64_t> dlg)
        {
            _set_feedback_position_by_axis_callback = dlg;
        }

        void set_feedback_to_drive_multiplier(igris::span<cnc_float_type> mult)
        {
            std::copy(mult.begin(), mult.end(), feedback_to_drive.begin());
        }

        void set_control_to_drive_multiplier(igris::span<cnc_float_type> mult)
        {
            std::copy(mult.begin(), mult.end(), control_to_drive.begin());
        }

        void set_feedback_to_drive_multiplier(int axno, cnc_float_type mult)
        {
            feedback_to_drive[axno] = mult;
        }

        void set_control_to_drive_multiplier(int axno, cnc_float_type mult)
        {
            control_to_drive[axno] = mult;
        }

        void set_maximum_drop_pulses(igris::span<cnc_float_type> max_drop)
        {
            std::copy(
                max_drop.begin(), max_drop.end(), maximum_drop_pulses.begin());
        }

        void set_feedback_position(igris::span<int64_t> pos)
        {
            std::array<int64_t, NMAX_AXES> fpos;
            for (size_t i = 0; i < pos.size(); ++i)
                fpos[i] = static_cast<int64_t>(pos[i] / feedback_to_drive[i]);
            _set_feedback_position_callback(igris::span(fpos));
        }

        void set_feedback_position(size_t axno, int64_t val)
        {
            int64_t fpos = static_cast<int64_t>(val / feedback_to_drive[axno]);
            _set_feedback_position_by_axis_callback(axno, fpos);
        }

        int64_t drop_pulses_allowed(int no)
        {
            return maximum_drop_pulses[no];
        }

        void set_drop_pulses_allowed(int no, int64_t val)
        {
            maximum_drop_pulses[no] = val;
        }

        // int64_t tandem_allowed_diff()
        //{
        //    return maximum_tandem_mistake;
        //}

        // void set_tandem_allowed_diff(int64_t val)
        //{
        //    maximum_tandem_mistake = val;
        //}

        bool verify_position(igris::span<int64_t> feedback_position,
                             igris::span<int64_t> control_position)
        {
            for (size_t i = 0; i < total_axes; ++i)
            {
                int64_t drop_pulses =
                    feedback_position[i] * feedback_to_drive[i] -
                    control_position[i] * control_to_drive[i];

                if (std::abs(drop_pulses) > maximum_drop_pulses[i])
                {
                    return false;
                }
            }
            return true;
        }

        std::vector<int64_t>
        feedback_position_as_drive(igris::span<int64_t> feedback_position)
        {
            std::vector<int64_t> vec;
            vec.resize(total_axes);
            for (size_t i = 0; i < total_axes; ++i)
            {
                vec[i] = feedback_position[i] * feedback_to_drive[i];
            }
            return vec;
        }

        std::vector<int64_t>
        control_position_as_drive(igris::span<int64_t> control_position)
        {
            std::vector<int64_t> vec;
            vec.resize(total_axes);
            for (size_t i = 0; i < total_axes; ++i)
            {
                vec[i] = control_position[i] * control_to_drive[i];
            }
            return vec;
        }

        bool verify_position_in_drives(
            igris::span<int64_t> feedback_position_as_drive,
            igris::span<int64_t> control_position_as_drive)
        {
            for (size_t i = 0; i < total_axes; ++i)
            {
                int64_t drop_pulses = feedback_position_as_drive[i] -
                                      control_position_as_drive[i];

                if (std::abs(drop_pulses) > maximum_drop_pulses[i])
                {
                    return false;
                }
            }
            return true;
        }

        void enable_tandem_protection_for_axis(size_t axno)
        {
            for (auto &tandem : _tandems)
            {
                for (auto num : tandem.nums())
                {
                    if (num == axno)
                        tandem.in_operation = true;
                }
            }
        }

        void finish_operations()
        {
            for (auto &tandem : _tandems)
            {
                tandem.in_operation = false;
            }
        }

        bool verify_tandems(igris::span<int64_t> feedback_position)
        {
            for (auto &tandem : _tandems)
            {
                if (tandem.in_operation == false)
                    continue;

                size_t reference_index = tandem.nums()[0];
                auto reference_mul = tandem.muls()[0];
                auto reference_pos = feedback_position[reference_index];
                auto reference_to = feedback_to_drive[reference_index];
                cnc_float_type reference_drive =
                    reference_pos * reference_mul * reference_to;
                for (size_t i = 1; i < tandem.nums().size(); ++i)
                {
                    size_t it_index = tandem.nums()[i];
                    auto it_mul = tandem.muls()[i];
                    auto it_pos = feedback_position[it_index];
                    auto it_to = feedback_to_drive[it_index];

                    cnc_float_type it_drive = it_pos * it_mul * it_to;
                    cnc_float_type diff = it_drive - reference_drive;
                    if (std::abs(diff) > tandem.maximum_tandem_mistake())
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        void add_tandem(const std::vector<size_t> &tandem,
                        const std::vector<int64_t> &muls,
                        cnc_float_type max_mistake)
        {
            _tandems.emplace_back(tandem, muls, max_mistake);
        }

        void add_tandem(const feedback_guard_tandem &tandem)
        {
            _tandems.push_back(tandem);
        }

        void remove_tandem(size_t idx)
        {
            _tandems.erase(
                std::remove_if(_tandems.begin(),
                               _tandems.end(),
                               [idx](const cnc::feedback_guard_tandem &v) {
                                   return std::count(
                                       v.nums().begin(), v.nums().end(), idx);
                               }),
                _tandems.end());
        }

        void add_tandem_command(const nos::argv &argv, nos::ostream &os)
        {
            cnc_float_type mistake = MAXIMUM_TANDEM_MISTAKE;
            std::vector<size_t> nums;
            std::vector<int64_t> muls;

            for (auto &arg : argv)
            {
                auto lst = nos::split(arg, ':');

                std::string name_or_index_or_command = lst[0];
                std::string multiplier = lst.size() > 1 ? lst[1] : "1";

                if (name_or_index_or_command == "maxmistake")
                {
                    if (lst.size() != 2)
                    {
                        nos::println_to(os, "'maxmistake' argument required");
                        return;
                    }

                    mistake = std::stoi(multiplier);
                    continue;
                }

                else
                {
                    size_t index;
                    if (isdigit(name_or_index_or_command[0]))
                    {
                        index = std::stoi(name_or_index_or_command);
                    }
                    else
                    {
                        index = symbol_to_index(name_or_index_or_command[0]);
                    }
                    // assert(index == 0 || index == 1 || index == 2 ||
                    //       index == 4);
                    nums.push_back(index);

                    if (lst.size() == 1)
                    {
                        muls.push_back(1);
                    }
                    else if (lst.size() == 2)
                    {
                        muls.push_back(std::stoi(multiplier));
                    }
                    else
                    {
                        nos::println_to(os, "too many arguments");
                        return;
                    }
                }
            }

            system_lock();
            for (auto n : nums)
                remove_tandem(n);
            add_tandem(nums, muls, mistake);
            system_unlock();
        }
    };
}

#endif
