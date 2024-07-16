
/**
@file
*/

#ifndef IGRIS_HEXASCII_STRING_H
#define IGRIS_HEXASCII_STRING_H

#include <igris/buffer.h>
#include <igris/compiler.h>
#include <string>

namespace igris
{
    std::string hexascii_encode(const uint8_t *data, size_t size);
    std::string hexascii_encode(std::string const &str);
    std::string hexascii_decode(std::string const &str);

    std::string hexascii_encode(igris::buffer const &buf);
    std::string hexascii_decode(igris::buffer const &buf);
}

#endif
