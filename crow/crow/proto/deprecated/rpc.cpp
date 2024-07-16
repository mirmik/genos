#include <crow/proto/rpc.h>

void crow::rpc_node::incoming_packet(crow::packet *pack)
{
    int8_t format;
    int8_t status;

    nos::buffer data;
    nos::buffer outdata;
    nos::buffer function_name;
    crow::node_subheader *nsh = crow::node::subheader(pack);

    data = crow::node_data(pack);

    igris::archive::binreader reader(data.data(), data.size());

    reader.load(format);
    reader.load_set_buffer(function_name);

    data =
        nos::buffer{(char *)reader.pointer(),
                    (size_t)((char *)reader.end() - (char *)reader.pointer())};

    remote_function_basic *procedure =
        rfuncmap[std::string(function_name.data(), function_name.size())];
    if (procedure == nullptr)
    {
        status = CROW_RPC_ERROR_FUNCTION_NOT_FOUNDED;

        nos::buffer vdata[] = {
            {(char *)&status, 1},
        };

        crow::node::send_v(nsh->rid, nsh->sid,
                           {pack->addrptr(), pack->addrsize()}, vdata, 1, 0,
                           10);

        crow::release(pack);
        return;
    }

    if (format == CROW_RPC_BINARY_FORMAT)
    {
        size_t outsize = procedure->outsize();
        void *outbuf = alloca(outsize);
        outdata = nos::buffer{(char *)outbuf, outsize};
        status = procedure->invoke(data, outdata);

        nos::buffer vdata[] = {{(char *)&status, 1}, outdata};

        crow::node::send_v(nsh->rid, nsh->sid,
                           {pack->addrptr(), pack->addrsize()}, vdata, 2, 0,
                           10);
    }

    else if (format == CROW_RPC_TEXT_FORMAT)
    {
        std::string outdata;
        std::string senddata;
        igris::archive::binary_string_writer writer(senddata);

        status = procedure->invoke_text_format(data, outdata);

        writer.dump(status);
        igris::serialize(writer, outdata);

        crow::node::send(nsh->rid, nsh->sid,
                         {pack->addrptr(), pack->addrsize()}, senddata, 0, 0);
    }

    else
    {
        status = CROW_RPC_ERROR_UNRESOLVED_FORMAT;

        nos::buffer vdata[] = {
            {(char *)&status, 1},
        };

        crow::node::send_v(nsh->rid, nsh->sid,
                           {pack->addrptr(), pack->addrsize()}, vdata, 1, 0,
                           10);
    }

    crow::release(pack);
}
