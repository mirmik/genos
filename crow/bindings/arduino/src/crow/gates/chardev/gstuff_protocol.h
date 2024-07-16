#ifndef CROW_chardev_PROTOCOLS_GSTUFF_H
#define CROW_chardev_PROTOCOLS_GSTUFF_H

namespace crow
{
    class gstuff_protocol : public chardev_protocol
    {
        int size_encoded(crow_packet *pack) override;

        void encode(crow_packet *pack, nos::buffer);
        void decode(crow_packet *pack, nos::buffer);

        void encode_streamed(int *state, char in, char *out) override;
        void decode_streamed(int *state, char in, char *out) override;

        void encode_stream_reset() override;
        void decode_stream_reset() override;
    };
}

#endif
