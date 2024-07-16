#ifndef RALGO_HEIMER_SPEED_PHASER_AXIS_H
#define RALGO_HEIMER_SPEED_PHASER_AXIS_H

#include <ralgo/oldheimer/axis.h>
#include <ralgo/oldheimer/phaser.h>

#include <hal/irq.h>
#include <util/cpu_delay.h>

namespace heimer
{
    template <class P, class IntPos, class V>
    class phaser_axis : public axis_node<P, V>
    {
        using parent = axis_node<P, V>;

    public:
        phaser<P, IntPos, V> *controlled = nullptr;
        V compspd = 0;
        float compkoeff = 0;
        P diff;

    public:
        constexpr phaser_axis(const char *name) : axis_node<P, V>(name) {}

        constexpr phaser_axis(const char *name, phaser<P, IntPos, V> *phaser)
            : axis_node<P, V>(name), controlled(phaser)
        {
        }

        void print_info() override
        {
            nos::println("compkoeff:", compkoeff);
            nos::println("compspd:", compspd);
            nos::println("diff * 1000:", diff * 1000);
            parent::print_info();
        }

        control_node *iterate(control_node *it)
        {
            if (it == nullptr)
                return controlled;
            return nullptr;
        }

        void feedback()
        {
            parent::feedpos = controlled->feedback_position();
            parent::feedspd = controlled->feedback_speed();
        }

        void serve_impl() override
        {
            // Ошибка по установленному значению.
            if (controlled->update_needed)
            {
                controlled->invoke_update();
            }

            diff = parent::position_error();

            if (diff > 1)
            {
                controlled->set_speed(0);

                char str[56];
                sprintf(str,
                        "position deviation error : mnemo:%s",
                        parent::mnemo());
                ralgo::warn(str);

                force_stop_interrupt_args msg("position deviation error");
                parent::throw_interrupt(&msg);
            }

            // Скорость вычисляется как
            // сумма уставной скорости на
            compspd = parent::ctrspd + compkoeff * diff;
            assert(!isnan(compspd));
            controlled->set_speed(compspd);
        }

        void set_compensate(float val)
        {
            compkoeff = val;
        }

        V compensated_speed()
        {
            return compspd;
        }

        P request_feedback_position() override
        {
            // Запрасываем данные от контролируемого устройства.
            return controlled->feedback_position();
        }
    };
}

#endif
