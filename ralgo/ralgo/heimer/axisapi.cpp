#include <ralgo/heimer/axisapi.h>
#include <ralgo/heimer/axisctr.h>
#include <ralgo/heimer/signal_processor.h>
#include <vector>

std::vector<heimer::axis_controller *> axises;
std::vector<heimer::axis_controller *> igroups;

void heimer::collect_axis_api()
{
    heimer::signal_processor *proc;

    int axises_total = 0;
    int igroups_total = 0;
    dlist_for_each_entry(proc, &heimer::signal_processor_list, list_lnk)
    {
        if (proc->is_axisctr())
        {
            if (proc->leftdim() == 1)
                axises_total++;
            if (proc->leftdim() > 1)
                igroups_total++;
        }
    }

    axises.clear();
    igroups.clear();
    axises.reserve(axises_total);
    igroups.reserve(igroups_total);

    dlist_for_each_entry(proc, &heimer::signal_processor_list, list_lnk)
    {
        heimer::axis_controller *axproc = static_cast<axis_controller *>(proc);
        if (proc->is_axisctr())
        {
            if (proc->leftdim() == 1)
                axises.push_back(axproc);
            if (proc->leftdim() > 1)
                igroups.push_back(axproc);
        }
    }
}

int heimer::axises_api_command(int argc, char **argv, char *ans, int anslen)
{
    if (argc > 0 && strcmp(argv[0], "list") == 0)
    {
        char buf[16];
        memset(buf, 0, 16);
        for (unsigned int i = 0; i < axises.size(); ++i)
        {
            snprintf(buf, 16, "%s\r\n", axises[i]->name().data());
            strcat(ans, buf);
        }
        return 0;
    }

    if (argc < 2)
    {
        snprintf(ans, anslen, "Usage: ax NUM CMD [OPTS...]\r\n");
        return -1;
    }

    int axno = atoi(argv[0]);

    return axises[axno]->command(argc - 1, argv + 1, ans, anslen);
}

int heimer::igroups_api_command(int argc, char **argv, char *ans, int anslen)
{
    if (argc > 0 && strcmp(argv[0], "list") == 0)
    {
        char buf[16];
        memset(buf, 0, 16);
        for (unsigned int i = 0; i < igroups.size(); ++i)
        {
            snprintf(buf, 16, "%s\r\n", igroups[i]->name().data());
            strcat(ans, buf);
        }
        return 0;
    }

    if (argc < 2)
    {
        snprintf(ans, anslen, "Usage: ig NUM CMD [OPTS...]\r\n");
        return -1;
    }

    int axno = atoi(argv[0]);

    return igroups[axno]->command(argc - 1, argv + 1, ans, anslen);
}
