/**
    @file velocity_applier.h
*/

#ifndef RALGO_HEIMER_VELOCITY_APPLIER
#define RALGO_HEIMER_VELOCITY_APPLIER

#include <ralgo/heimer/axis_state.h>
#include <ralgo/heimer/signal_processor.h>
#include <ralgo/log.h>
#include <ralgo/robo/stepper_controller.h>

namespace heimer
{
    /**
        Применятет данные объектов axis_state к объектам реализующим интерфейсы
       датчиков скоростей и положений библиотеки robo. Комплементарно
       корректирует установленную скорость по данных о положении.
    */
    class velocity_applier : public signal_processor
    {
        robo::i_velocity_setter *controlled_velset = nullptr;
        robo::i_velocity_feedback *controlled_velget = nullptr;
        robo::i_position_feedback *controlled_posget = nullptr;
        axis_state *state = nullptr;

        position_t deviation_error_limit = 0.1;
        double compkoeff = compkoeff_timeconst(
            0.003); /// Коэффициент комплементарного фильтра.
        double compkoeff_hard = compkoeff_timeconst(
            0.0001); /// Коэффициент комплементарного фильтра.

        // Количество импульсов в системной единице (миллиметре или радиане).
        double gear = 1;

        disctime_t last_time = 0;

        robo::fixed_frequency_stepper_controller *stepctr = nullptr;

        bool deactivation_enabled = false;
        bool interrupt_situation = false;

    public:
        // debug
        velocity_t compspd = 0;

    public:
        velocity_applier();

        velocity_applier(const char *name,
                         robo::fixed_frequency_stepper_controller *stepctr);

        velocity_applier(const char *name,
                         robo::fixed_frequency_stepper_controller *stepctr,
                         axis_state *state);

        velocity_applier(const velocity_applier &) = delete;
        velocity_applier &operator=(const velocity_applier &) = delete;

        void set_gear(double gear);

        void init(const char *name,
                  robo::fixed_frequency_stepper_controller *stepctr,
                  axis_state *state);

        int bind(int argc, char **argv, char *output, int outmax);

        static double compkoeff_timeconst(double T)
        {
            return 1. / discrete_time_frequency() / T;
        }
        void set_compkoeff(double ck)
        {
            this->compkoeff = ck;
        }
        void set_compkoeff_timeconst(double T)
        {
            this->compkoeff = compkoeff_timeconst(T);
        }
        int set_current_position_protected(double pos);
        int set_current_steps_protected(int64_t pos);

        int feedback(disctime_t time) override;
        int serve(disctime_t time) override;
        int info(char *ans, int anslen) override;
        int command(int argc, char **argv, char *output, int outmax) override;
        void deinit() override;
        signal_head *iterate_left(signal_head *) override;
        signal_head *iterate_right(signal_head *) override;

        void on_activate(disctime_t time) override
        {
            last_time = time;
            deactivation_enabled = false;
        }

        int on_deactivation_request(disctime_t) override
        {
            if (interrupt_situation)
            {
                interrupt_situation = false;
                return 0;
            }

            deactivation_enabled = true;
            return 1;
        }
    };
}

#endif
