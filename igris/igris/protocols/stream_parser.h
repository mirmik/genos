#ifndef IGRIS_PROTOCOLS_STREAM_H
#define IGRIS_PROTOCOLS_STREAM_H

#include <cstring>

struct stream_parser_operations
{
    void *buffer;
    size_t maxsize;
    size_t cursor;

    void (*newdgram)(void *data, size_t size);
    void (*recvdata)(const void *data, size_t size);
    void (*errhandle)(int errcode);
    void (*reset)();
};

struct stream_parser
{
    const struct stream_parser_operations *ops;
};

__BEGIN_DECLS

void stream_parser_init(struct stream_parser *parser,
                        const struct stream_parser_operations *ops);

void stream_parser_recv(struct stream_parser *parser, const void *data,
                        size_t size);

__END_DECLS

#endif