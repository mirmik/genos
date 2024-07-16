#ifndef RALGO_HEIMER_STEPPER_CONTROLLER_H
#define RALGO_HEIMER_STEPPER_CONTROLLER_H

#include <igris/compiler.h>
#include <igris/sync/syslock.h>
#include <stdint.h>

#include <ralgo/disctime.h>
#include <ralgo/heimer/heimer_types.h>
#include <ralgo/robo/stepper.h>

#include <ralgo/heimer/fast_cycle_device.h>
#include <ralgo/robo/iposvel.h>

#define STEPCTR_OVERRUN -22

namespace robo
{
    /**
        Драйвер шагового двигателя или любого устройства, управляемого шагами.
        Даёт команду на совершения шага изделия в зависимости от установленной
       скорости.
    */
    class stepper_controller : public i_position_feedback
    {
    public:
        robo::stepper *stepper = nullptr;
        double trigger_level = 0.75;

    public:
        int64_t _control_pos = 0;
        int64_t _virtual_pos = 0;

        // protected:
    public:
        int64_t units_in_step = (1 << 24);
        int64_t units_in_step_triggered = units_in_step * trigger_level;

    public:
        stepper_controller();
        stepper_controller(robo::stepper *stepper);
        stepper_controller(const stepper_controller &) = delete;
        stepper_controller &operator=(const stepper_controller &) = delete;

        void reset_current_position(double pos)
        {
            _control_pos = _virtual_pos = pos * units_in_step;
        }

        void init(robo::stepper *stepper);

        void set_steps_position(position_t pos);
        void set_position(position_t pos);

        int shift(int64_t shift);

        // дискретное время дано с плавающей точкой,
        // чтобы можно было передавать интервалы времени
        // меньше disctime
        int speed_apply(double speed, double delta);

        void set_trigger_level(double trigger_level)
        {
            this->trigger_level = trigger_level;
        }

        void set_units_in_step(int64_t ups)
        {
            units_in_step = ups;
            units_in_step_triggered = ups * trigger_level;
            evaluate();
        }

        virtual void evaluate() {}

        int64_t control_pos()
        {
            return _control_pos;
        }
        int64_t virtual_pos()
        {
            return _virtual_pos;
        }

        double real_feedback_position()
        {
            system_lock();
            auto counter_value = stepper->steps_count();
            system_unlock();

            return counter_value;
        }

        double control_feedback_position()
        {
            system_lock();
            auto counter_value = (double)_virtual_pos / (double)units_in_step;
            system_unlock();

            return counter_value;
        }

        double feedback_position() override
        {
            return control_feedback_position();
        }

        virtual void info(char *buf, int len);
    };

    class fixed_frequency_stepper_controller : public stepper_controller,
                                               public heimer::fast_cycle_device,
                                               public i_velocity_driver
    {
        void (*interrupt_handle)(void *, int) = nullptr;
        void *interrupt_priv = nullptr;

    public:
        double speed_to_shift = 1;
        double freq = 1;
        int64_t current_shift = 0;

    public:
        fixed_frequency_stepper_controller();
        fixed_frequency_stepper_controller(robo::stepper *stepper);

        fixed_frequency_stepper_controller(
            const fixed_frequency_stepper_controller &) = delete;
        fixed_frequency_stepper_controller &
        operator=(const fixed_frequency_stepper_controller &) = delete;

        void set_frequency(double _freq)
        {
            freq = _freq;
            evaluate();
        }

        void set_interrupt_handler(void (*handle)(void *, int), void *arg)
        {
            this->interrupt_handle = handle;
            this->interrupt_priv = arg;
        }

        void set_velocity(double speed) override;

        int constant_frequency_serve();

        double feedback_velocity() override
        {
            return (double)current_shift / (double)speed_to_shift;
        }

        void info(char *buf, int len) override;

        double frequency()
        {
            return freq;
        }

        int fast_cycle_serve() override
        {
            return constant_frequency_serve();
        }

    private:
        void evaluate() override;
    };
}

#endif
