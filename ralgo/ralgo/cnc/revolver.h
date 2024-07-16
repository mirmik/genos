#ifndef RALGO_REVOLVER_H
#define RALGO_REVOLVER_H

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <igris/container/ring.h>
#include <igris/event/delegate.h>
#include <igris/sync/syslock.h>
#include <ralgo/cnc/defs.h>
#include <ralgo/cnc/planblock.h>
#include <ralgo/cnc/shift.h>
#include <ralgo/log.h>
#include <ralgo/robo/stepper.h>

namespace cnc
{

    constexpr int64_t FIXED_POINT_MUL = 1600000000;

    class revolver_task
    {
    public:
        // std::array<cnc_float_type, NMAX_AXES> accelerations;
        // std::array<cnc_float_type, NMAX_AXES> accelerations_mul05;
        std::array<int32_t, NMAX_AXES> accelerations_fixed;
        int32_t counter;

    public:
        revolver_task() = default;
        revolver_task(const std::array<cnc_float_type, NMAX_AXES> &accel,
                      int32_t counter)
            : counter(counter)
        {
            for (int i = 0; i < NMAX_AXES; ++i)
            {
                // accelerations_mul05[i] = accelerations[i] * 0.5;
                accelerations_fixed[i] = accel[i] * FIXED_POINT_MUL;
            }
        }
    };

    /**
     * Structure of revolver_task ring:
     *
     *              blocks.tail()                     (place for new shift)
     *                    |                                   |
     *                    |                                   |
     *                    v                                   v
     *      ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
     *     |     |     |     |     |     |     |     |     |     |     |     |
     *     |     |     |  .  |  .  |  .  |  .  |  .  |  .  |     |     |     |
     *     |     |     |     |     |     |     |     |     |     |     |     |
     *      ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
     *                    ^                                   ^
     *                    |                                   |
     * counters:       (tail)-->                           (head)-->
     * */
    class revolver
    {
    public:
        int steppers_total = 0;
        int32_t counter = 0;

    public:
        igris::ring<cnc::revolver_task> revolver_task_ring = {};
        robo::stepper **steppers = nullptr;

        cnc::revolver_task *current_revolver_task = {};
        control_shift zero_step = {};

        // std::array<cnc_float_type, NMAX_AXES> velocities = {};
        // std::array<cnc_float_type, NMAX_AXES> dda_counters = {};

        std::array<int64_t, NMAX_AXES> velocities_fixed = {};
        std::array<int64_t, NMAX_AXES> dda_counters_fixed = {};

        // std::array<cnc_float_type, NMAX_AXES> gears = {};
        // std::array<cnc_float_type, NMAX_AXES> gears_high_trigger = {};

        std::array<int64_t, NMAX_AXES> gears_fixed = {};
        std::array<int64_t, NMAX_AXES> gears_high_trigger_fixed = {};

    public:
        void change_task();

        void update_triggers()
        {
            for (unsigned int i = 0; i < gears_fixed.size(); ++i)
            {
                // gears_high_trigger[i] = gears[i] * 0.9;
                gears_high_trigger_fixed[i] = gears_fixed[i] * 0.9;
            }
        }

        void gears_fill(double val)
        {
            int64_t ival = val * FIXED_POINT_MUL;
            for (unsigned int i = 0; i < gears_fixed.size(); ++i)
            {
                gears_fixed[i] = ival;
            }
        }

        void gears_resize(size_t total)
        {
            (void)total;
        }

        double get_gear(int i)
        {
            return (double)gears_fixed[i] / FIXED_POINT_MUL;
        }

        void set_gear(int i, double val)
        {
            // gears[i] = val;
            // gears_high_trigger[i] = val * 0.9;

            gears_fixed[i] = val * FIXED_POINT_MUL;
            gears_high_trigger_fixed[i] = gears_fixed[i] * 0.9;
        }

        std::array<cnc_float_type, NMAX_AXES> get_gears()
        {
            std::array<cnc_float_type, NMAX_AXES> gears;
            for (unsigned int i = 0; i < gears.size(); ++i)
            {
                gears[i] = (double)gears_fixed[i] / FIXED_POINT_MUL;
            }
            return gears;
        }

        std::array<cnc_float_type, NMAX_AXES> current_velocities() const
        {
            // return velocities;
            std::array<cnc_float_type, NMAX_AXES> velocities;
            for (unsigned int i = 0; i < velocities.size(); ++i)
            {
                velocities[i] = (double)velocities_fixed[i] / FIXED_POINT_MUL;
            }
            return velocities;
        }

        void set_gears(const double *gears, int size)
        {
            for (int i = 0; i < size; ++i)
            {
                this->gears_fixed[i] = gears[i] * FIXED_POINT_MUL;
            }

            // std::copy(gears, gears + size, this->gears.begin());
            update_triggers();
        }

        void set_gears_high_trigger(double *gears_high_trigger, int size)
        {
            // std::copy(gears_high_trigger,
            //          gears_high_trigger + size,
            //          this->gears_high_trigger.begin());

            for (int i = 0; i < size; ++i)
            {
                this->gears_high_trigger_fixed[i] =
                    gears_high_trigger[i] * FIXED_POINT_MUL;
            }
        }

        revolver();
        bool is_empty();
        void cleanup();
        robo::stepper **get_steppers();
        void set_steppers(robo::stepper **steppers_table, int size);
        int queue_size();
        void current_steps(int64_t *steps);
        std::vector<int64_t> current_steps();
        std::vector<int64_t> current_steps_no_lock();
        int room();
        void push(uint16_t step, uint16_t dir);
        void serve();
        void clear();
        void clear_no_velocity_drop();
    };
}

#endif
