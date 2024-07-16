#include <ralgo/heimer/axisctr.h>
#include <ralgo/heimer/sigtypes.h>
#include <ralgo/linalg/vecops.h>
#include <ralgo/log.h>

#include <igris/datastruct/sparse_array.h>
#include <igris/math.h>
#include <igris/util/bug.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace heimer;

void axis_controller::set_handlers(
    void *operation_handlers_priv,
    void (*operation_start_handler)(void *priv, axis_controller *ax),
    void (*operation_finish_handler)(void *priv, axis_controller *ax))
{
    this->operation_handlers_priv = operation_handlers_priv;
    this->operation_start_handler = operation_start_handler;
    this->operation_finish_handler = operation_finish_handler;
}

void axis_controller::set_gain(double *gain)
{
    for (int i = 0; i < leftdim(); ++i)
    {
        settings[i].gain = gain[i];
    }
}

void axis_controller::set_velocity_external(double speed)
{
    vel = speed;
}

void axis_controller::set_accdcc_external(double acc, double dcc)
{
    this->acc = acc;
    this->dcc = dcc;
}

void axis_controller::set_acceleration_external(double acc)
{
    this->acc = acc;
}

void axis_controller::set_decceleration_external(double dcc)
{
    this->dcc = dcc;
}

void axis_controller::set_limits_external(double *back, double *forw)
{
    for (int i = 0; i < leftdim(); ++i)
    {
        settings[i].limits_enabled = 1;
        settings[i].backlim = back[i];
        settings[i].forwlim = forw[i];
    }
}

void axis_controller::set_controlled(axis_state **state)
{
    for (int i = 0; i < leftdim(); ++i)
    {
        settings[i].controlled = state[i];
        settings[i].controlled->attach_possible_controller(this);
    }
}

void axis_controller::finish_trajectory(disctime_t time, position_t *ctrpos)
{
    u.f.operation_finished_flag = 1;
    if (operation_finish_handler)
        operation_finish_handler(operation_handlers_priv, this);
    lintraj.set_point_hold(time, ctrpos);
    curtraj = &lintraj;
    u.f.release_control_flag = 1;
}

int axis_controller::feedback(disctime_t)
{
    return 0;
}

int axis_controller::serve(disctime_t time)
{
    std::vector<position_t> ctrpos(leftdim());
    std::vector<velocity_t> ctrvel(leftdim());

    if (u.f.release_control_flag)
    {
        u.f.release_control_flag = 0;
        _deactivate(time);
        return 0;
    }

    for (int i = 0; i < leftdim(); ++i)
    {
        if (!settings[i].controlled->current_controller ||
            settings[i].controlled->current_controller != this)
        {
            return SIGNAL_PROCESSOR_RETURN_NOT_ACTIVE;
        }
    }

    if (!curtraj)
        return SIGNAL_PROCESSOR_RETURN_RUNTIME_ERROR;

    int sts = curtraj->attime(time, ctrpos.data(), ctrvel.data());

    for (int i = 0; i < leftdim(); ++i)
    {
        // Установить текущие целевые параметры.
        // Скорость умножается на частоту, чтобы вернуться от
        // дискретного времени, в которых работает расчёт траектории к секундам.
        settings[i].controlled->ctrpos = ctrpos[i];
        settings[i].controlled->ctrvel = ctrvel[i] * discrete_time_frequency();
    }

    if (sts && !u.f.operation_finished_flag)
    {
        finish_trajectory(time, ctrpos.data());
    }

    return 0;
}

int axis_controller::_absmove(disctime_t curtim,
                              position_t *curpos,
                              position_t *tgtpos,
                              double extdist)
{
    if (is_active())
    {
        ralgo::warn("stop instead");
        return stop(curtim);
    }

    for (int i = 0; i < leftdim(); ++i)
        if (settings[i].controlled->current_controller)
        {
            ralgo::warn("already controlled");
            return -1;
        }

    if (activate(curtim))
    {
        ralgo::warn("axisctr activation is fault");
        return -1;
    }

    disctime_t tgttim =
        curtim + (double)(ABS(extdist)) / (vel / discrete_time_frequency());
    if ((tgttim - curtim == 0) && (tgtpos - curpos != 0))
        tgttim = curtim + 1;

    if (extdist == 0 || vel == 0 || curtim == tgttim)
    {
        finish_trajectory(curtim, curpos);
        return 0;
    }

    disctime_t acc_time = (vel / acc) * discrete_time_frequency();
    disctime_t dcc_time = (vel / dcc) * discrete_time_frequency();

    lintraj.init_nominal_speed(curtim,
                               tgttim,
                               curpos,
                               tgtpos,
                               acc_time,
                               dcc_time,
                               u.f.spattern_enabled);

    u.f.operation_finished_flag = 0;
    u.f.release_control_flag = 0;
    if (operation_start_handler)
        operation_start_handler(operation_handlers_priv, this);
    curtraj = &lintraj;
    return 0;
}

