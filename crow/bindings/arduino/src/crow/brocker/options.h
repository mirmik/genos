/** @file */

#ifndef CROWKER_OPTIONS_H
#define CROWKER_OPTIONS_H

#include <stdint.h>

namespace crowker_implementation
{
    struct options
    {
    };

    struct crow_options : public options
    {
        uint8_t qos = 0;
        uint16_t ackquant = 200;
        crow_options(uint8_t qos, uint16_t ackquant)
            : qos(qos), ackquant(ackquant)
        {
        }
    };
} // namespace crowker_implementation

#endif