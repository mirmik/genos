#ifndef RALGO_ROBO_STEPPER_SIMULATOR_H
#define RALGO_ROBO_STEPPER_SIMULATOR_H

namespace robo
{
    class stepper
    {
    public:
        virtual void inc() = 0;
        virtual void dec() = 0;
        virtual int64_t steps_count() = 0;
    };
}

#endif
