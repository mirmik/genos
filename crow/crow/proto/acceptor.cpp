#include <crow/proto/acceptor.h>
#include <crow/proto/channel.h>
#include <crow/tower.h>
#include <crow/warn.h>

#include <nos/trace.h>

void crow::acceptor::incoming_packet(crow::packet *pack)
{
    crow::subheader_channel *shc = crow::get_subheader_channel(pack);

    if (shc->ftype == Frame::HANDSHAKE_REQUEST)
    {
        auto ch = init_channel();
        if (ch->id == 0)
            ch->bind(dynport());

        ch->wait_handshake_request();
        ch->incoming_packet(pack);
    }
    else
        crow::release(pack);
}

void crow::acceptor::undelivered_packet(crow::packet *pack)
{
    crow::release(pack);
}

uint16_t crow::dynport()
{
    crow::warn("TODO: crow::dynport");
    return 512;
}
