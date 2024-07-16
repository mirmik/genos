#include <ralgo/heimer/axis_stub_processor.h>
#include <ralgo/heimer/sigtypes.h>

heimer::axis_stub_processor::axis_stub_processor(const std::string &name)
    : signal_processor(name, 0, 1)
{
}

heimer::axis_stub_processor::axis_stub_processor(
    const std::string &name, const std::vector<axis_state *> &axstates)
    : signal_processor(name, 0, axstates.size()), axstates(axstates)
{
    for (auto axstate : axstates)
        bind(axstate);
}

int heimer::axis_stub_processor::feedback(disctime_t time)
{
    for (auto _axstate : axstates)
    {
        auto pos = _axstate->ctrpos;
        auto vel = _axstate->ctrvel;

        if (_apply_speed_mode)
        {
            disctime_t delta = time - lasttime;
            pos = pos + vel * delta / discrete_time_frequency();
        }

        _axstate->feedpos = pos;
        _axstate->feedvel = vel;
    }

    lasttime = time;
    return 0;
}

int heimer::axis_stub_processor::serve(disctime_t)
{
    return 0;
}

static inline int bind(heimer::axis_stub_processor *axctr,
                       int argc,
                       char **argv,
                       char *output,
                       int outmax)
{
    if (argc < 1)
    {
        snprintf(output, outmax, "Need argument");
        return -1;
    }

    heimer::signal_head *sig = heimer::signal_get_by_name(argv[0]);

    if (!sig)
    {
        snprintf(output,
                 outmax,
                 "Wrong signal name '%s' (type 'siglist' for display)",
                 argv[0]);
        return -1;
    }

    if (sig->type != SIGNAL_TYPE_AXIS_STATE)
    {
        snprintf(
            output, outmax, "Wrong signal type. name:(%s)", sig->name.c_str());
        return -1;
    }

    axctr->bind(static_cast<heimer::axis_state *>(sig));

    return 0;
}

static inline int
info(heimer::axis_stub_processor *axctr, int, char **, char *output, int outmax)
{
    snprintf(output, outmax, "is_active: %d\r\n", axctr->is_active());
    return 0;
}

int heimer::axis_stub_processor::command(int argc,
                                         char **argv,
                                         char *output,
                                         int outmax)
{
    int status = ENOENT;

    if (strcmp("bind", argv[0]) == 0)
        status = ::bind(this, argc - 1, argv + 1, output, outmax);

    if (strcmp("info", argv[0]) == 0)
        status = ::info(this, argc - 1, argv + 1, output, outmax);

    if (strcmp("applyspeed", argv[0]) == 0)
    {
        apply_speed_mode(true);
        status = 0;
    }

    return status;
}

void heimer::axis_stub_processor::deinit()
{
    for (auto axstate : axstates)
        axstate->detach_listener(this);
    axstates.clear();
}

heimer::signal_head *
heimer::axis_stub_processor::iterate_left(heimer::signal_head *)
{
    return NULL;
}

heimer::signal_head *
heimer::axis_stub_processor::iterate_right(heimer::signal_head *iter)
{
    if (iter == NULL)
        return axstates.front();

    for (auto it = axstates.begin(); it != axstates.end(); ++it)
    {
        if (*it == iter)
        {
            if (++it == axstates.end())
                return NULL;
            else
                return *it;
        }
    }
    return NULL;
}

void heimer::axis_stub_processor::bind(heimer::axis_state *iter)
{
    iter->attach_listener(this);
}

void heimer::axis_stub_processor::apply_speed_mode(bool en)
{
    _apply_speed_mode = en;
}

void heimer::axis_stub_processor::on_activate(disctime_t time)
{
    lasttime = time;
}
