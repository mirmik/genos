#include <trigger/comparectr.h>
#include <trigger/scanning_command.h>

extern PeriodicCompareController periodic_compare_x;
extern PeriodicCompareController periodic_compare_y;

int scan_compare_start(const nos::argv &args, nos::ostream &os)
{
    if (args.size() != 1)
    {
        nos::println_to(os, "usage: scan_compare_start <scan_id>");
        return -1;
    }

    int scan_id = atoi(args[0].data());
    switch (scan_id)
    {
    case 0:
        periodic_compare_x.start();
        break;
    case 1:
        periodic_compare_y.start();
        break;
    }

    return 0;
}

int set_scan_points(const nos::argv &args, nos::ostream &os)
{
    if (args.size() != 2)
    {
        nos::println_to(os, "usage: set_scan_points <scan_id> <points>");
        return -1;
    }

    int scan_id = atoi(args[0].data());
    int points = atoi(args[1].data());

    switch (scan_id)
    {
    case 0:
        periodic_compare_x.incpos_points = points;
        break;
    case 1:
        periodic_compare_y.incpos_points = points;
        break;
    }

    return 0;
}

int set_scan_distance(const nos::argv &args, nos::ostream &os)
{
    if (args.size() != 2)
    {
        nos::println_to(os, "usage: set_scan_distance <scan_id> <distance>");
        return -1;
    }

    int scan_id = atoi(args[0].data());
    int distance = atoi(args[1].data());

    switch (scan_id)
    {
    case 0:
        periodic_compare_x.incpos_move = distance;
        break;
    case 1:
        periodic_compare_y.incpos_move = distance;
        break;
    }

    return 0;
}

int eventctr_save(const nos::argv &args, nos::ostream &os)
{
    if (args.size() != 1)
    {
        nos::println_to(os, "usage: eventctr_save <tblidx>");
        return -1;
    }
    auto tblidx = atoi(args[0].data());
    eventctr.save_table(tblidx);
    return 0;
}

int eventctr_load(const nos::argv &args, nos::ostream &os)
{
    if (args.size() != 1)
    {
        nos::println_to(os, "usage: eventctr_load <tblidx>");
        return -1;
    }
    auto tblidx = atoi(args[0].data());
    eventctr.load_table(tblidx);
    return 0;
}

int eventctr_set_loop_total(const nos::argv &args, nos::ostream &os)
{
    if (args.size() != 1)
    {
        nos::println_to(os, "usage: eventctr_set_loop_total <total>");
        return -1;
    }
    auto total = atoi(args[0].data());
    eventctr.set_loop_total(total);
    return 0;
}

int eventctr_reset_table_postfix(const nos::argv &args, nos::ostream &os)
{
    if (args.size() != 1)
    {
        nos::println_to(os, "usage: eventctr_reset_table_postfix <size>");
        return -1;
    }
    auto size = atoi(args[0].data());
    eventctr.reset_table_postfix(size);
    return 0;
}

int eventctr_reset_table_prefix(const nos::argv &args, nos::ostream &os)
{
    if (args.size() != 1)
    {
        nos::println_to(os, "usage: eventctr_reset_table_prefix <size>");
        return -1;
    }
    auto size = atoi(args[0].data());
    eventctr.reset_table_prefix(size);
    return 0;
}

int eventctr_reset_table_cycle(const nos::argv &args, nos::ostream &os)
{
    if (args.size() != 1)
    {
        nos::println_to(os, "usage: eventctr_reset_table_cycle <size>");
        return -1;
    }
    auto size = atoi(args[0].data());
    eventctr.reset_table_cycle(size);
    return 0;
}

int eventctr_push_back(const nos::argv &args, nos::ostream &os)
{
    if (args.size() != 1)
    {
        nos::println_to(os, "usage: eventctr_push_back <value>");
        return -1;
    }
    auto value = atoi(args[0].data());
    eventctr.push_back(value);
    return 0;
}

nos::executor
    trigger_executor({{"scancompstart", "", scan_compare_start},
                      {"scanpoints", "", set_scan_points},
                      {"scandist", "", set_scan_distance},
                      {"evsave", "", eventctr_save},
                      {"evload", "", eventctr_load},
                      {"evlooplen", "", eventctr_set_loop_total},
                      {"evresetpostfix", "", eventctr_reset_table_postfix},
                      {"evresetprefix", "", eventctr_reset_table_prefix},
                      {"evresetcycle", "", eventctr_reset_table_cycle},
                      {"evpush", "", eventctr_push_back}});