int axis_controller::incmove(disctime_t current_time, const double *dist_real)
{
    std::vector<position_t> curpos(leftdim());
    std::vector<position_t> tgtpos(leftdim());
    double extdist_accumulator = 0;

    for (int i = 0; i < leftdim(); ++i)
    {
        position_t dist = dist_real[i] * settings[i].gain;

        curpos[i] = settings[i].controlled->feedpos;
        tgtpos[i] = curpos[i] + dist;

        if (settings[i].limits_enabled)
            tgtpos[i] = CLAMP(tgtpos[i],
                              settings[i].backlim * settings[i].gain,
                              settings[i].forwlim * settings[i].gain);

        double extdist = (tgtpos[i] - curpos[i]) / settings[i].gain;
        extdist_accumulator += extdist * extdist;
    }

    return _absmove(
        current_time, curpos.data(), tgtpos.data(), sqrt(extdist_accumulator));
}

int axis_controller::absmove(disctime_t current_time, const double *pos_real)
{
    std::vector<position_t> curpos(leftdim());
    std::vector<position_t> tgtpos(leftdim());
    double extdist_accumulator = 0;

    for (int i = 0; i < leftdim(); ++i)
    {
        curpos[i] = settings[i].controlled->feedpos;
        tgtpos[i] = pos_real[i] * settings[i].gain;

        if (settings[i].limits_enabled)
            tgtpos[i] = CLAMP(tgtpos[i],
                              settings[i].backlim * settings[i].gain,
                              settings[i].forwlim * settings[i].gain);

        double extdist = (tgtpos[i] - curpos[i]) / settings[i].gain;
        extdist_accumulator += extdist * extdist;
    }

    return _absmove(
        current_time, curpos.data(), tgtpos.data(), sqrt(extdist_accumulator));
}

double axis_controller::ctrpos_external(int axno)
{
    return settings[axno].controlled->ctrpos / settings[axno].gain;
}

double axis_controller::feedpos_external(int axno)
{
    return settings[axno].controlled->feedpos / settings[axno].gain;
}

double axis_controller::ctrvel_external(int axno)
{
    return settings[axno].controlled->ctrvel / settings[axno].gain;
}

axis_controller *heimer::create_axis_controller(const char *name)
{
    axis_controller *ptr = new heimer::axis_controller(name);
    return ptr;
}

void axis_controller::release_controlled()
{
    for (int i = 0; i < leftdim(); ++i)
    {
        if (settings[i].controlled)
        {
            settings[i].controlled->detach_possible_controller(this);
            settings[i].controlled = NULL;
        }
    }
}

void axis_controller::deinit()
{
    release_controlled();
}

signal_head *axis_controller::iterate_left(signal_head *iter)
{
    if (iter == NULL)
        return settings[0].controlled;

    for (int i = 0; i < leftdim() - 1; ++i)
    {
        if (iter == settings[i].controlled)
        {
            return settings[i + 1].controlled;
        }
    }

    return NULL;
}

signal_head *axis_controller::iterate_right(signal_head *iter)
{
    (void)iter;
    return NULL;
}

double axis_controller::external_velocity()
{
    return vel;
}
double axis_controller::external_acceleration()
{
    return acc;
}
double axis_controller::external_decceleration()
{
    return dcc;
}

void axis_controller::collect_feedpos(position_t *pos)
{
    for (int i = 0; i < leftdim(); ++i)
        pos[i] = settings[i].controlled->feedpos;
}

void axis_controller::collect_feedvel(velocity_t *pos)
{
    for (int i = 0; i < leftdim(); ++i)
        pos[i] = settings[i].controlled->feedvel;
}

