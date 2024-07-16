#ifndef RALGO_HEIMER_COMMAND_CENTER_H
#define RALGO_HEIMER_COMMAND_CENTER_H

#include <ctype.h>
#include <ralgo/oldheimer/axisctr.h>
#include <ralgo/oldheimer/linintctr.h>

namespace heimer
{
    // TODO: Переработать интерфейсы, чтобы класс мог стать нешаблонным
    template <class P, class V> class command_center_cls
    {
        igris::array_view<axisctr<P, V> *> axes = {};
        igris::array_view<linintctr_basic<P, V> *> igroups = {};

    public:
        command_center_cls() {}

    public:
        void attach_axes(igris::array_view<axisctr<P, V> *> axes)
        {
            this->axes = axes;
        }
        void attach_igroups(igris::array_view<linintctr_basic<P, V> *> igroups)
        {
            this->igroups = igroups;
        }

        axisctr<P, V> *find_axis(const char *name)
        {
            if (isdigit(*name))
            {
                unsigned int axno = strtol(name, nullptr, 10);
                if (axno < axes.size())
                    return axes[axno];
            }

            else
            {
                for (auto *ax : axes)
                {
                    if (strcmp(name, ax->mnemo()) == 0)
                        return ax;
                }
            }

            nos::println("undefined axis");
            return nullptr;
        }

        int axcmd(int argc, char **argv)
        {

            const char *usage =
                "usage: ax AXNO CMD [ARGS ...]\r\n"
                "CMD:\r\n"
                "mov         - incremental move\r\n"
                "incmov      - absolute move\r\n"
                "stop        - stop current operation\r\n"
                "setspd      - set speed value\r\n"
                "setacc      - set acceleration/deceleration values\r\n"
                "setgain     - set gain for input commands\r\n"
                "setreverse  - set reverse for input commands\r\n"
                "setlim      - set forward/backward limits\r\n"
                "feed        - print current feedback information (?)\r\n"
                "pos         - print current position\r\n"
                "name        - print axis name";

            if (argc < 2)
            {
                nos::println(usage);
                return 0;
            }

            if (strcmp(argv[1], "list") == 0)
            {
                for (unsigned int i = 0; i < axes.size(); ++i)
                {
                    nos::fprintln("{}: {}", i, axes[i]->mnemo());
                }
                return 0;
            }

            if (argc < 3)
            {
                nos::println(usage);
                return 0;
            }

            axisctr<P, V> *ax = find_axis(argv[1]);
            if (!ax)
            {
                nos::println("undefined axisctr");
                return -1;
            }

            ax->command(argc - 2, argv + 2);
            return 0;
        }

        int igcmd(int argc, char **argv)
        {
            const char *usage = "usage: ig AXNO CMD [ARGS ...]; igcmd --list";

            if (argc < 2)
            {
                nos::println(usage);
                return 0;
            }

            if (strcmp(argv[1], "list") == 0)
            {
                for (unsigned int i = 0; i < igroups.size(); ++i)
                {
                    nos::fprintln("{}: {}", i, igroups[i]->mnemo());
                }
                return 0;
            }

            if (argc < 3)
            {
                nos::println(usage);
                return 0;
            }

            unsigned int igno = strtol(argv[1], nullptr, 10);

            if (igno >= igroups.size())
            {
                nos::println("undefined igroup");
                return 0;
            }

            igroups[igno]->command(argc - 2, argv + 2);
            return 0;
        }

        control_node *find_node_by_name(const char *name)
        {
            control_node *node;
            dlist_for_each_entry(
                node, &control_node_list, control_node_list_lnk)
            {
                if (strcmp(node->mnemo(), name) == 0)
                {
                    return node;
                }
            }

            return NULL;
        }

        int ctrcmd(int argc, char **argv)
        {
            control_node *node;
            const char *name;

            if (argc == 1 || strcmp(argv[1], "--help") == 0)
            {
                goto __usage__;
            }

            if (strcmp(argv[1], "list") == 0)
            {
                control_node *node;
                dlist_for_each_entry(
                    node, &control_node_list, control_node_list_lnk)
                {
                    nos::fprintln("{:10}  sts:{:3}  ctr:{:10}",
                                  node->mnemo(),
                                  node->is_active() ? "on" : "off",
                                  node->last_controller()
                                      ? node->last_controller()->mnemo()
                                      : "none");
                }
                return 0;
            }

            name = argv[1];
            node = find_node_by_name(argv[1]);
            if (node == NULL)
            {
                nos::println("wrong node name");
                return -1;
            }

            if (strcmp(argv[2], "on") == 0)
            {
                if (argc != 3)
                {
                    nos::println("wrong count of args");
                    return -1;
                }

                if (node->is_active())
                {
                    nos::println(name, ":", "currently active");
                    return 0;
                }

                int sts = node->activate();
                if (sts)
                {
                    nos::println("activation error");
                    return -1;
                }

                if (node->is_active())
                {
                    nos::println(name, "activation ... green");
                }
                else
                {
                    nos::println(name, "activation ... fault");
                    return -1;
                }
                return 0;
            }

            if (strcmp(argv[2], "off") == 0)
            {
                if (argc != 3)
                {
                    nos::println("wrong count of args");
                    return -1;
                }

                int sts = node->deactivate();
                if (sts)
                {
                    nos::println("deactivation error");
                    return -1;
                }

                if (!node->is_active())
                {
                    nos::println(name, "deactivation ... green");
                }
                else
                {
                    nos::println(name, "deactivation ... fault");
                }

                return 0;
            }

            if (strcmp(argv[2], "status") == 0)
            {
                if (argc != 3)
                {
                    nos::println("wrong count of args");
                    return -1;
                }

                nos::println("\tis_active:", node->is_active());
                nos::println("\tis_controlled:", node->is_controlled());
                nos::println("\tis_alarmed:", node->is_alarmed());
                return 0;
            }

            if (strcmp(argv[2], "info") == 0)
            {
                if (argc != 3)
                {
                    nos::println("wrong count of args");
                    return -1;
                }

                node->print_info();
                return 0;
            }

            if (node->internal_command(argc - 2, argv + 2) == 0)
                return 0;

        __usage__:
            nos::println("Usage:");
            nos::println("\tctr list");
            nos::println("\tctr NODENAME on");
            nos::println("\tctr NODENAME off");
            nos::println("\tctr NODENAME status");
            nos::println("\tctr NODENAME info");

            return -1;
        }
    };

    int axcmd(int argc, char **argv);
    int igcmd(int argc, char **argv);
    int ctrcmd(int argc, char **argv);

    extern heimer::command_center_cls<float, float> command_center;
    extern igris::console_command command_center_cmdtable[];
}

#endif
