#ifndef NOS_SERIAL_OSTREAM_H
#define NOS_SERIAL_OSTREAM_H

#include <zillot/common/uartring.h>

class nos_serial_ostream : public nos::ostream
{
public:
    zillot::uartring *ring;

    nos_serial_ostream(zillot::uartring *ring) : ring(ring) {}

    nos::expected<size_t, nos::output_error> write(const void *data,
                                                   size_t size) override
    {
        return ring->write(data, size);
    }
};

#endif