int axis_controller::stop(disctime_t curtim)
{
    if (!is_active())
    {
        ralgo::warn(name().data(), ": not active");
        return -1;
    }

    std::vector<position_t> feedpos(leftdim());
    std::vector<velocity_t> feedspd(leftdim());

    collect_feedpos(feedpos.data());
    collect_feedvel(feedspd.data());

    for (int i = 0; i < leftdim(); ++i)
        feedspd[i] /= discrete_time_frequency();

    if (curtraj == nullptr)
        return 0;

    // Время, которое потребуется на остановку в дискретных единицах.
    double stoptime = restore_internal_velocity_from_axstates() / dcc *
                      discrete_time_frequency();

    lintraj.set_stop_pattern(feedpos.data(), feedspd.data(), curtim, stoptime);

    u.f.operation_finished_flag = 0;
    u.f.release_control_flag = 0;
    if (operation_start_handler)
        operation_start_handler(operation_handlers_priv, this);
    curtraj = &lintraj;

    return 0;
}

velocity_t axis_controller::restore_internal_velocity_from_axstates()
{
    velocity_t acc = 0;

    for (int i = 0; i < leftdim(); ++i)
    {
        velocity_t vel = settings[i].controlled->feedvel / settings[i].gain;
        acc += vel * vel;
    }

    return sqrt(acc);
}

int axis_controller::hardstop(disctime_t time)
{
    std::vector<position_t> curpos(leftdim());
    for (int i = 0; i < leftdim(); ++i)
    {
        curpos[i] = settings[i].controlled->feedpos;
    }

    for (int i = 0; i < leftdim(); ++i)
    {
        settings[i].controlled->ctrvel = 0;
        settings[i].controlled->ctrpos = curpos[i];
    }

    finish_trajectory(time, curpos.data());
    return 0;
}

void axis_controller::reinit()
{
    vel = 0;
    acc = 0;
    dcc = 0;
    signal_processor::u.f.is_axisctr = 1;

    operation_start_handler = NULL;
    operation_finish_handler = NULL;
    operation_handlers_priv = NULL;

    approvals = NULL;
    approvals_total = 0;

    curtraj = NULL;

    lintraj.init(1, &settings[0].sfpos, leftdim());
}

axis_controller::axis_controller(const std::string &name)
    : signal_processor(name, 0, 0)
{
    //_init();
}

axis_controller::axis_controller(const std::string &name,
                                 const std::vector<axis_state *> &states)
    : signal_processor(name, states.size(), 0)
{
    settings.resize(states.size());
    for (unsigned int i = 0; i < states.size(); ++i)
    {
        settings[i].controlled = states[i];
        settings[i].controlled->attach_possible_controller(this);
    }
    reinit();
}

void axis_controller::add_state(axis_state *state)
{
    settings.push_back(axis_settings());
    settings.back().controlled = state;
    settings.back().controlled->attach_possible_controller(this);
    set_dims(settings.size(), 0);
    reinit();
}

bool axis_controller::on_interrupt(disctime_t time)
{
    ralgo::warn("axisctr:", name().data(), " - hardstop by interrupt.");

    hardstop(time);
    return false;
}

int axis_controller::incmove(disctime_t current_time,
                             const std::initializer_list<double> &dist_real)
{
    return incmove(current_time, &*dist_real.begin());
}

int axis_controller::absmove(disctime_t current_time,
                             const std::initializer_list<double> &pos_real)
{
    return absmove(current_time, &*pos_real.begin());
}

static inline int
setvel(axis_controller *axctr, int argc, char **argv, char *output, int outmax)
{
    if (argc < 1)
    {
        snprintf(output, outmax, "Need argument");
        return -1;
    }

    axctr->set_velocity_external(atof(argv[0]));
    return 0;
}

static inline int
setacc(axis_controller *axctr, int argc, char **argv, char *output, int outmax)
{
    if (argc < 1)
    {
        snprintf(output, outmax, "Need argument");
        return -1;
    }

    axctr->set_acceleration_external(atof(argv[0]));

    if (argc > 1)
        axctr->set_decceleration_external(atof(argv[1]));
    return 0;
}

static inline int
setlim(axis_controller *axctr, int argc, char **argv, char *output, int outmax)
{
    if (argc != axctr->leftdim() * 2)
    {
        snprintf(output,
                 outmax,
                 "Can't bind %d symbols for %d leftdim() axisctr",
                 argc,
                 axctr->leftdim());
        return -1;
    }

    for (int i = 0; i < axctr->leftdim(); ++i)
    {
        axctr->settings[i].backlim = atof(argv[i * 2]);
        axctr->settings[i].forwlim = atof(argv[i * 2 + 1]);
    }

    return 0;
}

