#include <cctype>
#include <comm/console.h>
#include <devices/Device.h>
#include <groups/InterpolationGroup.h>
#include <nos-shell/standart.h>
#include <nos/io/stdfile.h>
#include <nos/log.h>
#include <string>
#include <string_view>
#include <tables.h>
#include <thread>

int hello(const nos::argv &args, nos::ostream &out)
{
    nos::println_to(out, "HelloWorld");
    return 0;
}

int remove_axis(const nos::argv &args, nos::ostream &out)
{
    int axno = args[1].to_int();
    delete_axis(axno);
    return 0;
}

int remove_device(const nos::argv &args, nos::ostream &out)
{
    int devno = args[1].to_int();
    devices_list.erase(std::next(devices_list.begin(), devno));
    return 0;
}

int info(const nos::argv &args, nos::ostream &out)
{
    nos::println_to(out, "Axes:");
    for (auto *ax : get_axis_list())
    {
        nos::fprintln_to(out, "\t{}", ax->name());
    }
    nos::println_to(out, "Groups:");
    for (auto *grp : get_group_list())
    {
        nos::fprintln_to(out, "\t{}", grp->name());
    }
    nos::println_to(out, "Devices:");
    for (auto *dev : devices_list)
    {
        nos::fprintln_to(out, "\t{}", dev->name());
    }
    return 0;
}

Device *find_device(const std::string &name)
{
    if (isdigit(name[0]))
    {
        int no = std::stoi(name);
        return devices_list[no];
    }
    else
    {
        auto it =
            std::find_if(devices_list.begin(),
                         devices_list.end(),
                         [&name](Device *dev) { return dev->name() == name; });
        if (it != devices_list.end())
            return *it;
        return nullptr;
    }
}

int devcmd(const nos::argv &args, nos::ostream &out)
{
    auto *dev = find_device(args[1]);
    if (dev == nullptr)
    {
        nos::fprintln_to(out, "Device {} not found", args[1]);
        return 1;
    }
    dev->command(args.without(2), out);
    return 0;
}

int axcmd(const nos::argv &args, nos::ostream &out)
{
    auto *ax = find_axis(args[1]);
    if (ax == nullptr)
    {
        nos::fprintln_to(out, "Axis {} not found", args[1]);
        return 1;
    }
    ax->command(args.without(2), out);
    return 0;
}

int grpcmd(const nos::argv &args, nos::ostream &out)
{
    if (args.size() < 2)
    {
        nos::println_to(out, "Usage: grp <group> <command>");
        return 1;
    }

    auto *grp = find_group(args[1]);
    if (grp == nullptr)
    {
        nos::fprintln_to(out, "Group {} not found", args[1]);
        return 1;
    }
    grp->command(args.without(2), out);
    return 0;
}

int devlist(const nos::argv &args, nos::ostream &out)
{
    for (size_t i = 0; i < devices_list.size(); i++)
    {
        auto *dev = devices_list[i];
        nos::fprintln_to(
            out, "{}: {} - {}", i, dev->name(), dev->declared_type());
    }
    return 0;
}

int grplist(const nos::argv &args, nos::ostream &out)
{
    for (size_t i = 0; i < get_group_list().size(); i++)
    {
        auto *grp = get_group_list()[i];
        nos::fprintln_to(
            out, "{}: {} - {}", i, grp->name(), grp->declared_type());
    }
    return 0;
}

int axlist(const nos::argv &args, nos::ostream &out)
{
    for (size_t i = 0; i < axes_total(); i++)
    {
        auto *ax = get_axis(i);
        nos::fprintln_to(
            out, "{}: {} - {}", i, ax->name(), ax->declared_type());
    }
    return 0;
}

int console_exit(const nos::argv &args, nos::ostream &out)
{
    exit(0);
}
nos::executor executor({{"hello", "helloworld", hello},
                        {"quit", "", console_exit},
                        {"q", "", console_exit},
                        {"remove_axis", "", remove_axis},
                        {"remove_device", "", remove_device},
                        {"exit", "", console_exit},
                        {"dev", "", devcmd},
                        {"ax", "", axcmd},
                        {"grp", "", grpcmd},
                        {"devlist", "", devlist},
                        {"axlist", "", axlist},
                        {"grplist", "", grplist},
                        {"info", "", info}});

nos::cancel_token_source global_cancel;

std::string execute_tokens(nos::tokens &tokens)
{
    try
    {
        nos::string_buffer sb;
        if (tokens.size() == 0)
            return "";
        executor.execute(tokens, sb);
        return sb.str();
    }
    catch (std::exception &e)
    {
        return std::string("Error: ") + e.what();
    }
}

int userIOThreadHandler()
{
    std::string str;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    while (1)
    {
        std::cout << "$ ";
        getline(std::cin, str);
        nos::tokens tokens(str);
        auto sb = execute_tokens(tokens);
        if (sb.size() == 0)
            continue;
        std::cout << sb;
    }
    return 0;
}

std::thread io_console_thread;
void start_io_console_thread()
{
    nos::log::info("Starting terminal console thread");
    io_console_thread = std::thread(userIOThreadHandler);
}

std::thread tcp_console_thread;
void start_tcp_console_thread(int port)
{
    nos::log::info("Starting tcp console thread");
    tcp_console_thread = std::thread(nos::shell_tcp_thread_function,
                                     port,
                                     std::ref(executor),
                                     global_cancel.token());
}
