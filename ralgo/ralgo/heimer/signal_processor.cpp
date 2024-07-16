#include <ralgo/heimer/executor.h>
#include <ralgo/heimer/signal.h>
#include <ralgo/heimer/signal_processor.h>
#include <ralgo/log.h>
#include <ralgo/util/helpers.h>

#include <igris/math.h>
#include <igris/shell/rshell.h>
#include <igris/util/bug.h>

#include <nos/print.h>

#include <assert.h>
#include <string.h>

using namespace heimer;

DLIST_HEAD(heimer::signal_processor_list);
bool heimer::debug_activations = false;

void signal_processor::set_dims(int ldim, int rdim)
{
    _leftdim = ldim;
    _rightdim = rdim;
}

signal_processor::signal_processor(const std::string &name, int ldim, int rdim)
{
    set_dims(ldim, rdim);
    set_name(name);
    rebind();
    this->u.flags = 0;
}

signal_processor::signal_processor(int ldim, int rdim)
    : signal_processor("undef", ldim, rdim)
{
}

void signal_processor::rebind()
{
    dlist_add_tail(&list_lnk, &signal_processor_list);
}

void signal_processor::set_name(const std::string &name)
{
    _name = name;
}

void signal_processor::deinit()
{
    dlist_del(&list_lnk);
}

int heimer::signal_processors_count()
{
    return dlist_size(&signal_processor_list);
}

void heimer::signal_processors_list_reinit()
{
    while (!dlist_empty(&signal_processor_list))
    {
        dlist_del_init(signal_processor_list.next);
    }

    dlist_init(&signal_processor_list);
}

int signal_processor::activate(disctime_t curtim)
{
    int success = 1;

    if (ralgo::global_protection)
        return -1;

    if (is_active())
        return 0;

    signal_head *iter = NULL;
    while ((iter = iterate_left(iter)))
    {
        int err = iter->activate(this, curtim);
        if (err)
        {
            success = 0;
            break;
        }
    }

    if (success)
    {
        u.f.active = 1;
        on_activate(curtim);

        if (heimer::debug_activations)
        {
            ralgo::info("active:", name().data());
        }

        return 0;
    }

    else
    {
        _deactivate(curtim);
        return -1;
    }
}

int signal_processor::_deactivate(disctime_t curtim)
{
    signal_head *iter = NULL;

    // Передаём команду деактивации на нижние уровни через подчинённые сигналы.
    while ((iter = iterate_left(iter)))
    {
        int err = iter->deactivate(this, curtim);
        (void)err;
    }
    u.f.active = 0;
    if (heimer::debug_activations)
    {
        ralgo::info("deactive:", name().data());
    }

    return 0;
}

int signal_processor::on_deactivation_request(disctime_t curtim)
{
    return _deactivate(curtim);
}

int signal_processor::deactivation_request(disctime_t curtim,
                                           bool ignore_request)
{
    signal_head *iter;
    int all_right_deactivated = 1;

    iter = NULL;
    while ((iter = iterate_right(iter)))
    {
        if (iter->current_controller)
            all_right_deactivated = 0;
    }

    if (!all_right_deactivated)
        return -1;

    if (!ignore_request)
    {
        return on_deactivation_request(curtim);
    }
    else
    {
        return _deactivate(curtim);
    }
}

signal_processor *heimer::signal_processor_get_by_name(const char *name)
{
    signal_processor *sig;
    dlist_for_each_entry(sig, &signal_processor_list, list_lnk)
    {
        if (strncmp(sig->name().data(), name, SIGNAL_NAME_MAX_LENGTH) == 0)
            return sig;
    }
    return NULL;
}

nos::buffer signal_processor::name()
{
    return {_name.data(), _name.size()};
}

bool signal_processor::is_active()
{
    return u.f.active;
}

void signal_processor::on_activate(disctime_t) {}

bool signal_processor::is_dynamic_resources()
{
    return u.f.dynamic_resources;
}

void signal_processor::set_dynamic_resources_flag(bool en)
{
    u.f.dynamic_resources = en;
}

void signal_processor::release_signals()
{
    signal_head *iter;

    iter = nullptr;
    while ((iter = iterate_left(iter)))
    {
        iter->detach_possible_controller(this);
    }

    iter = nullptr;
    while ((iter = iterate_left(iter)))
    {
        iter->detach_listener(this);
    }
}

