#ifndef RALGO_HEIMER_LINEAR_INTERPOLATOR_H
#define RALGO_HEIMER_LINEAR_INTERPOLATOR_H

#include <iterator>

#include <igris/dtrace.h>
#include <ralgo/linalg/vecops.h>

#include <ralgo/oldheimer/control.h>
#include <ralgo/oldheimer/coordinate_checker.h>
#include <ralgo/oldheimer/interpolation_group.h>

#include <ralgo/trajectory/trajNd.h>

namespace heimer
{
    template <class Position, class Speed, int Dim>
    class linintctr : public linintctr_basic<Position, Speed>
    {
        // Линейная интерполяция в декартовой метрике.

        using parent = linintctr_basic<Position, Speed>;
        bool operation_finished_flag = true;

        Speed _speed = 1;
        Speed _acc_val = 1;
        Speed _dcc_val = 1;

        // float _speed_multiplier = 1;

        ralgo::trajNd<Dim, Position, Speed> *curtraj = nullptr;
        ralgo::trajNd_line<Dim, Position, Speed> lintraj;

        float _gains[Dim] = {};
        int8_t _reverse[Dim] = {};

        float poskoeff = 0.01;
        bool _in_operation = false;

        igris::array_view<heimer::axis_node<Position, Speed> *> _axes;

    public:
        bool in_operate()
        {
            return _in_operation;
        }

        linintctr(const char *name,
                  igris::array_view<heimer::axis_node<Position, Speed> *> axes)
            : linintctr(name, axes.data())
        {
        }

        void current_point(Position *ptr)
        {
            for (int i = 0; i < Dim; ++i)
            {
                *ptr++ = _axes[i]->feedpos;
            }
        }

        linintctr(const char *name, heimer::axis_node<Position, Speed> **axes)
            : linintctr_basic<Position, Speed>(name), _axes(axes, Dim)
        {
            ralgo::vecops::fill(_gains, 1.f);
            ralgo::vecops::fill(_reverse, 0);
        }

        constexpr int dim()
        {
            return Dim;
        }

        axis_node<float, float> *get_axis(int index)
        {
            return _axes[index];
        }

        int _move(igris::array_view<Position> curpos,
                  igris::array_view<Position> tgtpos,
                  bool without_checker = false)
        {
            if (ralgo::global_protection)
            {
                ralgo::warn(parent::mnemo(),
                            ": cannot start: global protection is setted");
                return -1;
            }

            if (!parent::is_active())
            {
                ralgo::warn(parent::mnemo(), ": not active");
                return -1;
            }

            if (parent::coord_checker && !without_checker)
            {
                char msg[64];
                if (parent::coord_checker->check(
                        this, tgtpos.data(), tgtpos.size(), msg))
                {
                    ralgo::warn(parent::mnemo(), ": ", msg);
                    return -1;
                }
            }

            float speed_multiplier = evaluate_speed_multiplier(curpos, tgtpos);
            float gained_speed = speed_multiplier * _speed;
            float gained_acc_val = speed_multiplier * _acc_val;
            float gained_dcc_val = speed_multiplier * _dcc_val;

            auto dist = ralgo::vecops::distance(curpos, tgtpos);

            int64_t time = (int64_t)(((Speed)fabs(dist)) / gained_speed *
                                     ralgo::discrete_time_frequency());
            int64_t curtime = ralgo::discrete_time();
            int64_t tgttim = curtime + time;

            if (dist == 0 || curtime == tgttim)
            {
                return 0;
            }

            lintraj.reset(curpos, curtime, tgtpos, tgttim);
            lintraj.set_speed_pattern(gained_acc_val,
                                      gained_dcc_val,
                                      gained_speed,
                                      parent::enable_full_spattern);

            curtraj = &lintraj;
            _in_operation = true;
            operation_finished_flag = false;
            parent::operation_start_signal(this);

            return 0;
        }

        float evaluate_speed_multiplier(igris::array_view<Position> curpos,
                                        igris::array_view<Position> tgtpos)
        {
            Position interval_with_gain[Dim];
            Position interval_without_gain[Dim];

            for (unsigned int i = 0; i < Dim; ++i)
            {
                interval_with_gain[i] = tgtpos[i] - curpos[i];
                interval_without_gain[i] = interval_with_gain[i] / _gains[i];
            }

            float interval_without_gain_length =
                ralgo::vecops::length(interval_without_gain);
            float interval_with_gain_length =
                ralgo::vecops::length(interval_with_gain);

            float speed_multiplier =
                interval_with_gain_length / interval_without_gain_length;
            return speed_multiplier;
        }

        float
        evaluate_speed_multiplier_by_curspd(igris::array_view<Position> curspd)
        {
            Speed without_gain[Dim];

            for (unsigned int i = 0; i < Dim; ++i)
            {
                without_gain[i] = curspd[i] / _gains[i];
            }

            float without_gain_length = ralgo::vecops::length(without_gain);
            float with_gain_length = ralgo::vecops::length(curspd);

            float speed_multiplier = with_gain_length / without_gain_length;
            return speed_multiplier;
        }

