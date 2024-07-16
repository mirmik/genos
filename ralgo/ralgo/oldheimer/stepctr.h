#ifndef RALGO_HEIMER_STEPCTR_H
#define RALGO_HEIMER_STEPCTR_H

#include <assert.h>
#include <cstdlib>
#include <igris/dprint.h>
#include <igris/dtrace.h>
#include <igris/math.h>
#include <igris/sync/syslock.h>
#include <ralgo/oldheimer/interrupt_args.h>
#include <ralgo/oldheimer/phaser.h>

#include <ralgo/log.h>

namespace heimer
{
    template <class Position, class IntPos, class Speed>
    class stepctr : public phaser<Position, IntPos, Speed>
    {
    protected:
        using parent = phaser<Position, IntPos, Speed>;
        using parent::ext2int_pos;

        // Количество физических шагов
        int64_t steps_total = 0;

        // количество импульсов в текущем шаге
        // (дискретный аналог скорости)
        volatile double curstep = 0;

        IntPos update_position = 0;

        IntPos pulsewidth = 0; // electronic gear rate
        IntPos pulsewidth_triggered =
            0; // служебная переменная хранит значение для триггера шмидта
        IntPos accum = 0;

        bool limited = false;
        IntPos blim = 0;
        IntPos flim = 0;

        int not_corrected_counter = 0;

        float triglevel = 0.7;
        float _deltatime = 1;

    private:
        volatile double virtual_pos = 0; // позиция без учета electronic gear
        volatile double control_pos = 0; // реально установленная позиция
        // Позиции имеют тип double, чтобы корректно
        // обрабатывать движение на малых скоростях

    public:
        stepctr(const char *name) : parent(name) {}

        bool on_interrupt(control_node *slave,
                          control_node *source,
                          interrupt_args *data) override
        {
            return false; // пробросить выше
        }

        int internal_command(int argc, char **argv) override
        {
            if (strcmp(argv[0], "setintlim") == 0)
            {
                if (argc < 3)
                    return -1;

                system_lock();
                limited = 1;
                blim = atoi64(argv[1], 10, nullptr);
                flim = atoi64(argv[2], 10, nullptr);
                system_unlock();

                return 0;
            }

            if (strcmp(argv[0], "setintpos") == 0)
            {
                if (argc < 2)
                    return -1;

                set_internal_position(atoi(argv[1]));
                return 0;
            }

            else if (strcmp(argv[0], "setgain") == 0)
            {
                if (argc < 2)
                    return -1;

                parent::set_gain(atof(argv[1]));
                return 0;
            }

            else if (strcmp(argv[0], "setgear") == 0)
            {
                if (argc < 2)
                    return -1;

                set_gear(atof(argv[1]));
                return 0;
            }

            return -1;
        }

        void print_info() override
        {
            parent::print_info();
            nos::println("_deltatime:", _deltatime);
            nos::println("pulsewidth(gear):", pulsewidth);
            nos::println("curstep:", (double)curstep);
            nos::println("steps_total:", steps_total);
            nos::println("virtpos:", (IntPos)virtual_pos);
            nos::println("ctrlpos:", (IntPos)control_pos);
            nos::println("limited:", limited);
            nos::println("blim:", blim);
            nos::println("flim:", flim);
        }

        void set_deltatime(int32_t ticks_per_second)
        {
            _deltatime = 1.0 / ticks_per_second;
        }

        IntPos current_step()
        {
            return curstep;
        }
        // void set_curstep(IntPos curstep) { this->curstep = curstep; }

        int step_counter()
        {
            return steps_total;
        }

        // Position position () { return virtual_pos; }
        // Position current_position () { return virtual_pos; }
        void set_current_position(Position pos)
        {
            set_internal_position(ext2int_pos(pos));
        }

        void invoke_update() override
        {
            control_update_interrupt_args args;

            system_lock();
            parent::update_needed = false;
            virtual_pos = update_position;
            control_pos = update_position;
            parent::_target_position = virtual_pos;
            parent::_feedback_position = virtual_pos;

            parent::throw_interrupt(&args);

            system_unlock();
        }

        void set_internal_position(IntPos arg)
        {
            update_position = arg;
            parent::update_needed = true;
        }

        void set_gear(Position gear)
        {
            system_lock();
            pulsewidth = gear;
            pulsewidth_triggered = gear * triglevel;
            system_unlock();
        }

        auto gear()
        {
            return pulsewidth;
        }

        virtual void inc() = 0;
        virtual void dec() = 0;

        void set_speed_internal_impl(Speed spd) override;

        void serve_impl() override
        {
            if (curstep == 0)
                return;

            not_corrected_counter++;
            if (not_corrected_counter > 10000)
            {
                // set_speed_internal_impl(0);
                curstep = 0;
                return;
            }

            virtual_pos += curstep;

            int64_t diffpos = virtual_pos - control_pos;

            bool positive = diffpos > 0;

            if (positive)
            {
                if (diffpos > pulsewidth_triggered)
                {
                    inc();
                    steps_total++;
                    control_pos += pulsewidth;
                }
            }

            else
            {
                if (diffpos < -pulsewidth_triggered)
                {
                    dec();
                    steps_total--;
                    control_pos -= pulsewidth;
                }
            }

            parent::_target_position = virtual_pos;
            parent::_feedback_position = virtual_pos;
        }
    };

    template <class Position, class IntPos, class Speed>
    class stepctr_emulator : public stepctr<Position, IntPos, Speed>
    {
    public:
        using parent = stepctr<Position, IntPos, Speed>;
        stepctr_emulator(const char *name)
            : stepctr<Position, IntPos, Speed>(name)
        {
        }

        void inc()
        {
            parent::_feedback_position += parent::pulsewidth;
        }

        void dec()
        {
            parent::_feedback_position -= parent::pulsewidth;
        }
    };
}

#include <ralgo/oldheimer/phaser_axis.h>

template <class Position, class IntPos, class Speed>
void heimer::stepctr<Position, IntPos, Speed>::set_speed_internal_impl(
    Speed spd)
{
    not_corrected_counter = 0;

    assert(!isnan(spd));

    system_lock();
    auto vpos = virtual_pos;
    system_unlock();
    decltype(curstep) cstep = spd * _deltatime;

    if (limited && ((vpos > (flim + pulsewidth) && cstep > 0) ||
                    (vpos < (blim - pulsewidth) && cstep < 0)))
    {
        cstep = 0;

        char str[56];
        sprintf(str, "stroke internal limits : mnemo:%s", parent::mnemo());
        ralgo::warn(str);

        force_stop_interrupt_args msg("stroke_internal_limits");
        parent::throw_interrupt(&msg);
    }

    if (ABS(cstep) > pulsewidth)
    {
        cstep = 0;

        char str[56];
        sprintf(str, "impulse channel overrun : mnemo:%s", parent::mnemo());
        ralgo::warn(str);

        force_stop_interrupt_args msg("ABS(curstep) > pulsewidth");
        parent::throw_interrupt(&msg);
    }

    system_lock();
    curstep = cstep;
    system_unlock();
}

#endif
