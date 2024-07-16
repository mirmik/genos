#ifndef RALGO_PLANNING_TRAJ_ND_H
#define RALGO_PLANNING_TRAJ_ND_H

#include <nos/fprint.h>
#include <ralgo/trajectory/tsdeform.h>

namespace ralgo
{
    template <size_t Dim, class P, class V> class trajNd
    {
    protected:
        // Время в дискретных единицах времени.
        // (см. ralgo::discrete_time_frequency)
        int64_t stim;
        int64_t ftim;

    public:
        virtual int attime(int64_t time,
                           igris::array_view<P> pos,
                           igris::array_view<V> spd) = 0;

        bool is_finished(int64_t time)
        {
            return time > ftim;
        }
    };

    template <size_t Dim, class P, class V>
    class trajNd_line : public trajNd<Dim, P, V>
    {
    public:
        using trajNd<Dim, P, V>::stim;
        using trajNd<Dim, P, V>::ftim;

        P spos[Dim];
        P fpos[Dim];

        V setted_speed[Dim];

    public:
        struct trajectory_speed_deformer spddeform;

        void set_start_position(int i, P pos)
        {
            spos[i] = pos;
        }

        void set_finish_position_inc(int i, P inc)
        {
            fpos[i] = spos[i] + inc;
        }

        // Инициализировать траекторию, когда spos и fpos уже установлены.
        int reset(int64_t stim, int64_t ftim)
        {
            this->stim = stim;
            this->ftim = ftim;

            assert(stim != ftim);

            for (unsigned int i = 0; i < Dim; ++i)
            {
                if (stim == ftim)
                    setted_speed[i] = 0;
                else
                {
                    setted_speed[i] =
                        ((double)(fpos[i] - spos[i])) / ((double)(ftim - stim));
                }
            }

            return 0;
        }

        int reset(igris::array_view<P> &spos,
                  int64_t stim,
                  igris::array_view<P> &fpos,
                  int64_t ftim)
        {
            std::copy(std::begin(spos), std::end(spos), std::begin(this->spos));
            std::copy(std::begin(fpos), std::end(fpos), std::begin(this->fpos));
            reset(stim, ftim);

            return 0;
        }

        int attime(int64_t time,
                   igris::array_view<P> pos,
                   igris::array_view<V> spd) override
        {
            // Умножение на коэффициент времени перерасщитывает скорость
            // взятую на дискретную единицу времени в скорость взятую
            // на единицу времени рабочего пространства.

            double time_unit =
                ftim == stim ? 0
                             : (double)(time - stim) / (double)(ftim - stim);

            assert(!isnan(time_unit));

            auto posmod = tsdeform_posmod(&spddeform, time_unit);
            auto spdmod = tsdeform_spdmod(&spddeform, time_unit);

            for (unsigned int i = 0; i < Dim; ++i)
            {
                pos[i] = fpos[i] * posmod + spos[i] * (1 - posmod);
                spd[i] =
                    setted_speed[i] * spdmod * ralgo::discrete_time_frequency();
            }

            return (tsdeform_is_finished(&spddeform, time_unit) || stim == ftim)
                       ? 1
                       : 0;
        }

        void set_speed_pattern(double acc,
                               double dcc,
                               double speed,
                               bool full_spattern = false)
        {
            // Чтобы расчитать интервал времени разгона, необходимо
            // соотнести значение ускорения и скорости.
            // Здесь acc - тангенс угла,
            // setted_speed - установленная скорость в дискреных единицах
            // тогда setted_speed / acc = acc_time в дискретных единицах

            double time = ftim - stim;

            double acc_time = speed / acc * ralgo::discrete_time_frequency();
            double dcc_time = speed / dcc * ralgo::discrete_time_frequency();

            double acc_part = acc_time / time;
            double dcc_part = dcc_time / time;

            // ralgo::speed_deformer::acc_dcc_balance(acc_part, dcc_part);

            tsdeform_set_speed_pattern(
                &spddeform, acc_part, dcc_part, 0, 0, full_spattern);
        }

        void set_stop_trajectory(igris::array_view<P> curpos,
                                 igris::array_view<V> curspd,
                                 V dccval)
        {
            // скоростной деформатор работает с точным выведением в позицию, и
            // изменяет время, поэтому подменяем время в два раза, чтобы
            // соответствовать равнозамедленному паттерну.

            stim = ralgo::discrete_time();
            double realdiff = ralgo::vecops::length(curspd) / dccval;
            ftim = stim + realdiff / 2 * ralgo::discrete_time_frequency();

            std::copy(
                std::begin(curpos), std::end(curpos), std::begin(this->spos));
            if (ftim > stim)
            {
                for (unsigned int i = 0; i < Dim; ++i)
                    fpos[i] = spos[i] + curspd[i] * realdiff / 2;

                for (unsigned int i = 0; i < Dim; ++i)
                    setted_speed[i] =
                        curspd[i] / ralgo::discrete_time_frequency();
            }
            else
            {
                ftim = stim + 1; // prevent zero division
                for (unsigned int i = 0; i < Dim; ++i)
                    fpos[i] = spos[i];

                ralgo::vecops::fill(setted_speed, 0);
            }

            tsdeform_set_stop_pattern(&spddeform);
        }

        void set_point_hold(igris::array_view<P> curpos)
        {
            ftim = ralgo::discrete_time();
            stim = ftim - 1;

            std::copy(
                std::begin(curpos), std::end(curpos), std::begin(this->spos));
            std::copy(
                std::begin(curpos), std::end(curpos), std::begin(this->fpos));

            ralgo::vecops::fill(setted_speed, 0);

            tsdeform_set_stop_pattern(&spddeform);
        }

        ssize_t print_to(nos::ostream &os) const
        {
            return nos::fprint_to(os, "({},{},{})", spos, fpos, setted_speed);
        }
    };
}

#endif
