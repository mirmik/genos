#ifndef RALGO_ROBO_STEPPER_H
#define RALGO_ROBO_STEPPER_H

namespace robo
{
    class stepper
    {
    public:
        int64_t counter = 0;

    public:
        virtual ~stepper() = default;

        virtual void inc()
        {
            ++counter;
        }
        virtual void dec()
        {
            --counter;
        }

        // The control immitation without any action
        virtual void no_action() {}

        int64_t steps_count()
        {
            return counter;
        }

        int64_t steps()
        {
            return steps_count();
        }

        void set_counter_value(int64_t val)
        {
            counter = val;
        }
    };
}

#endif
