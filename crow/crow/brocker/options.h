/** @file */

#ifndef CROWKER_OPTIONS_H
#define CROWKER_OPTIONS_H

#include <stdint.h>

namespace crowker_implementation
{
    struct options
    {
        uint8_t qos = 0;
        uint16_t ackquant = 200;
        options(uint8_t qos, uint16_t ackquant) : qos(qos), ackquant(ackquant)
        {
        }
        options() = default;
    };
} // namespace crowker_implementation

#endif