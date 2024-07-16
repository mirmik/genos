#include <crow/nodes/pubsub_defs.h>
#include <crow/nodes/service_node.h>
#include <crow/warn.h>
#include <nos/print.h>

void crow::service_node::incoming_packet(crow::packet *pack)
{
    curpack = pack;
    auto &subheader = pack->subheader<consume_subheader>();
    auto data = subheader.message();
    int reply_theme_length = data.data()[0];
    auto message = nos::buffer{data.data() + 1 + reply_theme_length,
                               data.size() - 1 - reply_theme_length};
    dlg(message.data(), message.size(), *this);

    // release after reply
    crow::release(pack);
}

void crow::service_node::reply(const char *answ, size_t size)
{
    auto &subheader = curpack->subheader<consume_subheader>();
    auto data = subheader.message();
    int reply_theme_length = data.data()[0];
    auto reply_theme = nos::buffer(data.data() + 1, reply_theme_length);
    if (reply_theme != "__noanswer__")
        publish(curpack->addr(), subheader.sid, reply_theme, {answ, size}, qos,
                ackquant);
}
