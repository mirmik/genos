#include <crow/proto/acceptor.h>
#include <crow/proto/channel.h>
#include <crow/tower.h>
#include <crow/warn.h>

#include <igris/util/bug.h>
#include <nos/print.h>

void crow::channel::incoming_packet(crow::packet *pack)
{
    crow::node_subheader *sh_node = (node_subheader *)pack->dataptr();
    crow::subheader_channel *sh_channel = crow::get_subheader_channel(pack);

    switch (sh_channel->ftype)
    {
        case crow::Frame::HANDSHAKE_REQUEST:
            // Кто-то пытается установить с нами связь.
            if (u.f.naive_listener ||
                u.f._state == CROW_CHANNEL_WAIT_HANDSHAKE_REQUEST)
            {
                crow::subheader_handshake *sh_handshake =
                    crow::get_subheader_handshake(pack);

                // TODO: перенести аллокацию под адрес в другое место
                // raddr_ptr = malloc(pack->addrsize());
                if (pack->addrsize() > raddr_cap)
                    return;

                memcpy(raddr_ptr, pack->addrptr(), pack->addrsize());
                raddr_len = pack->addrsize();
                rid = sh_node->sid;

                this->qos = sh_handshake->qos;
                this->ackquant = sh_handshake->ackquant;

                send_handshake_answer();

                u.f._state = CROW_CHANNEL_CONNECTED;
            }
            else
            {
                crow::warn("WARN: HANDSHAKE_REQUEST on another state");
            }

            break;

        case crow::Frame::HANDSHAKE_ANSWER:
            // Кто-то ответил на зов.
            if (u.f._state == CROW_CHANNEL_WAIT_HANDSHAKE_ANSWER)
            {
                // Если мы еще ни с кем и никогда.
                crow::subheader_handshake *sh_handshake =
                    crow::get_subheader_handshake(pack);

                // TODO: перенести аллокацию под адрес в другое место
                // raddr_ptr = malloc(pack->addrsize());
                if (pack->addrsize() > raddr_cap)
                    return;

                memcpy(raddr_ptr, pack->addrptr(), pack->addrsize());
                raddr_len = pack->addrsize();
                rid = sh_node->sid;
                qos = sh_handshake->qos;
                ackquant = sh_handshake->ackquant;

                u.f._state = CROW_CHANNEL_CONNECTED;
                notify_one(0);
            }
            else
            {
                crow::warn("WARN: HANDSHAKE_ANSWER on another state");
            }

            break;

        case crow::Frame::DATA:
            incoming_data_packet(pack);
            return;

        case crow::Frame::REFUSE:
            u.f._state = CROW_CHANNEL_DISCONNECTED;
            break;

        default:
            BUG();
            break;
    }

    crow::release(pack);
}

void crow::channel::incoming_data_packet(crow::packet *pack)
{
    this->incoming_handler(this, pack);
}

void crow::channel::undelivered_packet(crow::packet *pack)
{
    notify_one(-1);
    u.f._state = CROW_CHANNEL_DISCONNECTED;
    crow::release(pack);
}

void crow::channel::handshake(const uint8_t *raddr_ptr,
                              uint16_t raddr_len,
                              nodeid_t rid,
                              uint8_t qos,
                              uint16_t ackquant)
{
    crow::node_subheader sh_node;
    crow::subheader_channel sh_channel;
    crow::subheader_handshake sh_handshake;

    sh_node.sid = id;
    sh_node.rid = this->rid = rid;
    sh_node.u.f.type = CROW_NODEPACK_COMMON;

    sh_channel.frame_id = 0;
    sh_channel.ftype = crow::Frame::HANDSHAKE_REQUEST;

    sh_handshake.qos = this->qos = qos;
    sh_handshake.ackquant = this->ackquant = ackquant;

    nos::buffer vec[] = {{(char *)&sh_node, sizeof(sh_node)},
                         {(char *)&sh_channel, sizeof(sh_channel)},
                         {(char *)&sh_handshake, sizeof(sh_handshake)}};

    u.f._state = CROW_CHANNEL_WAIT_HANDSHAKE_ANSWER;

    //_state = crow::State::CONNECTED;
    crow::send_v({raddr_ptr, raddr_len}, vec, sizeof(vec) / sizeof(nos::buffer),
                 CROW_NODE_PROTOCOL, 2, ackquant);
}

void crow::channel::send_handshake_answer()
{
    crow::node_subheader sh_node;
    crow::subheader_channel sh_channel;
    crow::subheader_handshake sh_handshake;

    sh_node.sid = id;
    sh_node.rid = rid;
    sh_node.u.f.type = CROW_NODEPACK_COMMON;

    sh_channel.frame_id = 0;
    sh_channel.ftype = crow::Frame::HANDSHAKE_ANSWER;

    sh_handshake.qos = this->qos;
    sh_handshake.ackquant = this->ackquant;

    nos::buffer vec[] = {{(char *)&sh_node, sizeof(sh_node)},
                         {(char *)&sh_channel, sizeof(sh_channel)},
                         {(char *)&sh_handshake, sizeof(sh_handshake)}};

    //_state = crow::State::CONNECTED;
    crow::send_v({raddr_ptr, raddr_len}, vec, sizeof(vec) / sizeof(nos::buffer),
                 CROW_NODE_PROTOCOL, 2, ackquant);
}

int crow::channel::send(const char *data, size_t size)
{
    if (u.f._state != CROW_CHANNEL_CONNECTED)
    {
        return CROW_CHANNEL_ERR_NOCONNECT;
    }

    crow::node_subheader sh_node;
    crow::subheader_channel sh_channel;

    sh_node.sid = this->id;
    sh_node.rid = this->rid;
    sh_node.u.f.type = CROW_NODEPACK_COMMON;

    sh_channel.frame_id = this->fid++;
    sh_channel.ftype = crow::Frame::DATA;

    nos::buffer vec[] = {
        {(char *)&sh_node, sizeof(sh_node)},
        {(char *)&sh_channel, sizeof(sh_channel)},
        {(char *)data, size},
    };

    crow::send_v({this->raddr_ptr, this->raddr_len}, vec,
                 sizeof(vec) / sizeof(nos::buffer), CROW_NODE_PROTOCOL,
                 this->qos, this->ackquant);

    return 0;
}

nos::buffer crow::channel::getdata(crow::packet *pack)
{
    return nos::buffer(pack->dataptr() + sizeof(crow::node_subheader) +
                           sizeof(crow::subheader_channel),
                       pack->datasize() - sizeof(crow::node_subheader) -
                           sizeof(crow::subheader_channel));
}
