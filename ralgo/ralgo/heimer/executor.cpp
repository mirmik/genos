#include <ralgo/heimer/axis_state.h>
#include <ralgo/heimer/axisapi.h>
#include <ralgo/heimer/executor.h>
#include <ralgo/heimer/fast_cycle_device.h>
#include <ralgo/heimer/signal.h>
#include <ralgo/heimer/sigtypes.h>
#include <ralgo/log.h>

#include <igris/dprint.h>

void heimer::executor_class::set_order_table(signal_processor **order_table,
                                             int capacity,
                                             int size)
{
    this->order_table = order_table;
    this->order_table_capacity = capacity;
    this->order_table_size = size;
}

int heimer::executor_class::order_sort()
{
    signal_head *it;
    dlist_for_each_entry(it, &signals_list, list_lnk)
    {
        it->sorting_mark = 0;
        if (it->listener == nullptr)
        {
            ralgo::warn("signal without listener. name: ", it->name.c_str());
            return -1;
        }
    }

    for (int i = 0; i < order_table_size; ++i)
    {
        int j;
        signal_processor *sigproc = order_table[i];

        for (j = i; j < order_table_size; ++j)
        {
            sigproc = order_table[j];

            int has_unordered = 0;

            signal_head *h = nullptr;
            while ((h = sigproc->iterate_left(h)))
            {
                if (h->sorting_mark == 0)
                {
                    has_unordered++;
                }
            }

            if (has_unordered == 0)
            {
                break;
            }
        }

        if (j == order_table_size)
            break;

        order_table[j] = order_table[i];
        order_table[i] = sigproc;

        signal_head *h = nullptr;
        while ((h = sigproc->iterate_right(h)))
        {
            h->sorting_mark = 1;
        }
    }

    return 0;
}

void heimer::executor_class::append_processor(signal_processor *proc)
{
    if (order_table_size >= order_table_capacity)
        return;

    order_table[order_table_size++] = proc;
}

int heimer::executor_class::serve(disctime_t curtime)
{
    int retcode;

    for (int i = order_table_size - 1; i >= 0; --i)
    {
        // if (order_table[i]->need_activation() &&
        // !order_table[i]->is_active())
        if (!order_table[i]->is_active())
            continue;

        // serve исполняется только если контроллер включен.
        retcode = order_table[i]->serve(curtime);

        switch (retcode)
        {
        case SIGNAL_PROCESSOR_RETURN_OK:
            break;
        case SIGNAL_PROCESSOR_RETURN_NOT_ACTIVE:
            break;
        case SIGNAL_PROCESSOR_RETURN_RUNTIME_ERROR:
            return SIGNAL_PROCESSOR_RETURN_RUNTIME_ERROR;
        default:
            return SIGNAL_PROCESSOR_RETURN_RUNTIME_ERROR;
        }
    }

    return 0;
}

int heimer::executor_class::feedback(disctime_t curtime)
{
    int retcode;

    for (int i = 0; i < order_table_size; ++i)
    {
        retcode = order_table[i]->feedback(curtime);

        switch (retcode)
        {
        case SIGNAL_PROCESSOR_RETURN_OK:
            break;
        case SIGNAL_PROCESSOR_RETURN_NOT_ACTIVE:
            break;
        case SIGNAL_PROCESSOR_RETURN_RUNTIME_ERROR:
            return SIGNAL_PROCESSOR_RETURN_RUNTIME_ERROR;
        default:
            return SIGNAL_PROCESSOR_RETURN_RUNTIME_ERROR;
        }
    }

    return 0;
}

int heimer::executor_class::exec(disctime_t curtime)
{
    char buf[16];
    int retcode;

    if (global_protection)
        return 0;

    retcode = feedback(curtime);
    if (retcode)
    {
        sprintf(buf, "%d", retcode);
        ralgo::warn("executor: feedback set retcode ", buf);
    }

    retcode = serve(curtime);
    if (retcode)
    {
        sprintf(buf, "%d", retcode);
        ralgo::warn("executor: feedback set retcode ", buf);
    }

    return retcode;
}

heimer::executor_class::~executor_class()
{
    if (u.f.dynamic)
        delete[] order_table;
}

void heimer::executor_class::allocate_order_table(int size)
{
    if (order_table)
        delete[] order_table;

    order_table = new signal_processor *[size];
    order_table_size = 0;
    order_table_capacity = size;
    u.f.dynamic = 1;
}

heimer::executor_class heimer::executor;
int heimer::executor_command(int, char **argv, char *output, int maxsize)
{
    int status = ENOENT;

    if (strcmp("order", argv[0]) == 0)
    {
        if (!executor.allowed_to_execution)
        {
            executor.allocate_order_table(heimer::signal_processors_count());

            heimer::signal_processor *proc;
            dlist_for_each_entry(proc, &heimer::signal_processor_list, list_lnk)
            {
                executor.append_processor(proc);
            }
        }

        char buffer[28];
        memset(buffer, 0, 28);
        snprintf(buffer, 28, "size: %d\r\n", executor.order_table_size);
        strncat(output, buffer, maxsize);

        memset(buffer, 0, 28);
        snprintf(buffer, 28, "capacity: %d\r\n", executor.order_table_capacity);
        strncat(output, buffer, maxsize);

        for (int i = executor.order_table_size - 1; i >= 0; --i)
        {
            memset(buffer, 0, 28);
            snprintf(buffer,
                     28,
                     "%*s\r\n",
                     (int)executor.order_table[i]->name().size(),
                     executor.order_table[i]->name().data());
            strncat(output, buffer, maxsize);
        }

        status = 0;
    }

    if (strcmp("start", argv[0]) == 0)
    {
        if (executor.allowed_to_execution)
        {
            snprintf(output, maxsize, "executor: it runned\r\n");
            return 0;
        }

        executor.allocate_order_table(heimer::signal_processors_count());

        heimer::signal_processor *proc;
        dlist_for_each_entry(proc, &heimer::signal_processor_list, list_lnk)
        {
            executor.append_processor(proc);
        }

        collect_axis_api();
        executor.activate_process();
        global_protection = false;
        return 0;
    }

    if (strcmp("stop", argv[0]) == 0)
    {
        executor.deactivate_process();
        return 0;
    }

    if (status == ENOENT)
    {
        snprintf(output, maxsize, "executor: unresolved command\r\n");
    }

    return status;
}

void heimer::executor_class::execute_if_allowed(disctime_t time)
{
    if (allowed_to_execution)
        exec(time);
}

void heimer::executor_class::activate_process()
{
    allowed_to_execution = true;
}

void heimer::executor_class::deactivate_process()
{
    allowed_to_execution = false;
}

void heimer::executor_class::exec_fast_cycle()
{
    if (global_protection || !allowed_to_execution)
        return;

    fast_cycle_device *dev;
    dlist_for_each_entry(dev, &heimer::fast_cycle_list, fast_cycle_list_lnk)
    {
        int sts = dev->fast_cycle_serve();
        if (sts)
        {
            deactivate_process();
        }
    }
}

bool heimer::is_device_ready_for_settings_change()
{

    heimer::signal_processor *proc;
    dlist_for_each_entry(proc, &heimer::signal_processor_list, list_lnk)
    {
        if (proc->is_active())
            return false;
    }
    return true;
}