static inline int feed(axis_controller *axctr, int, char **, char *output, int)
{
    for (int i = 0; i < axctr->leftdim(); ++i)
    {
        char buf[64];
        axctr->settings[i].controlled->info(buf, 64);
        strcat(output, buf);
    }

    return 0;
}

static inline int
setdcc(axis_controller *axctr, int argc, char **argv, char *output, int outmax)
{
    if (argc < 1)
    {
        snprintf(output, outmax, "Need argument");
        return -1;
    }

    axctr->set_acceleration_external(atof(argv[0]));
    return 0;
}

static inline int
bind(axis_controller *axctr, int argc, char **argv, char *output, int outmax)
{
    if (argc != axctr->leftdim())
    {
        snprintf(output,
                 outmax,
                 "Can't bind %d symbols for %d leftdim() axisctr",
                 argc,
                 axctr->leftdim());
        return -1;
    }

    {
        std::vector<axis_state *> arr(argc);

        for (int i = 0; i < argc; ++i)
        {
            signal_head *sig = signal_get_by_name(argv[i]);

            if (!sig)
            {
                snprintf(output,
                         outmax,
                         "Wrong signal name '%s' (type 'siglist' for display)",
                         argv[i]);
                return -1;
            }

            if (sig->type != SIGNAL_TYPE_AXIS_STATE)
            {
                snprintf(output,
                         outmax,
                         "Wrong signal type. name:(%s)",
                         sig->name.c_str());
                return -1;
            }

            arr[i] = static_cast<axis_state *>(sig);
        }

        axctr->release_controlled();
        axctr->set_controlled(arr.data());
    }

    return 0;
}

static inline int
setgain(axis_controller *axctr, int argc, char **argv, char *output, int outmax)
{
    if (argc != axctr->leftdim())
    {
        snprintf(output,
                 outmax,
                 "Can't bind %d symbols for %d leftdim() axisctr",
                 argc,
                 axctr->leftdim());
        return -1;
    }

    for (int i = 0; i < argc; ++i)
    {
        axctr->settings[i].gain = atof(argv[i]);
    }

    return 0;
}

static inline int
info(axis_controller *axctr, int argc, char **argv, char *output, int outmax)
{
    (void)argc;
    (void)argv;

    constexpr int bufsize = 96;
    char buf[bufsize];

    memset(buf, 0, bufsize);
    snprintf(buf, bufsize, "isactive:%d, ", axctr->is_active());
    strncat(output, buf, outmax);

    memset(buf, 0, bufsize);
    snprintf(buf, bufsize, "leftdim():%d, ", axctr->leftdim());
    strncat(output, buf, outmax);

    memset(buf, 0, bufsize);
    snprintf(buf, bufsize, "signals:");
    strncat(output, buf, outmax);

    for (int i = 0; i < axctr->leftdim() - 1; ++i)
    {
        memset(buf, 0, bufsize);
        if (axctr->settings[i].controlled == nullptr)
        {
            snprintf(buf, bufsize, "(none),");
            strncat(output, buf, outmax);
        }
        else
        {
            snprintf(buf,
                     bufsize,
                     "%s,",
                     axctr->settings[i].controlled->name.c_str());
            strncat(output, buf, outmax);
        }
    }
    memset(buf, 0, bufsize);
    snprintf(buf,
             bufsize,
             "%s\r\n",
             axctr->settings[axctr->leftdim() - 1].controlled->name.c_str());
    strncat(output, buf, outmax);

    memset(buf, 0, bufsize);
    nos::format_buffer(buf,
                       "external: vel:{}, acc:{}, dcc:{}\r\n",
                       axctr->external_velocity(),
                       axctr->external_acceleration(),
                       axctr->external_decceleration());
    strncat(output, buf, outmax);

    memset(buf, 0, bufsize);
    snprintf(
        buf,
        bufsize,
        "flags: opfinished:%d, releaseflag:%d, dynamic:%d, spattern:%d\r\n",
        axctr->u.f.operation_finished_flag,
        axctr->u.f.release_control_flag,
        axctr->u.f.dynamic_resources,
        (uint8_t)axctr->u.f.spattern_enabled);
    strncat(output, buf, outmax);

    for (int i = 0; i < axctr->leftdim(); ++i)
    {
        memset(buf, 0, bufsize);
        nos::format_buffer(buf,
                           "axsets: lims: {},{},{} sfpos: {},{} gain: {}\r\n",
                           axctr->settings[i].limits_enabled,
                           axctr->settings[i].backlim,
                           axctr->settings[i].forwlim,
                           axctr->settings[i].sfpos.spos,
                           axctr->settings[i].sfpos.fpos,
                           axctr->settings[i].gain);
        strncat(output, buf, outmax);
    }

    return 0;
}

