#include <ralgo/heimer/dof6_controller.h>
#include <ralgo/heimer/sigtypes.h>

using namespace heimer;

dof6_controller::dof6_controller(const char *name)
    : signal_processor(name, 1, 0)
{
}

signal_head *dof6_controller::iterate_left(signal_head *iter)
{
    return iter == nullptr ? controlled : nullptr;
}

signal_head *dof6_controller::iterate_right(signal_head *)
{
    return nullptr;
}

void dof6_controller::set_controlled(dof6_signal *controlled)
{
    this->controlled = controlled;
}

void dof6_controller::set_velocity(double v)
{
    vel = v;
}

void dof6_controller::set_acceleration(double v)
{
    acc = v;
}

void dof6_controller::set_decceleration(double v)
{
    dcc = v;
}

int dof6_controller::feedback(disctime_t)
{
    return 0;
}

int dof6_controller::serve(disctime_t)
{
    return 0;
}

static inline int
setvel(dof6_controller *axctr, int argc, char **argv, char *output, int outmax)
{
    if (argc < 1)
    {
        snprintf(output, outmax, "Need argument");
        return -1;
    }

    axctr->set_velocity(atof(argv[0]));
    return 0;
}

static inline int
setacc(dof6_controller *axctr, int argc, char **argv, char *output, int outmax)
{
    if (argc < 1)
    {
        snprintf(output, outmax, "Need argument");
        return -1;
    }

    axctr->set_acceleration(atof(argv[0]));

    if (argc > 1)
        axctr->set_decceleration(atof(argv[1]));
    return 0;
}

static inline int
setdcc(dof6_controller *axctr, int argc, char **argv, char *output, int outmax)
{
    if (argc < 1)
    {
        snprintf(output, outmax, "Need argument");
        return -1;
    }

    axctr->set_acceleration(atof(argv[0]));
    return 0;
}

/*static inline
int absmove(dof6_controller * axctr, int argc, char ** argv, char * output, int
outmax)
{
    if (argc < 6)
    {
        snprintf(output, outmax, "Need argument");
        return -1;
    }

    axctr->absmove(atof(argv[0]), atof(argv[1]), atof(argv[2]), atof(argv[3]),
atof(argv[4]), atof(argv[5])); return 0;
}*/

int dof6_controller::command(int argc, char **argv, char *output, int outmax)
{
    int status = ENOENT;

    //	else if (strcmp("info", argv[0]) == 0)
    //		status = info(this, argc - 1, argv + 1, output, outmax);

    //	if (strcmp("absmove", argv[0]) == 0)
    //		status = ::absmove(this, argc - 1, argv + 1, output, outmax);

    //	else if (strcmp("incmove", argv[0]) == 0)
    //		status = incmov(this, argc - 1, argv + 1, output, outmax);

    //	else if (strcmp("stop", argv[0]) == 0)
    //		status = ::stop(this, argc - 1, argv + 1, output, outmax);

    //	else if (strcmp("hardstop", argv[0]) == 0)
    //		status = ::hardstop(this, argc - 1, argv + 1, output, outmax);

    if (strcmp("setvel", argv[0]) == 0)
        status = setvel(this, argc - 1, argv + 1, output, outmax);

    else if (strcmp("setacc", argv[0]) == 0)
        status = setacc(this, argc - 1, argv + 1, output, outmax);

    else if (strcmp("setdcc", argv[0]) == 0)
        status = setdcc(this, argc - 1, argv + 1, output, outmax);

    //	else if (strcmp("setgain", argv[0]) == 0)
    //		status = setgain(this, argc - 1, argv + 1, output, outmax);

    if (status != ENOENT)
    {
        return status;
    }

    return signal_processor::command(argc, argv, output, outmax);
}

void dof6_controller::deinit() {}

int dof6_controller::leftsigtype(int)
{
    return SIGNAL_TYPE_DOF6;
}
signal_head *dof6_controller::leftsig(int)
{
    return controlled;
}
void dof6_controller::set_leftsig(int, signal_head *sig)
{
    controlled = static_cast<dof6_signal *>(sig);
}
