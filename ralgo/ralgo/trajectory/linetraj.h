#ifndef RALGO_TRAJECTORY_LINETRAJ_H
#define RALGO_TRAJECTORY_LINETRAJ_H

#include <igris/datastruct/sparse_array.h>
#include <ralgo/heimer/heimer_types.h>
#include <ralgo/trajectory/trajectory.h>

typedef struct sf_position
{
    position_t spos;
    position_t fpos;
} sf_position_t;

/// Простейшая линейная в пространстве параметров траектория.
class line_trajectory : public trajectory
{
public:
    disctime_t stim = 0;
    disctime_t ftim = 0;

    /// Специфика использования траекторного объекта такова,
    /// что данные о крайних точках удобно хранить в распределённой структуре
    /// (см. axisctr) sfpos ссылается на элементы sf_position_t
    struct sparse_array sfpos = {};
    struct trajectory_speed_deformer tsd = {};

public:
    line_trajectory() = default;
    line_trajectory(const line_trajectory &) = default;
    line_trajectory &operator=(const line_trajectory &) = default;
    virtual ~line_trajectory() = default;

    line_trajectory(int dim,
                    sf_position_t *sfpos_array,
                    int sfpos_stride = sizeof(sf_position_t))
    {
        init(dim, sfpos_array, sfpos_stride);
    }

    void init(int dim,
              sf_position_t *sfpos_array,
              int sfpos_stride = sizeof(sf_position_t));

    /// detail: Процедура не учитывает возможные начальную и оконечную скорости.
    void init_nominal_speed(disctime_t stim,
                            disctime_t ftim,
                            position_t *spos,
                            position_t *fpos,

                            disctime_t acc_time,
                            disctime_t dcc_time,

                            int full_spattern = false);

    void init_nominal_speed(disctime_t stim,
                            disctime_t ftim,
                            position_t spos,
                            position_t fpos,
                            disctime_t acc_time,
                            disctime_t dcc_time,
                            int full_spattern = false)
    {
        position_t _spos[1] = {spos};
        position_t _fpos[1] = {fpos};
        return init_nominal_speed(
            stim, ftim, _spos, _fpos, acc_time, dcc_time, full_spattern);
    }

    int
    attime(disctime_t time, position_t *outpos, velocity_t *outvel) override;

    void set_point_hold(disctime_t time, position_t *pos);

    void set_stop_pattern(position_t *curpos,
                          velocity_t *curspd,
                          disctime_t curtime,
                          disctime_t stoptime);
};

template <int N> class lintraj : public line_trajectory
{
private:
    sf_position_t _arr[N];

public:
    lintraj() : line_trajectory(N, _arr) {}
};

#endif
