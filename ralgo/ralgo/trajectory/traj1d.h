#ifndef HEIMER_TRAJ1D_H
#define HEIMER_TRAJ1D_H

/**
    @file
*/

#include <ralgo/disctime.h>
#include <ralgo/trajectory/tsdeform.h>

#include <igris/dprint.h>

#define HEIMER_TRAJ1D_CONTINUE 0
#define HEIMER_TRAJ1D_FINISHED 1

// TODO:
// 		Есть подозрение, что деление на discrete_time_frequency
//      можно упразднить.

namespace ralgo
{
    template <class P, class V> struct traj1d_nominal_speed_params
    {
        int64_t stim = 0;
        P spos = 0;
        P fpos = 0;

        V speed = 0;
        V acc = 0;
        V dcc = 0;

        bool full_spattern = false;
    };

    template <class P, class V> struct traj1d_timestamp_params
    {
        int64_t stim = 0;
        int64_t ftim = 0;
        int64_t acctime = 0;
        int64_t dcctime = 0;
        P spos = 0;
        P fpos = 0;
    };

    template <class P, class V> class traj1d
    {
    public:
        int64_t stim = -1;
        int64_t ftim = 0;

    public:
        virtual ~traj1d() = default;
        virtual int attime(int64_t time, P &pos, V &spd) = 0;
        bool is_finished(int64_t time)
        {
            return time > ftim;
        }
    };

    template <class P, class V> class traj1d_line : public traj1d<P, V>
    {
    public:
        using traj1d<P, V>::stim;
        using traj1d<P, V>::ftim;
        P spos = 0;
        P fpos = 0;

        V setted_speed = 0;

    private:
        struct trajectory_speed_deformer spddeform = {};

    public:
        void init_timestamp_mode(struct traj1d_timestamp_params<P, V> *args)
        {
            spos = args->spos;
            fpos = args->fpos;

            stim = args->stim;
            ftim = args->ftim;

            if (stim == ftim)
                setted_speed = 0;
            else
            {
                setted_speed = (double)(fpos - spos) / (ftim - stim);
            }

            double acc_part = (double)args->acctime / (double)(ftim - stim);
            double dcc_part = (double)args->dcctime / (double)(ftim - stim);

            if (acc_part + dcc_part > 1)
            {
                acc_part = 0.5;
                dcc_part = 0.5;
            }

            tsdeform_set_timestamp_pattern(
                &spddeform, acc_part, dcc_part, 0, 0);
        }

        void
        init_nominal_speed_mode(struct traj1d_nominal_speed_params<P, V> *args)
        {
            setted_speed = args->speed / ralgo::discrete_time_frequency();

            P dist = args->fpos - args->spos;
            double time = fabs(dist) / setted_speed;

            if (dist < 0)
                setted_speed = -setted_speed;

            spos = args->spos;
            fpos = args->fpos;

            stim = args->stim;
            ftim = stim + (int64_t)time;

            double acc_time =
                args->speed / args->acc * ralgo::discrete_time_frequency();
            double dcc_time =
                args->speed / args->dcc * ralgo::discrete_time_frequency();

            double acc_part = acc_time / time;
            double dcc_part = dcc_time / time;

            // Учёт возможного треугольного паттерна осуществляется
            // здесь:
            tsdeform_set_speed_pattern(
                &spddeform, acc_part, dcc_part, 0, 0, args->full_spattern);
        }

        int attime(int64_t time, P &pos, V &spd) override
        {
            // Умножение на коэффициент времени перерасщитывает скорость
            // взятую на дискретную единицу времени в скорость взятую
            // на единицу времени рабочего пространства.

            /*
                u = t / (t_fini - t_strt)
                P(t) = posmod(u)
                x = x[0] + v*t*P(t) = x[0] + (x[1]-x[0])*P(t)
                v = V * spdmod(u)
            */

            double time_unit =
                ftim == stim ? 0
                             : (double)(time - stim) / (double)(ftim - stim);

            assert(!isnan(time_unit));

            auto posmod = tsdeform_posmod(&spddeform, time_unit);
            auto spdmod = tsdeform_spdmod(&spddeform, time_unit);

            pos = fpos * posmod + spos * (1 - posmod);
            spd = setted_speed * spdmod * ralgo::discrete_time_frequency();

            return (tsdeform_is_finished(&spddeform, time_unit) || stim == ftim)
                       ? 1
                       : 0;
        }

        void set_stop_trajectory(P curpos, V curspd, V dccval)
        {
            // скоростной деформатор работает с точным выведением в позицию, и
            // изменяет время, поэтому подменяем время в два раза, чтобы
            // соответствовать равнозамедленному паттерну.

            stim = ralgo::discrete_time();
            double realdiff = (fabs(curspd) / dccval);
            ftim =
                stim + (int)(realdiff * ralgo::discrete_time_frequency() / 2);

            spos = curpos;

            if (ftim > stim)
            {
                fpos = curpos + curspd * realdiff / 2;
                setted_speed = curspd / ralgo::discrete_time_frequency();
            }
            else
            {
                ftim = stim + 1;
                fpos = spos;
                setted_speed = 0;
            }

            tsdeform_set_stop_pattern(&spddeform);
        }

        void set_point_hold(P pos)
        {
            ftim = ralgo::discrete_time();
            stim = ftim - 1;

            spos = pos;
            fpos = pos;

            setted_speed = 0;

            tsdeform_set_stop_pattern(&spddeform);
        }
    };
}

#endif