static int bindleft(
    signal_processor *axctr, int argc, char **argv, char *output, int outmax)
{
    if (argc != axctr->leftdim())
    {
        snprintf(output,
                 outmax,
                 "Can't bind %d symbols for %d _dim axisctr",
                 argc,
                 axctr->leftdim());
        return -1;
    }

    {
        TEMPORARY_STORAGE(signal_head *, argc, arr);

        for (int i = 0; i < argc; ++i)
        {
            signal_head *sig = signal_get_by_name(argv[i]);

            if (!sig)
            {
                snprintf(output,
                         outmax,
                         "Wrong signal name '%s ' (type 'siglist' for display)",
                         argv[i]);
                return -1;
            }

            if (sig->type != axctr->leftsigtype(i))
            {
                snprintf(output,
                         outmax,
                         "Wrong signal type. name:(%s)",
                         sig->name.c_str());
                return -1;
            }

            arr[i] = sig;
        }

        axctr->set_leftside(std::data(arr));
    }

    return 0;
}

static int bindright(
    signal_processor *axctr, int argc, char **argv, char *output, int outmax)
{
    if (argc != axctr->rightdim())
    {
        snprintf(output,
                 outmax,
                 "Can't bind %d symbols for %d _dim axisctr",
                 argc,
                 axctr->rightdim());
        return -1;
    }

    {
        TEMPORARY_STORAGE(signal_head *, argc, arr);

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

            if (sig->type != axctr->rightsigtype(i))
            {
                snprintf(output,
                         outmax,
                         "Wrong signal type. name:(%s)",
                         sig->name.c_str());
                return -1;
            }

            arr[i] = sig;
        }

        axctr->set_rightside(std::data(arr));
    }

    return 0;
}

int signal_processor::info(char *output, int outmax)
{
    snprintf(output, outmax, "Info is not implemented\r\n");
    return 0;
}

int signal_processor::help(char *output, int outmax)
{
    snprintf(output, outmax, "TODO: Automatic help.\r\n");
    return 0;
}

int signal_processor::command(int argc, char **argv, char *output, int outmax)
{
    int status = ENOENT;

    if (strcmp("bindleft", argv[0]) == 0)
        status = ::bindleft(this, argc - 1, argv + 1, output, outmax);

    if (strcmp("bindright", argv[0]) == 0)
        status = ::bindright(this, argc - 1, argv + 1, output, outmax);

    if (strcmp("info", argv[0]) == 0)
        status = info(output, outmax);

    if (strcmp("help", argv[0]) == 0)
        status = help(output, outmax);

    return status;
}

static signal_head *_;
signal_head *signal_processor::leftsig(int)
{
    BUG();
    return _;
}

signal_head *signal_processor::rightsig(int)
{
    BUG();
    return _;
}

uint8_t signal_processor::leftdim()
{
    return _leftdim;
}

uint8_t signal_processor::rightdim()
{
    return _rightdim;
}

int leftsigtype(int)
{
    BUG();
    return 0;
}

int rightsigtype(int)
{
    BUG();
    return 0;
}

void signal_processor::set_leftside(signal_head **arr)
{
    for (int i = 0; i < leftdim(); ++i)
    {
        set_leftsig(i, arr[i]);
        leftsig(i)->attach_possible_controller(this);
    }
}

void signal_processor::set_rightside(signal_head **arr)
{
    for (int i = 0; i < rightdim(); ++i)
    {
        set_rightsig(i, arr[i]);
        rightsig(i)->attach_listener(this);
    }
}

int signal_processor::leftsigtype(int)
{
    BUG();
    return 0;
}

int signal_processor::rightsigtype(int)
{
    BUG();
    return 0;
}

void signal_processor::set_leftsig(int, signal_head *)
{
    BUG();
}

void signal_processor::set_rightsig(int, signal_head *)
{
    BUG();
}

signal_head *signal_processor::iterate_left(signal_head *iter)
{
    if (iter == NULL)
        return leftsig(0);

    for (int i = 0; i < leftdim() - 1; ++i)
    {
        if (iter == leftsig(i))
        {
            return leftsig(i + 1);
        }
    }

    return NULL;
}

signal_head *signal_processor::iterate_right(signal_head *iter)
{
    if (iter == NULL)
        return rightsig(0);

    for (int i = 0; i < rightdim() - 1; ++i)
    {
        if (iter == rightsig(i))
        {
            return rightsig(i + 1);
        }
    }

    return NULL;
}

void signal_processor::interrupt(disctime_t time, bool ignore_handle)
{
    if (!ignore_handle)
    {
        bool prevent = on_interrupt(time);
        if (prevent)
        {
            return;
        }
    }

    signal_head *iter = nullptr;
    while ((iter = iterate_right(iter)))
    {
        iter->provide_interrupt(time);
    }
}