        int incmove(igris::array_view<Position> mov)
        {
            Position curpos[Dim];
            Position tgtpos[Dim];

            for (unsigned int i = 0; i < mov.size(); ++i)
            {
                curpos[i] = get_axis(i)->target_position();
                tgtpos[i] = curpos[i] + mov[i] * _gains[i];
            }

            return _move(curpos, tgtpos);
        }

        int absmove(igris::array_view<Position> pos,
                    bool without_checker = false)
        {
            Position curpos[Dim];
            Position tgtpos[Dim];

            for (unsigned int i = 0; i < pos.size(); ++i)
            {
                curpos[i] = get_axis(i)->ctrpos;
                tgtpos[i] = pos[i] * _gains[i];
            }

            return _move(curpos, tgtpos, without_checker);
        }

        int incmove(Position *mov)
        {
            return incmove({mov, Dim});
        }

        int absmove(Position *pos)
        {
            return absmove({pos, Dim});
        }

        int parted_absmove(int *axno, Position *pos, int len)
        {
            Position curpos[Dim];
            Position tgtpos[Dim];

            for (unsigned int i = 0; i < Dim; ++i)
            {
                curpos[i] = get_axis(i)->ctrpos;
                tgtpos[i] = curpos[i];
            }

            for (int i = 0; i < len; ++i)
            {
                tgtpos[axno[i]] = pos[i] * _gains[axno[i]];
            }

            return _move(curpos, tgtpos);
        }

        int set_speed(Speed speed)
        {
            _speed = speed;
            return 0;
        }

        // Установить ускорение в собственных единицах 1/c^2.
        int set_accdcc(float acc, float dcc)
        {
            _acc_val = acc;
            _dcc_val = dcc;
            return 0;
        }

        int update_control_by_trajectory()
        {
            Position ctrpos[Dim];
            Speed ctrspd[Dim];

            int is_finish =
                curtraj->attime(ralgo::discrete_time(), ctrpos, ctrspd);

            apply_phase(ctrpos, ctrspd);

            if (is_finish)
            {
                _in_operation = false;
            }

            return is_finish;
        }

        void apply_phase(Position *ctrpos, Speed *ctrspd)
        {
            for (int i = 0; i < Dim; ++i)
            {
                _axes[i]->control(ctrpos[i], ctrspd[i]);
            }
        }

        float *gains() override
        {
            return _gains;
        }

        void set_gains(igris::array_view<float> arr) override
        {
            for (unsigned int i = 0; i < arr.size(); ++i)
                _gains[i] = arr[i];
        }

        void set_reverses(igris::array_view<int> arr) override
        {
            for (unsigned int i = 0; i < arr.size(); ++i)
                _reverse[i] = arr[i];
        }

        void serve_impl() override
        {
            int sts;

            if (curtraj)
            {
                sts = update_control_by_trajectory();

                if (sts && !operation_finished_flag)
                {
                    operation_finished_flag = true;
                    parent::operation_finish_signal(this);
                }
            }
        }

        void print_info() override
        {
            nos::println("TODO");
        }

        Speed speed()
        {
            return _speed;
        }
        Speed acceleration()
        {
            return _acc_val;
        }
        Speed deceleration()
        {
            return _dcc_val;
        }

        control_node *iterate(control_node *slt) override
        {
            if (slt == nullptr)
                return _axes[0];

            for (unsigned int i = 0; i < _axes.size() - 1; ++i)
            {
                if (slt == _axes[i])
                {
                    return _axes[i + 1];
                }
            }

            return nullptr;
        }

    public:
        int hardstop() override
        {
            Position curpos[Dim];

            for (unsigned int i = 0; i < Dim; ++i)
            {
                curpos[i] = get_axis(i)->target_position();
            }

            lintraj.set_point_hold(curpos);

            for (unsigned int i = 0; i < _axes.size(); ++i)
            {
                _axes[i]->ctrspd = 0;
                _axes[i]->ctrpos = _axes[i]->feedpos;
            }

            parent::operation_finish_signal(this);
            operation_finished_flag = true;

            for (unsigned int i = 0; i < _axes.size(); ++i)
            {
                _axes[i]->hardstop();
            }

            curtraj = &lintraj;

            return 0;
        }

        void collect_feedpos(Position *feedpos)
        {
            for (int i = 0; i < Dim; i++)
                feedpos[i] = _axes[i]->feedpos;
        }

        void collect_feedspd(Position *feedspd)
        {
            for (int i = 0; i < Dim; i++)
                feedspd[i] = _axes[i]->feedspd;
        }

        int stop() override
        {
            Position feedpos[Dim];
            Speed feedspd[Dim];

            collect_feedpos(feedpos);
            collect_feedspd(feedspd);

            if (curtraj == nullptr)
                return 0;

            float speed_multiplier =
                evaluate_speed_multiplier_by_curspd(feedspd);
            lintraj.set_stop_trajectory(
                feedpos, feedspd, _dcc_val * speed_multiplier);

            operation_finished_flag = false;
            curtraj = &lintraj;

            return 0;
        }

        bool on_interrupt(control_node *slave,
                          control_node *source,
                          interrupt_args *data) override
        {
            (void)slave;
            (void)source;
            if (data->code() == HEIMER_INTERRUPT_TYPE_CONTROL_UPDATE)
            {
                // update_from_controlled();
                stop();
            }

            else
            {
                hardstop();
            }

            return false; // пробросить выше
        }
    };
}

#endif
