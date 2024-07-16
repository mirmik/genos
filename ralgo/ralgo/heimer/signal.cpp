#include <igris/shell/rshell.h>
#include <ralgo/heimer/signal.h>
#include <ralgo/heimer/signal_processor.h>
#include <ralgo/log.h>
#include <string.h>

#include <errno.h>
#include <nos/fprint.h>

#include <igris/datastruct/nametbl.h>

using namespace heimer;

DLIST_HEAD(heimer::signals_list);

signal_head::signal_head(uint8_t type) : signal_head("undef", type) {}

void signal_head::init(const std::string &name, uint8_t type)
{
    refs = 0;
    this->type = type;
    set_name(name);
    dlist_move_tail(&list_lnk, &signals_list);
    current_controller = NULL;
    listener = NULL;
    sorting_mark = 0;
}

void signal_head::deinit()
{
    dlist_del_init(&list_lnk);
}

void signal_head::set_name(const std::string &name)
{
    this->name = name;
}

int signal_head::attach_listener(signal_processor *proc)
{
    if (listener)
        return -1;

    listener = proc;
    refs++;

    return 0;
}

void signal_head::attach_possible_controller(signal_processor *)
{
    refs++;
}

int signal_head::detach_listener(signal_processor *proc)
{
    if (listener != proc)
        return -1;

    listener = nullptr;
    refs--;

    return 0;
}

void signal_head::detach_possible_controller(signal_processor *)
{
    refs--;
}

int heimer::signals_count()
{
    return dlist_size(&signals_list);
}

void heimer::signal_head_list_reinit()
{
    while (!dlist_empty(&signals_list))
    {
        dlist_del_init(signals_list.next);
    }

    dlist_init(&signals_list);
}

signal_head *heimer::signal_get_by_name(const char *name)
{
    signal_head *sig;
    dlist_for_each_entry(sig, &signals_list, list_lnk)
    {
        if (sig->name == name)
            return sig;
    }
    return NULL;
}

int signal_head::command_v(int argc, char **argv, char *output, int maxsize)
{
    (void)argc;
    char *opsname = argv[0];

    if (strcmp(opsname, "info") == 0)
        return info(output, maxsize);

    if (strcmp(opsname, "ctrinfo") == 0)
        return ctrinfo(output, maxsize);

    return ENOENT;
}

int signal_head::ctrinfo(char *buffer, int)
{
    const char *lname = listener ? listener->name().data() : "(null)";
    const char *cname =
        current_controller ? current_controller->name().data() : "(null)";

    nos::format_buffer(buffer, "listener:{}, controller:{}\r\n", lname, cname);
    return 0;
}

void signal_head::rebind()
{
    dlist_add_tail(&list_lnk, &signals_list);
}

int signal_head::activate(signal_processor *proc, disctime_t tim)
{
    if (current_controller)
    {
        // Если это вторичная попытка активации того же контроллера, передаём,
        // что продолжаем работу штатно.
        if (current_controller == proc)
        {
            ralgo::warn("signal is reactivated from curcontroller name: ",
                        name.c_str());
            return 0;
        }

        // Но, если это другой контрорллер, отдаём отказ.
        else
            return -1;
    }

    if (!listener)
    {
        ralgo::warn("trying to activate over signal without listener");
        return -1;
    }

    if (listener->activate(tim))
    {
        if (debug_activations)
        {
            ralgo::warn("fault in controller activation",
                        (const char *)listener->name().data());
        }
        return -1;
    }

    current_controller = proc;
    return 0;
}

int signal_head::deactivate(signal_processor *proc, disctime_t time)
{
    if (proc != current_controller)
        return -1;

    current_controller = NULL;

    if (listener)
    {
        return listener->deactivation_request(time);
    }

    return 0;
}

void signal_head::provide_interrupt(disctime_t time)
{
    if (current_controller)
        current_controller->interrupt(time, false);
}
