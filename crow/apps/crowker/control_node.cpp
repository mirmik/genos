#include <crow/brocker/crowker.h>
#include <crow/brocker/crowker_api.h>
#include <crow/nodes/node_delegate.h>
#include <crow/nodes/service_node.h>
#include <igris/shell/rshell_executor.h>
#include <string>

#include <igris/util/dstring.h>
#include <nos/shell/argv.h>
#include <nos/shell/executor.h>

static int themes(const nos::argv &, nos::ostream &os)
{
    auto &themes = crow::crowker::instance()->themes;
    if (themes.size() == 0)
    {
        nos::println_to(os, "No one theme here yet.");
        return 0;
    }

    for (auto &[name, theme] : themes)
    {
        std::string s = nos::format("name:{} count_of_subs:{}\n", name,
                                    theme.count_clients());
        nos::print_to(os, s);
    }
    return 0;
}

static int theme_names(const nos::argv &, nos::ostream &os)
{
    auto &themes = crow::crowker::instance()->themes;
    if (themes.size() == 0)
    {
        nos::println_to(os, "No one theme here yet.");
        return 0;
    }

    for (auto &a : themes)
    {
        std::string s = nos::format("{}\n", a.first);
        nos::print_to(os, s);
    }
    return 0;
}

static int clients(const nos::argv &, nos::ostream &os)
{
    auto clients = crow::crowker::instance()->clients();
    if (clients.size() == 0)
    {
        nos::println_to(os, "No one theme here yet.");
        return 0;
    }

    for (auto &a : clients)
    {
        std::string s = nos::format("name:{}\n", a->name());
        nos::print_to(os, s);
    }
    return 0;
}

static int last_messages(const nos::argv &argv, nos::ostream &os)
{
    std::vector<std::shared_ptr<std::string>> messages;

    if (argv.size() < 2)
    {
        nos::println_to(os, "Usage: last THEME [SIZE]");
        return -1;
    }

    size_t size = 1;
    if (argv.size() >= 3)
    {
        size = (size_t)atoi(argv[2].data());
    }

    auto &name = argv[1];
    auto &themes = crow::crowker::instance()->themes;

    auto theme_pair = themes.find({name.data(), name.size()});
    if (theme_pair == themes.end())
        return 0;
    else
    {
        auto &theme = theme_pair->second;
        std::lock_guard<std::mutex> lock(theme.local_lock());
        if (size > theme.queue_size())
            size = theme.queue_size();
        for (size_t i = 0; i < size; ++i)
        {
            if (theme.queue()[i])
                nos::println_to(os, igris::dstring(*theme.queue()[i]));
        }
    }
    return 0;
}

static int set_queue_size(const nos::argv &argv, nos::ostream &os)
{
    if (argv.size() < 3)
    {
        nos::println_to(os, "Usage: queue-size THEME [SIZE]");
        return -1;
    }
    size_t size = 1;
    if (argv.size() >= 3)
    {
        size = (size_t)atoi(argv[2].data());
    }
    auto &name = argv[1];
    auto &themes = crow::crowker::instance()->themes;
    auto theme_pair = themes.find({name.data(), name.size()});
    if (theme_pair == themes.end())
        return 0;

    theme_pair->second.resize_queue(size);
    return 0;
}

nos::executor executor(
    {nos::command{"clients", "crowker clients", clients},
     nos::command{"themes", "crowker themes", themes},
     nos::command{"theme-names", "crowker themes, only names", theme_names},
     nos::command{"last", "get latest theme messages", last_messages},
     nos::command{"queue-size", "set_queue_size", set_queue_size}});

static void incoming(crow::node_packet_ptr pack)
{
    nos::string_buffer answer;
    executor.execute(nos::tokens(pack.message().data()), answer);
    pack.reply({answer.str().data(), answer.str().size()});
}

static void undelivered(crow::node_packet_ptr pack)
{
    (void)pack;
}

static void incoming_beam(crow::node_packet_ptr)
{
    //	crowker_api.client_beam(pack->addr(), pack.sid(), pack.message());
}

static void control_handler(char *data, int, crow::service_node &node)
{
    nos::string_buffer answer;
    executor.execute(nos::tokens(data), answer);
    node.reply({answer.str().data(), answer.str().size()});
}

crow::node_delegate control(incoming, undelivered);
crow::node_delegate beamsocket(incoming_beam, undelivered);
crow::service_node control_service(control_handler);

void init_control_node()
{
    control.bind(CROWKER_CONTROL_BROCKER_NODE_NO);
    beamsocket.bind(CROWKER_BEAMSOCKET_BROCKER_NODE_NO);
    control_service.init(crow::hostaddr(""), "crowker/control");
    control_service.subscribe();
}