static inline int
incmov(axis_controller *axctr, int argc, char **argv, char *output, int outmax)
{
    if (argc != axctr->leftdim())
    {
        snprintf(output,
                 outmax,
                 "Can't use %d coord for %d leftdim() axisctr",
                 argc,
                 axctr->leftdim());
        return -1;
    }

    int dim = axctr->leftdim();
    std::vector<position_t> dist(dim);

    for (int i = 0; i < dim; ++i)
    {
        dist[i] = atof(argv[i]);
    }

    return axctr->incmove(discrete_time(), dist.data());
}

static inline int
absmov(axis_controller *axctr, int argc, char **argv, char *output, int outmax)
{
    if (argc != axctr->leftdim())
    {
        snprintf(output,
                 outmax,
                 "Can't use %d coord for %d leftdim() axisctr",
                 argc,
                 axctr->leftdim());
        return -1;
    }

    int dim = axctr->leftdim();
    std::vector<position_t> pos(dim);

    for (int i = 0; i < dim; ++i)
    {
        pos[i] = atof(argv[i]);
    }
    return axctr->absmove(discrete_time(), pos.data());
}

static inline int stop(axis_controller *axctr, int, char **, char *, int)
{
    return axctr->stop(discrete_time());
}

static inline int hardstop(axis_controller *axctr, int, char **, char *, int)
{
    return axctr->hardstop(discrete_time());
}

int axis_controller::command(int argc, char **argv, char *output, int outmax)
{
    int status = ENOENT;

    if (strcmp("bind", argv[0]) == 0)
        status = bind(this, argc - 1, argv + 1, output, outmax);

    else if (strcmp("help", argv[0]) == 0)
        status = help(output, outmax);

    else if (strcmp("info", argv[0]) == 0)
        status = ::info(this, argc - 1, argv + 1, output, outmax);

    else if (strcmp("absmove", argv[0]) == 0 || strcmp(argv[0], "mov") == 0)
        status = absmov(this, argc - 1, argv + 1, output, outmax);

    else if (strcmp("incmove", argv[0]) == 0 || strcmp(argv[0], "incmov") == 0)
        status = incmov(this, argc - 1, argv + 1, output, outmax);

    else if (strcmp("stop", argv[0]) == 0)
        status = ::stop(this, argc - 1, argv + 1, output, outmax);

    else if (strcmp("hardstop", argv[0]) == 0)
        status = ::hardstop(this, argc - 1, argv + 1, output, outmax);

    else if (strcmp("setvel", argv[0]) == 0 || strcmp(argv[0], "setspd") == 0)
        status = setvel(this, argc - 1, argv + 1, output, outmax);

    else if (strcmp("setacc", argv[0]) == 0)
        status = setacc(this, argc - 1, argv + 1, output, outmax);

    else if (strcmp("setdcc", argv[0]) == 0)
        status = setdcc(this, argc - 1, argv + 1, output, outmax);

    else if (strcmp("setgain", argv[0]) == 0)
        status = setgain(this, argc - 1, argv + 1, output, outmax);

    else if (strcmp("setlim", argv[0]) == 0)
        status = setlim(this, argc - 1, argv + 1, output, outmax);

    else if (strcmp("feed", argv[0]) == 0)
        status = feed(this, argc - 1, argv + 1, output, outmax);

    else if (strcmp("name", argv[0]) == 0)
    {
        snprintf(output, outmax, "%s\r\n", name().data());
        status = 0;
    }

    if (status == ENOENT)
        snprintf(output, outmax, "axisctr: Command is not found\r\n");

    return status;
}

int axis_controller::help(char *output, int outmax)
{
    snprintf(output,
             outmax,
             "bind\r\n"
             "info\r\n"
             "absmove\r\n"
             "incmove\r\n"
             "stop\r\n"
             "hardstop\r\n"
             "setvel\r\n"
             "setacc\r\n"
             "setdcc\r\n"
             "setgain\r\n");

    return 0;
}
