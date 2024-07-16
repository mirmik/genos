#include <ralgo/heimer/axstate_sincos_processor.h>

using namespace heimer;

int axstate_sincos_processor::serve(disctime_t time)
{
    (void)time;

    axis_state *xl = leftside[0];
    axis_state *yl = leftside[1];
    axis_state *al = leftside[2];

    axis_state *xr = rightside[0];
    axis_state *yr = rightside[1];
    axis_state *ar = rightside[2];

    double ascale = alpha_to_radian_scale;

    position_t a = ar->ctrpos + a_right_offset;
    double s = heimpos_sin(a * ascale); // безразмерная величина
    double c = heimpos_cos(a * ascale); // безразмерная величина
    double avelocity =
        heimvel_restore(ar->ctrvel) * ascale; // скорость в радианах

    xl->ctrpos = xr->ctrpos + x_offset + radius * c;
    yl->ctrpos = yr->ctrpos + y_offset + radius * s;
    al->ctrpos = a + a_left_offset;

    xl->ctrvel = xr->ctrvel - radius * s * avelocity;
    yl->ctrvel = yr->ctrvel + radius * c * avelocity;
    al->ctrvel = ar->ctrvel;

    return 0;
}

int axstate_sincos_processor::feedback(disctime_t time)
{
    (void)time;

    axis_state *xl = leftside[0];
    axis_state *yl = leftside[1];
    axis_state *al = leftside[2];

    axis_state *xr = rightside[0];
    axis_state *yr = rightside[1];
    axis_state *ar = rightside[2];

    double ascale = alpha_to_radian_scale;

    position_t a = al->feedpos - a_left_offset;
    double s = heimpos_sin(a * ascale); // безразмерная величина
    double c = heimpos_cos(a * ascale); // безразмерная величина
    double avelocity =
        heimvel_restore(al->feedvel) * ascale; // скорость в радианах

    xr->feedpos = xl->feedpos - x_offset - radius * c;
    yr->feedpos = yl->feedpos - y_offset - radius * s;
    ar->feedpos = a - a_right_offset;

    xr->feedvel = xl->feedvel + radius * s * avelocity;
    yr->feedvel = yl->feedvel - radius * c * avelocity;
    ar->feedvel = al->feedvel;

    return 0;
}

static inline int info(axstate_signal_processor *, int, char **, char *, int)
{
    return 0;
}

static inline int
radius(axstate_sincos_processor *proc, int argc, char **argv, char *, int)
{
    if (argc < 1)
        return -1;

    proc->set_radius(atof(argv[0]));
    return 0;
}

int axstate_sincos_processor::command(int argc,
                                      char **argv,
                                      char *output,
                                      int outmax)
{
    int status = ENOENT;

    if (strcmp("info", argv[0]) == 0)
        status = ::info(this, argc - 1, argv + 1, output, outmax);

    if (strcmp("radius", argv[0]) == 0)
        status = ::radius(this, argc - 1, argv + 1, output, outmax);

    if (strcmp("ascale", argv[0]) == 0)
    {
        if (argc < 1)
            return -1;

        set_alpha_scale(atof(argv[1]));
        return 0;
    }

    if (status != ENOENT)
        return status;

    return signal_processor::command(argc, argv, output, outmax);
}

void axstate_sincos_processor::deinit() {}

void axstate_sincos_processor::set_alpha_scale(double ascale)
{
    alpha_to_radian_scale = ascale;
}

void axstate_sincos_processor::set_offset(position_t xoff,
                                          position_t yoff,
                                          position_t aloff,
                                          position_t aroff)
{
    x_offset = xoff;
    y_offset = yoff;
    a_left_offset = aloff;
    a_right_offset = aroff;
}

void axstate_sincos_processor::set_x_offset(position_t xoff)
{
    x_offset = xoff;
}

void axstate_sincos_processor::set_y_offset(position_t yoff)
{
    y_offset = yoff;
}

void axstate_sincos_processor::set_a_left_offset(position_t aoff)
{
    a_left_offset = aoff;
}

void axstate_sincos_processor::set_a_right_offset(position_t aoff)
{
    a_right_offset = aoff;
}

axstate_sincos_processor::axstate_sincos_processor(const char *name)
    : axstate_signal_processor(name, 3, 3)
{
    attach_leftside_table(leftside);
    attach_rightside_table(rightside);

    this->radius = 0;
    attach_leftside_table(leftside);
    attach_rightside_table(rightside);
}

void heimer::axstate_sincos_processor::on_activate(disctime_t)
{
    for (int i = 0; i < 3; ++i)
    {
        rightside[i]->ctrpos = rightside[i]->feedpos;
        rightside[i]->ctrvel = rightside[i]->feedvel;
    }
}

void heimer::axstate_sincos_processor::set_radius(position_t p)
{
    radius = p;
}
