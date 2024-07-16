#include <crow/nodes/rshell_cli_node.h>

void crow::rshell_cli_node_base::incoming_packet(crow::packet *pack)
{
    auto sh = crow::node::subheader(pack);
    auto data = crow::node_data(pack);

    char *ansbuf = (char *)malloc(answer_buffer_size);
    memset(ansbuf, 0, answer_buffer_size);

    handle(data.data(), data.size(), ansbuf, answer_buffer_size - 1);
    int anslen = strlen(ansbuf);

    if (anslen > 0)
    {
        crow::node::send(id, sh->sid,
                         nos::buffer(pack->addrptr(), pack->addrsize()),
                         nos::buffer(ansbuf, anslen), 2, 200);
    }

    free(ansbuf);

    crow::release(pack);
    return;
}
