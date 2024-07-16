#include <igris/dprint.h>
#include <ralgo/trajectory/linetraj.h>

int line_trajectory::attime(disctime_t time, position_t *pos, velocity_t *spd)
{
    disctime_t local_time = time - stim;
    disctime_t full_time = ftim - stim;

    assert(full_time > 0);

    double time_unit =
        local_time <= 0 ? 0 : (double)(local_time) / (double)(full_time);

    double posmod = tsdeform_posmod(&tsd, time_unit);
    double spdmod = tsdeform_spdmod(&tsd, time_unit);

    for (int i = 0; i < dim; ++i)
    {
        sf_position_t *pair = sparse_array_ptr(&sfpos, i, sf_position_t);
        position_t spos = pair->spos;
        position_t fpos = pair->fpos;

        // Положение вычисляется по формуле линейной интерполяции.
        pos[i] = fpos * posmod + spos * (1 - posmod);

        // Скорость вычисляется просто путём умножения на коэффицент.
        // Выходная скорость имеет размерность единицы длины
        // на дискретную единицу времени
        velocity_t naive_speed =
            (velocity_t)(fpos - spos) / (velocity_t)full_time;
        spd[i] = naive_speed * spdmod;
    }

    return (tsdeform_is_finished(&tsd, time_unit) || stim == ftim) ? 1 : 0;
}

void line_trajectory::init(int dim,
                           sf_position_t *sfpos_array,
                           int sfpos_stride)
{
    trajectory::init(dim);
    sparse_array_init(&sfpos, sfpos_array, sfpos_stride);
}

/// detail: Процедура не учитывает возможные начальную и оконечную скорости.
void line_trajectory::init_nominal_speed(disctime_t stim,
                                         disctime_t ftim,
                                         position_t *spos,
                                         position_t *fpos,

                                         disctime_t acc_time,
                                         disctime_t dcc_time,

                                         int full_spattern)
{
    this->stim = stim;
    this->ftim = ftim;

    for (int i = 0; i < dim; ++i)
    {
        sf_position_t *pair = sparse_array_ptr(&sfpos, i, sf_position_t);

        pair->spos = spos[i];
        pair->fpos = fpos[i];
    }

    disctime_t time = ftim - stim;

    double acc_part = (double)acc_time / (double)time;
    double dcc_part = (double)dcc_time / (double)time;

    tsdeform_set_speed_pattern(&tsd, acc_part, dcc_part, 0, 0, full_spattern);
}

void line_trajectory::set_point_hold(disctime_t ftim, position_t *pos)
{
    this->ftim = ftim;
    this->stim = ftim - 1;

    for (int i = 0; i < dim; ++i)
    {
        sf_position_t *pair = sparse_array_ptr(&sfpos, i, sf_position_t);

        pair->spos = pos[i];
        pair->fpos = pos[i];
    }

    tsdeform_set_stop_pattern(&tsd);
}

void line_trajectory::set_stop_pattern(position_t *curpos,
                                       velocity_t *curspd,
                                       disctime_t curtime,
                                       disctime_t stoptime)
{
    // скоростной деформатор работает с точным выведением в позицию, и изменяет
    // время, поэтому подменяем время в два раза, чтобы соответствовать
    // равнозамедленному паттерну.

    stim = curtime;
    ftim = curtime + stoptime / 2; // Время измеяется из-за паттерна деформации.

    if (ftim > stim)
    {
        for (int i = 0; i < dim; ++i)
        {
            sf_position_t *pair = sparse_array_ptr(&sfpos, i, sf_position_t);
            pair->spos = curpos[i];
            pair->fpos = curpos[i] + curspd[i] * stoptime /
                                         2; // аналогичное сжатие времени.
        }
    }
    else
    {
        ftim = stim + 1; // prevent zero division
        for (int i = 0; i < dim; ++i)
        {
            sf_position_t *pair = sparse_array_ptr(&sfpos, i, sf_position_t);
            pair->spos = curpos[i];
            pair->fpos = curpos[i];
        }
    }

    // Паттерн подменяет прямоугольное движение на треугольное растянутое во
    // времени в два раза.
    tsdeform_set_stop_pattern(&tsd);
}
