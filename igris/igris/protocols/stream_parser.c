#include <igris/protocols/stream_parser.h>

void stream_parser_init(struct stream_parser *parser,
                        const struct stream_parser_operations *ops)
{
    parser->ops = ops;
}

void stream_parser_recv(struct stream_parser *parser, const void *data,
                        size_t size)
{
    parser->recvdata(parser, data, size);
}